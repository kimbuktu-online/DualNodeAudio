#include "DualNodeContextMenu.h"
#include "DualNodeInventorySlotViewModel.h"
#include "DualNodeInventoryComponent.h"
#include "DualNodeItemDefinition.h"
#include "DualNodeInventoryLibrary.h"
#include "GameFramework/Pawn.h"

void UDualNodeContextMenu::InitializeMenu(UDualNodeInventorySlotViewModel* InSlotVM)
{
	TargetSlotVM = InSlotVM;
}

void UDualNodeContextMenu::CloseMenu()
{
	RemoveFromParent();
}

UDualNodeInventoryComponent* UDualNodeContextMenu::GetOwningInventory() const
{
	// Nutzt die Library, aber mit sicherem internen Context
	return UDualNodeInventoryLibrary::GetInventoryComponent(GetOwningPlayerPawn());
}

const UDualNodeItemDefinition* UDualNodeContextMenu::GetTargetItemDefinition() const
{
	if (TargetSlotVM && TargetSlotVM->ItemGuid.IsValid())
	{
		if (UDualNodeInventoryComponent* Inv = GetOwningInventory())
		{
			for (const FDualNodeItemInstance& Instance : Inv->GetItems())
			{
				if (Instance.InstanceGuid == TargetSlotVM->ItemGuid)
				{
					return Instance.CachedDefinition;
				}
			}
		}
	}
	return nullptr;
}

void UDualNodeContextMenu::ExecuteUse()
{
	UDualNodeInventoryComponent* Inv = GetOwningInventory();
	if (Inv && TargetSlotVM)
	{
		Inv->Server_UseItemInSlot(TargetSlotVM->SlotIndex);
		CloseMenu();
	}
}

void UDualNodeContextMenu::ExecuteDrop(int32 Amount)
{
	UDualNodeInventoryComponent* Inv = GetOwningInventory();
	if (Inv && TargetSlotVM && Amount > 0)
	{
		Inv->Server_DropFromSlot(TargetSlotVM->SlotIndex, Amount);
		CloseMenu();
	}
}

void UDualNodeContextMenu::NativeOnMouseLeave(const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseLeave(InMouseEvent);
	// CloseMenu();
}