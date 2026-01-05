#include "DualNodeInventorySlotViewModel.h"
#include "DualNodeItemDefinition.h"
#include "DualNodeInventoryComponent.h"
#include "DualNodeItemFragment_Durability.h"
#include "DualNodeItemFragment_UseAction.h"

void UDualNodeInventorySlotViewModel::UpdateSlot(const FDualNodeItemInstance& ItemInstance, int32 InSlotIndex, UDualNodeInventoryComponent* InInventory)
{
	ItemGuid = ItemInstance.InstanceGuid;
	SlotIndex = InSlotIndex;

	if (ItemInstance.CachedDefinition && InInventory)
	{
		UE_MVVM_SET_PROPERTY_VALUE(ItemName, ItemInstance.CachedDefinition->ItemName);
		UE_MVVM_SET_PROPERTY_VALUE(ItemDescription, ItemInstance.CachedDefinition->ItemDescription);
		UE_MVVM_SET_PROPERTY_VALUE(ItemIcon, ItemInstance.CachedDefinition->ItemIcon);
		UE_MVVM_SET_PROPERTY_VALUE(StackCount, ItemInstance.StackCount);

		FText NewQuantity = (ItemInstance.StackCount > 1) 
			? FText::Format(NSLOCTEXT("InventoryUI", "QuantityLabel", "x{0}"), FText::AsNumber(ItemInstance.StackCount)) 
			: FText::GetEmpty();
		UE_MVVM_SET_PROPERTY_VALUE(QuantityText, NewQuantity);

		// V2.0: Haltbarkeit abfragen
		float Pct = InInventory->GetItemDurabilityPercent(SlotIndex);
		UE_MVVM_SET_PROPERTY_VALUE(DurabilityPercent, Pct);

		const UDualNodeItemFragment* DurFrag = ItemInstance.CachedDefinition->FindFragmentByClass(UDualNodeItemFragment_Durability::StaticClass());
		UE_MVVM_SET_PROPERTY_VALUE(bShowDurability, DurFrag != nullptr);

		if (const UDualNodeItemFragment_Durability* CastDur = Cast<UDualNodeItemFragment_Durability>(DurFrag))
		{
			if (CastDur->DurabilityType == EDualNodeDurabilityType::TimeBased)
			{
				// In einem echten HUD-Tick würde das hier öfter aktualisiert werden.
				// Hier setzen wir den Initialwert.
				int32 RemainingSeconds = FMath::Max(0, FMath::RoundToInt((ItemInstance.ExpirationTimestamp - InInventory->GetWorld()->GetTimeSeconds())));
				FText TimeText = FText::Format(NSLOCTEXT("InventoryUI", "ExpiresIn", "{0}s"), FText::AsNumber(RemainingSeconds));
				UE_MVVM_SET_PROPERTY_VALUE(DurabilityCountdownText, TimeText);
			}
		}

		UE_MVVM_SET_PROPERTY_VALUE(bIsUsable, ItemInstance.CachedDefinition->FindFragmentByClass(UDualNodeItemFragment_UseAction::StaticClass()) != nullptr);
		
		if (ItemInstance.CachedDefinition->Rarity) UE_MVVM_SET_PROPERTY_VALUE(RarityColor, ItemInstance.CachedDefinition->Rarity->RarityColor);
		else UE_MVVM_SET_PROPERTY_VALUE(RarityColor, FColor::White);
	}
	else
	{
		// Reset-Logik für leere Slots
		UE_MVVM_SET_PROPERTY_VALUE(StackCount, 0);
		UE_MVVM_SET_PROPERTY_VALUE(bShowDurability, false);
		UE_MVVM_SET_PROPERTY_VALUE(RarityColor, FColor(0,0,0,0));
	}
}

void UDualNodeInventorySlotViewModel::SetItemIcon(TObjectPtr<UTexture2D> NewValue) { UE_MVVM_SET_PROPERTY_VALUE(ItemIcon, NewValue); }
void UDualNodeInventorySlotViewModel::SetItemName(FText NewValue) { UE_MVVM_SET_PROPERTY_VALUE(ItemName, NewValue); }
void UDualNodeInventorySlotViewModel::SetQuantityText(FText NewValue) { UE_MVVM_SET_PROPERTY_VALUE(QuantityText, NewValue); }
void UDualNodeInventorySlotViewModel::SetStackCount(int32 NewValue) { UE_MVVM_SET_PROPERTY_VALUE(StackCount, NewValue); }
void UDualNodeInventorySlotViewModel::SetRarityColor(FColor NewValue) { UE_MVVM_SET_PROPERTY_VALUE(RarityColor, NewValue); }
void UDualNodeInventorySlotViewModel::SetIsUsable(bool bNewValue) { UE_MVVM_SET_PROPERTY_VALUE(bIsUsable, bNewValue); }