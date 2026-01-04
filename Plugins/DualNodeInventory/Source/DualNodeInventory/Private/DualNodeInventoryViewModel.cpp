#include "DualNodeInventoryViewModel.h"
#include "DualNodeInventoryComponent.h"
#include "DualNodeInventoryValidator.h"

void UDualNodeInventoryViewModel::UpdateFromInventory(UDualNodeInventoryComponent* Inventory)
{
	if (!Inventory) return;

	// Gewichtsberechnung (wie gehabt)
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

	// --- FIX FÜR FIXTE SLOT-ANZAHL ---
	TArray<TObjectPtr<UDualNodeInventorySlotViewModel>> NewSlotModels;
	const TArray<FDualNodeItemInstance>& CurrentItems = Inventory->GetItems();

	// 1. Vorhandene Items hinzufügen
	for (const FDualNodeItemInstance& Item : CurrentItems)
	{
		UDualNodeInventorySlotViewModel* SlotVM = NewObject<UDualNodeInventorySlotViewModel>(this);
		SlotVM->UpdateSlot(Item);
		NewSlotModels.Add(SlotVM);
	}

	// 2. Mit leeren Slots auffüllen bis MaxSlotCount erreicht ist
	while (NewSlotModels.Num() < Inventory->MaxSlotCount)
	{
		UDualNodeInventorySlotViewModel* EmptySlotVM = NewObject<UDualNodeInventorySlotViewModel>(this);
		FDualNodeItemInstance EmptyInstance; // Erstellt eine Instanz ohne Definition
		EmptySlotVM->UpdateSlot(EmptyInstance);
		NewSlotModels.Add(EmptySlotVM);
	}

	SetSlotViewModels(NewSlotModels);
}

void UDualNodeInventoryViewModel::SetDisplayWeight(FText NewValue) { UE_MVVM_SET_PROPERTY_VALUE(DisplayWeight, NewValue); }
void UDualNodeInventoryViewModel::SetWeightPercent(float NewValue) { UE_MVVM_SET_PROPERTY_VALUE(WeightPercent, NewValue); }
void UDualNodeInventoryViewModel::SetSlotViewModels(TArray<TObjectPtr<UDualNodeInventorySlotViewModel>> NewValue) { UE_MVVM_SET_PROPERTY_VALUE(SlotViewModels, NewValue); }