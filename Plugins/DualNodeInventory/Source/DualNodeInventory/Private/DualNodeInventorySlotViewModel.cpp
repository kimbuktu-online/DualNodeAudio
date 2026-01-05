#include "DualNodeInventorySlotViewModel.h"
#include "DualNodeItemDefinition.h"
#include "DualNodeInventoryComponent.h"
#include "DualNodeItemFragment_Durability.h"
#include "DualNodeItemFragment_UseAction.h"
#include "Engine/AssetManager.h"

void UDualNodeInventorySlotViewModel::UpdateSlot(const FDualNodeItemInstance& ItemInstance, int32 InSlotIndex, UDualNodeInventoryComponent* InInventory)
{
	ItemGuid = ItemInstance.InstanceGuid;
	SlotIndex = InSlotIndex;

	if (ItemInstance.CachedDefinition && InInventory)
	{
		SetItemName(ItemInstance.CachedDefinition->ItemName);
		SetItemDescription(ItemInstance.CachedDefinition->ItemDescription);
		SetStackCount(ItemInstance.StackCount);

		FText NewQuantity = (ItemInstance.StackCount > 1) 
			? FText::Format(NSLOCTEXT("InventoryUI", "QuantityLabel", "x{0}"), FText::AsNumber(ItemInstance.StackCount)) 
			: FText::GetEmpty();
		SetQuantityText(NewQuantity);

		// --- V2.2 ASYNCHRONES ICON LOADING ---
		TSoftObjectPtr<UTexture2D> SoftIcon = ItemInstance.CachedDefinition->ItemIcon;
		
		// Alten Request abbrechen, um "Flackern" bei schnellen Updates zu vermeiden
		if (IconLoadHandle.IsValid())
		{
			IconLoadHandle->CancelHandle();
		}

		if (!SoftIcon.IsNull())
		{
			if (SoftIcon.IsValid())
			{
				// Icon ist bereits im Speicher
				SetItemIcon(SoftIcon.Get());
			}
			else
			{
				// Icon muss geladen werden
				IconLoadHandle = UAssetManager::GetStreamableManager().RequestAsyncLoad(
					SoftIcon.ToSoftObjectPath(),
					FStreamableDelegate::CreateUObject(this, &UDualNodeInventorySlotViewModel::OnIconLoaded, SoftIcon)
				);
			}
		}
		else
		{
			SetItemIcon(nullptr);
		}

		// Durability
		float Pct = InInventory->GetItemDurabilityPercent(SlotIndex);
		SetDurabilityPercent(Pct);

		const UDualNodeItemFragment* DurFrag = ItemInstance.CachedDefinition->FindFragmentByClass(UDualNodeItemFragment_Durability::StaticClass());
		SetShowDurability(DurFrag != nullptr);

		if (const UDualNodeItemFragment_Durability* CastDur = Cast<UDualNodeItemFragment_Durability>(DurFrag))
		{
			if (CastDur->DurabilityType == EDualNodeDurabilityType::TimeBased)
			{
				int32 RemainingSeconds = FMath::Max(0, FMath::RoundToInt((ItemInstance.ExpirationTimestamp - InInventory->GetWorld()->GetTimeSeconds())));
				SetDurabilityCountdownText(FText::Format(NSLOCTEXT("InventoryUI", "ExpiresIn", "{0}s"), FText::AsNumber(RemainingSeconds)));
			}
		}

		SetIsUsable(ItemInstance.CachedDefinition->FindFragmentByClass(UDualNodeItemFragment_UseAction::StaticClass()) != nullptr);
		SetRarityColor(ItemInstance.CachedDefinition->Rarity ? ItemInstance.CachedDefinition->Rarity->RarityColor : FColor::White);
	}
	else
	{
		SetStackCount(0);
		SetItemIcon(nullptr);
		SetShowDurability(false);
		SetRarityColor(FColor(0,0,0,0));
		SetItemName(FText::GetEmpty());
		SetItemDescription(FText::GetEmpty());
	}
}

void UDualNodeInventorySlotViewModel::OnIconLoaded(TSoftObjectPtr<UTexture2D> LoadedIcon)
{
	if (LoadedIcon.IsValid())
	{
		SetItemIcon(LoadedIcon.Get());
	}
}

void UDualNodeInventorySlotViewModel::SetItemIcon(TObjectPtr<UTexture2D> NewValue) { UE_MVVM_SET_PROPERTY_VALUE(ItemIcon, NewValue); }
void UDualNodeInventorySlotViewModel::SetItemName(FText NewValue) { UE_MVVM_SET_PROPERTY_VALUE(ItemName, NewValue); }
void UDualNodeInventorySlotViewModel::SetItemDescription(FText NewValue) { UE_MVVM_SET_PROPERTY_VALUE(ItemDescription, NewValue); }
void UDualNodeInventorySlotViewModel::SetQuantityText(FText NewValue) { UE_MVVM_SET_PROPERTY_VALUE(QuantityText, NewValue); }
void UDualNodeInventorySlotViewModel::SetStackCount(int32 NewValue) { UE_MVVM_SET_PROPERTY_VALUE(StackCount, NewValue); }
void UDualNodeInventorySlotViewModel::SetRarityColor(FColor NewValue) { UE_MVVM_SET_PROPERTY_VALUE(RarityColor, NewValue); }
void UDualNodeInventorySlotViewModel::SetDurabilityPercent(float NewValue) { UE_MVVM_SET_PROPERTY_VALUE(DurabilityPercent, NewValue); }
void UDualNodeInventorySlotViewModel::SetDurabilityCountdownText(FText NewValue) { UE_MVVM_SET_PROPERTY_VALUE(DurabilityCountdownText, NewValue); }
void UDualNodeInventorySlotViewModel::SetShowDurability(bool bNewValue) { UE_MVVM_SET_PROPERTY_VALUE(bShowDurability, bNewValue); }
void UDualNodeInventorySlotViewModel::SetIsUsable(bool bNewValue) { UE_MVVM_SET_PROPERTY_VALUE(bIsUsable, bNewValue); }