#include "DualNodeInventoryComponent.h"
#include "Net/UnrealNetwork.h"
#include "Logging/StructuredLog.h"
#include "Engine/AssetManager.h"

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

	FPrimaryAssetId TargetId = ItemDef->GetPrimaryAssetId();
	int32 RemainingAmount = Amount;

	// 1. Stacking-Check [cite: 19, 20]
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

	// 2. Neue Slots belegen [cite: 23]
	while (RemainingAmount > 0 && InventoryArray.Items.Num() < MaxSlotCount)
	{
		FDualNodeItemInstance& NewItem = InventoryArray.Items.AddDefaulted_GetRef();
		NewItem.ItemId = TargetId;
		NewItem.CachedDefinition = ItemDef;
		NewItem.StackCount = FMath::Min(RemainingAmount, ItemDef->MaxStackSize);
		
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

			if (InventoryArray.Items[i].StackCount <= 0)
			{
				InventoryArray.Items.RemoveAt(i);
				InventoryArray.MarkArrayDirty();
			}
			else
			{
				InventoryArray.MarkItemDirty(InventoryArray.Items[i]);
			}

			if (RemainingToRemove <= 0) break;
		}
	}

	if (RemainingToRemove < Amount)
	{
		OnRep_Inventory();
		return true;
	}
	return false;
}

int32 UDualNodeInventoryComponent::FindStackableSlot(const UDualNodeItemDefinition* ItemDef) const
{
	if (!ItemDef) return INDEX_NONE;
	FPrimaryAssetId TargetId = ItemDef->GetPrimaryAssetId();

	for (int32 i = 0; i < InventoryArray.Items.Num(); ++i)
	{
		if (InventoryArray.Items[i].ItemId == TargetId && InventoryArray.Items[i].StackCount < ItemDef->MaxStackSize)
		{
			return i;
		}
	}
	return INDEX_NONE;
}

int32 UDualNodeInventoryComponent::FindFreeSlot() const
{
	return (InventoryArray.Items.Num() < MaxSlotCount) ? InventoryArray.Items.Num() : INDEX_NONE;
}

void UDualNodeInventoryComponent::OnRep_Inventory()
{
	// Sicherstellen, dass alle Definitionen auf Clients lokal aufgelöst werden
	for (FDualNodeItemInstance& Slot : InventoryArray.Items)
	{
		Slot.ResolveDefinition();
	}
	OnInventoryUpdated.Broadcast(this);
}