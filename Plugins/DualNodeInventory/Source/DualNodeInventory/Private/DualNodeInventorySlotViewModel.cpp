#include "DualNodeInventorySlotViewModel.h"
#include "DualNodeItemDefinition.h"
#include "DualNodeInventoryTypes.h"
#include "DualNodeItemFragment_UseAction.h"

void UDualNodeInventorySlotViewModel::UpdateSlot(const FDualNodeItemInstance& ItemInstance, int32 InSlotIndex)
{
	ItemGuid = ItemInstance.InstanceGuid;
	SlotIndex = InSlotIndex;

	if (ItemInstance.CachedDefinition)
	{
		SetItemName(ItemInstance.CachedDefinition->ItemName);
		SetItemIcon(ItemInstance.CachedDefinition->ItemIcon);
		
		FText NewQuantity = (ItemInstance.StackCount > 1) 
			? FText::Format(NSLOCTEXT("InventoryUI", "QuantityLabel", "x{0}"), FText::AsNumber(ItemInstance.StackCount)) 
			: FText::GetEmpty();
		SetQuantityText(NewQuantity);

		SetIsUsable(ItemInstance.CachedDefinition->FindFragmentByClass(UDualNodeItemFragment_UseAction::StaticClass()) != nullptr);

		if (ItemInstance.CachedDefinition->Rarity) SetRarityColor(ItemInstance.CachedDefinition->Rarity->RarityColor);
		else SetRarityColor(FColor::White);
	}
	else
	{
		SetItemName(FText::GetEmpty());
		SetItemIcon(nullptr);
		SetQuantityText(FText::GetEmpty());
		SetRarityColor(FColor(0, 0, 0, 0));
		SetIsUsable(false);
	}
}

void UDualNodeInventorySlotViewModel::SetItemIcon(TObjectPtr<UTexture2D> NewValue) { UE_MVVM_SET_PROPERTY_VALUE(ItemIcon, NewValue); }
void UDualNodeInventorySlotViewModel::SetItemName(FText NewValue) { UE_MVVM_SET_PROPERTY_VALUE(ItemName, NewValue); }
void UDualNodeInventorySlotViewModel::SetQuantityText(FText NewValue) { UE_MVVM_SET_PROPERTY_VALUE(QuantityText, NewValue); }
void UDualNodeInventorySlotViewModel::SetRarityColor(FColor NewValue) { UE_MVVM_SET_PROPERTY_VALUE(RarityColor, NewValue); }
void UDualNodeInventorySlotViewModel::SetIsUsable(bool bNewValue) { UE_MVVM_SET_PROPERTY_VALUE(bIsUsable, bNewValue); }