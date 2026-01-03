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

bool UDualNodeInventoryComponent::TryAddItem(const UDualNodeItemDefinition* ItemDef, int32 Amount)
{
	if (!GetOwner()->HasAuthority() || !ItemDef || Amount <= 0) return false;

	FText FailReason;
	// FIX: Iteration über TObjectPtr erfordert expliziten Typ oder Wrapper-Referenz
	for (const TObjectPtr<UDualNodeInventoryValidator>& ValidatorPtr : Validators)
	{
		if (ValidatorPtr && !ValidatorPtr->CanAddItem(this, ItemDef, Amount, FailReason))
		{
			UE_LOGFMT(LogTemp, Warning, "DualNode: Validierung fehlgeschlagen: {Reason}", FailReason.ToString());
			return false;
		}
	}

	FPrimaryAssetId TargetId = ItemDef->GetPrimaryAssetId();
	int32 RemainingAmount = Amount;

	if (ItemDef->bCanStack)
	{
		for (FDualNodeItemInstance& Slot : InventoryArray.Items)
		{
			if (Slot.ItemId == TargetId && Slot.StackCount < ItemDef->MaxStackSize)
			{
				int32 ToAdd = FMath::Min(RemainingAmount, ItemDef->MaxStackSize - Slot.StackCount);
				Slot.StackCount += ToAdd;
				RemainingAmount -= ToAdd;
				InventoryArray.MarkItemDirty(Slot);
				if (RemainingAmount <= 0) break;
			}
		}
	}

	while (RemainingAmount > 0 && InventoryArray.Items.Num() < MaxSlotCount)
	{
		FDualNodeItemInstance& NewItem = InventoryArray.Items.AddDefaulted_GetRef();
		NewItem.ItemId = TargetId;
		NewItem.CachedDefinition = ItemDef;
		NewItem.StackCount = FMath::Min(RemainingAmount, ItemDef->MaxStackSize);
		NewItem.InstanceGuid = FGuid::NewGuid();
		RemainingAmount -= NewItem.StackCount;
		InventoryArray.MarkArrayDirty();
	}

	if (RemainingAmount < Amount)
	{
		OnRep_Inventory();
		return true;
	}
	return false;
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