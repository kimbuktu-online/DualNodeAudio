#include "DualNodeInventoryViewModel.h"
#include "DualNodeInventoryComponent.h"
#include "DualNodeInventoryValidator.h"
#include "DualNodeInventorySettings.h"
#include "DualNodeInventorySlotViewModel.h"

void UDualNodeInventoryViewModel::UpdateFromInventory(UDualNodeInventoryComponent* Inventory)
{
	if (!Inventory) return;

	float CurrentWeight = Inventory->GetTotalWeight();
	
	// DNA 2.2: Globaler Fallback aus Settings
	float MaxWeight = UDualNodeInventorySettings::Get()->GlobalDefaultMaxWeight; 
	
	for (auto Validator : Inventory->Validators)
	{
		if (UDualNodeValidator_Weight* WeightValidator = Cast<UDualNodeValidator_Weight>(Validator))
		{
			MaxWeight = WeightValidator->MaxWeight;
			break;
		}
	}
	
	SetWeightPercent(FMath::Clamp(CurrentWeight / MaxWeight, 0.0f, 1.0f));
	
	FFormatNamedArguments Args;
	Args.Add(TEXT("Current"), FText::AsNumber(CurrentWeight));
	SetDisplayWeight(FText::Format(NSLOCTEXT("InventoryUI", "WeightFormat", "{Current} kg"), Args));

	const TArray<FDualNodeItemInstance>& Slots = Inventory->GetItems();

	// Viewmodel Recycling (v2.2)
	if (SlotViewModels.Num() != Slots.Num())
	{
		TArray<TObjectPtr<UDualNodeInventorySlotViewModel>> NewList;
		for (int32 i = 0; i < Slots.Num(); i++)
		{
			if (SlotViewModels.IsValidIndex(i)) NewList.Add(SlotViewModels[i]);
			else NewList.Add(NewObject<UDualNodeInventorySlotViewModel>(this));
		}
		SetSlotViewModels(NewList);
	}

	for (int32 i = 0; i < Slots.Num(); i++)
	{
		SlotViewModels[i]->UpdateSlot(Slots[i], i, Inventory);
	}
}

void UDualNodeInventoryViewModel::SetDisplayWeight(FText NewValue) { UE_MVVM_SET_PROPERTY_VALUE(DisplayWeight, NewValue); }
void UDualNodeInventoryViewModel::SetWeightPercent(float NewValue) { UE_MVVM_SET_PROPERTY_VALUE(WeightPercent, NewValue); }
void UDualNodeInventoryViewModel::SetSlotViewModels(TArray<TObjectPtr<UDualNodeInventorySlotViewModel>> NewValue) { UE_MVVM_SET_PROPERTY_VALUE(SlotViewModels, NewValue); }