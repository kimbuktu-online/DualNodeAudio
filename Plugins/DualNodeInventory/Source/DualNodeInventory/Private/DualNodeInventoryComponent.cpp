#include "DualNodeInventoryComponent.h"
#include "Net/UnrealNetwork.h"
#include "Logging/StructuredLog.h"

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

	int32 RemainingAmount = Amount;

	if (ItemDef->bCanStack)
	{
		for (FDualNodeItemInstance& Slot : InventoryArray.Items)
		{
			if (Slot.ItemDefinition == ItemDef && Slot.StackCount < ItemDef->MaxStackSize)
			{
				int32 CanAdd = ItemDef->MaxStackSize - Slot.StackCount;
				int32 ToAdd = FMath::Min(RemainingAmount, CanAdd);

				Slot.StackCount += ToAdd;
				RemainingAmount -= ToAdd;

				InventoryArray.MarkItemDirty(Slot);

				if (RemainingAmount <= 0) break;
			}
		}
	}

	while (RemainingAmount > 0)
	{
		if (InventoryArray.Items.Num() < MaxSlotCount)
		{
			FDualNodeItemInstance& NewItem = InventoryArray.Items.AddDefaulted_GetRef();
			NewItem.ItemDefinition = ItemDef;
			NewItem.StackCount = FMath::Min(RemainingAmount, ItemDef->MaxStackSize);
			NewItem.InstanceGuid = FGuid::NewGuid();

			RemainingAmount -= NewItem.StackCount;
			InventoryArray.MarkArrayDirty();
		}
		else
		{
			UE_LOGFMT(LogTemp, Warning, "Inventory on {Owner} is full!", GetOwner()->GetName());
			break;
		}
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

	int32 RemainingToRemove = Amount;

	for (int32 i = InventoryArray.Items.Num() - 1; i >= 0; --i)
	{
		if (InventoryArray.Items[i].ItemDefinition == ItemDef)
		{
			int32 InSlot = InventoryArray.Items[i].StackCount;
			int32 ToRemove = FMath::Min(RemainingToRemove, InSlot);

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

void UDualNodeInventoryComponent::OnRep_Inventory()
{
	OnInventoryUpdated.Broadcast(this);
}