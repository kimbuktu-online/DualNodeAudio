#include "DualNodeInventoryComponent.h"
#include "Net/UnrealNetwork.h"
#include "Logging/StructuredLog.h"
#include "DualNodeItemDefinition.h"

UDualNodeInventoryComponent::UDualNodeInventoryComponent()
{
	SetIsReplicatedByDefault(true);
	InventoryArray.OwnerComponent = this;
}

void UDualNodeInventoryComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(UDualNodeInventoryComponent, InventoryArray);
}

bool UDualNodeInventoryComponent::CanAddItem(const UDualNodeItemDefinition* ItemDef, int32 Amount, FText& OutFailureReason) const
{
	if (!ItemDef || Amount <= 0) return false;

	// 1. Validatoren prüfen
	for (const TObjectPtr<UDualNodeInventoryValidator>& ValidatorPtr : Validators)
	{
		if (ValidatorPtr && !ValidatorPtr->CanAddItem(this, ItemDef, Amount, OutFailureReason)) return false;
	}

	// 2. Platz prüfen (Stacking oder Free Slot)
	if (ItemDef->bCanStack && FindStackableSlot(ItemDef) != INDEX_NONE) return true;
	if (FindFreeSlot() != INDEX_NONE) return true;

	OutFailureReason = NSLOCTEXT("Inventory", "Full", "Inventar ist voll.");
	return false;
}

bool UDualNodeInventoryComponent::TryAddItem(const UDualNodeItemDefinition* ItemDef, int32 Amount)
{
	if (!GetOwner()->HasAuthority()) return false;

	FText DummyReason;
	if (!CanAddItem(ItemDef, Amount, DummyReason)) return false;

	FPrimaryAssetId TargetId = ItemDef->GetPrimaryAssetId();
	int32 RemainingAmount = Amount;

	// Stacking
	if (ItemDef->bCanStack)
	{
		int32 StackSlot = FindStackableSlot(ItemDef);
		while (StackSlot != INDEX_NONE && RemainingAmount > 0)
		{
			FDualNodeItemInstance& Slot = InventoryArray.Items[StackSlot];
			int32 ToAdd = FMath::Min(RemainingAmount, ItemDef->MaxStackSize - Slot.StackCount);
			Slot.StackCount += ToAdd;
			RemainingAmount -= ToAdd;
			InventoryArray.MarkItemDirty(Slot);
			StackSlot = FindStackableSlot(ItemDef);
		}
	}

	// Neue Slots
	while (RemainingAmount > 0)
	{
		int32 FreeSlot = FindFreeSlot();
		if (FreeSlot == INDEX_NONE) break;

		FDualNodeItemInstance& NewItem = InventoryArray.Items.AddDefaulted_GetRef();
		NewItem.ItemId = TargetId;
		NewItem.CachedDefinition = ItemDef;
		NewItem.StackCount = FMath::Min(RemainingAmount, ItemDef->MaxStackSize);
		NewItem.InstanceGuid = FGuid::NewGuid();
		RemainingAmount -= NewItem.StackCount;
		InventoryArray.MarkArrayDirty();
	}

	OnRep_Inventory();
	return (RemainingAmount < Amount);
}

bool UDualNodeInventoryComponent::RemoveItem(const UDualNodeItemDefinition* ItemDef, int32 Amount)
{
	if (!GetOwner()->HasAuthority() || !ItemDef || Amount <= 0) return false;

	FPrimaryAssetId TargetId = ItemDef->GetPrimaryAssetId();
	int32 RemainingToRemove = Amount;

	for (int32 i = InventoryArray.Items.Num() - 1; i >= 0; --i)
	{
		if (InventoryArray.Items[i].ItemId == TargetId)
		{
			int32 ToRemove = FMath::Min(RemainingToRemove, InventoryArray.Items[i].StackCount);
			InventoryArray.Items[i].StackCount -= ToRemove;
			RemainingToRemove -= ToRemove;

			if (InventoryArray.Items[i].StackCount <= 0) { InventoryArray.Items.RemoveAt(i); InventoryArray.MarkArrayDirty(); }
			else { InventoryArray.MarkItemDirty(InventoryArray.Items[i]); }
			if (RemainingToRemove <= 0) break;
		}
	}

	if (RemainingToRemove < Amount) { OnRep_Inventory(); return true; }
	return false;
}

int32 UDualNodeInventoryComponent::FindStackableSlot(const UDualNodeItemDefinition* ItemDef) const
{
	FPrimaryAssetId TargetId = ItemDef->GetPrimaryAssetId();
	for (int32 i = 0; i < InventoryArray.Items.Num(); ++i)
	{
		if (InventoryArray.Items[i].ItemId == TargetId && InventoryArray.Items[i].StackCount < ItemDef->MaxStackSize) return i;
	}
	return INDEX_NONE;
}

int32 UDualNodeInventoryComponent::FindFreeSlot() const
{
	return (InventoryArray.Items.Num() < MaxSlotCount) ? InventoryArray.Items.Num() : INDEX_NONE;
}

float UDualNodeInventoryComponent::GetTotalWeight() const
{
	float Weight = 0.0f;
	for (const auto& Item : InventoryArray.Items) if (Item.CachedDefinition) Weight += (Item.CachedDefinition->ItemWeight * Item.StackCount);
	return Weight;
}

int32 UDualNodeInventoryComponent::GetTotalAmountOfItem(const UDualNodeItemDefinition* ItemDef) const
{
	return ItemDef ? GetTotalAmountOfItemById(ItemDef->GetPrimaryAssetId()) : 0;
}

int32 UDualNodeInventoryComponent::GetTotalAmountOfItemById(FPrimaryAssetId ItemId) const
{
	int32 Total = 0;
	for (const auto& Item : InventoryArray.Items) if (Item.ItemId == ItemId) Total += Item.StackCount;
	return Total;
}

FDualNodeInventorySaveData UDualNodeInventoryComponent::GetInventorySnapshot() const
{
	FDualNodeInventorySaveData Snapshot;
	for (const auto& Item : InventoryArray.Items)
	{
		FDualNodeItemSaveData& Data = Snapshot.SavedItems.AddDefaulted_GetRef();
		Data.ItemId = Item.ItemId; Data.StackCount = Item.StackCount; Data.InstanceGuid = Item.InstanceGuid;
	}
	return Snapshot;
}

void UDualNodeInventoryComponent::LoadInventoryFromSnapshot(const FDualNodeInventorySaveData& Snapshot)
{
	if (!GetOwner()->HasAuthority()) return;
	InventoryArray.Items.Empty();
	for (const auto& Saved : Snapshot.SavedItems)
	{
		FDualNodeItemInstance& NewItem = InventoryArray.Items.AddDefaulted_GetRef();
		NewItem.ItemId = Saved.ItemId; NewItem.StackCount = Saved.StackCount; NewItem.InstanceGuid = Saved.InstanceGuid;
		NewItem.ResolveDefinition();
	}
	InventoryArray.MarkArrayDirty();
	OnRep_Inventory();
}

void UDualNodeInventoryComponent::OnRep_Inventory()
{
	for (FDualNodeItemInstance& Slot : InventoryArray.Items) Slot.ResolveDefinition();
	OnInventoryUpdated.Broadcast(this);
}