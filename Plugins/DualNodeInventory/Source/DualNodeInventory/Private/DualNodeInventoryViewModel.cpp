#include "DualNodeInventoryViewModel.h"
#include "DualNodeInventoryComponent.h"
#include "DualNodeInventoryValidator.h"
#include "DualNodeInventorySlotViewModel.h"

void UDualNodeInventoryViewModel::UpdateFromInventory(UDualNodeInventoryComponent* Inventory)
{
	if (!Inventory) return;

	float CurrentWeight = Inventory->GetTotalWeight();
	float MaxWeight = 100.0f; 
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

	TArray<TObjectPtr<UDualNodeInventorySlotViewModel>> NewSlotModels;
	const TArray<FDualNodeItemInstance>& Slots = Inventory->GetItems();

	for (int32 i = 0; i < Slots.Num(); i++)
	{
		UDualNodeInventorySlotViewModel* SlotVM = NewObject<UDualNodeInventorySlotViewModel>(this);
		// FIX C2660: Übergabe von 'Inventory' für Haltbarkeits-Checks
		SlotVM->UpdateSlot(Slots[i], i, Inventory);
		NewSlotModels.Add(SlotVM);
	}
	SetSlotViewModels(NewSlotModels);
}

void UDualNodeInventoryViewModel::SetDisplayWeight(FText NewValue) { UE_MVVM_SET_PROPERTY_VALUE(DisplayWeight, NewValue); }
void UDualNodeInventoryViewModel::SetWeightPercent(float NewValue) { UE_MVVM_SET_PROPERTY_VALUE(WeightPercent, NewValue); }
void UDualNodeInventoryViewModel::SetSlotViewModels(TArray<TObjectPtr<UDualNodeInventorySlotViewModel>> NewValue) { UE_MVVM_SET_PROPERTY_VALUE(SlotViewModels, NewValue); }