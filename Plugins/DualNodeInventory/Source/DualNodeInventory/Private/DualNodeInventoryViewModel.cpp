#include "DualNodeInventoryViewModel.h"
#include "DualNodeInventoryComponent.h"

void UDualNodeInventoryViewModel::UpdateFromInventory(UDualNodeInventoryComponent* Inventory)
{
	if (!Inventory) return;

	// Gewicht berechnen
	float CurrentWeight = Inventory->GetTotalWeight();
	float MaxWeight = 100.0f; 
	
	SetWeightPercent(FMath::Clamp(CurrentWeight / MaxWeight, 0.0f, 1.0f));
	
	FFormatNamedArguments Args;
	Args.Add(TEXT("Current"), FText::AsNumber(CurrentWeight));
	SetDisplayWeight(FText::Format(NSLOCTEXT("InventoryUI", "WeightFormat", "{Current} kg"), Args));

	// Slots aktualisieren
	TArray<TObjectPtr<UDualNodeInventorySlotViewModel>> NewSlotModels;
	for (const FDualNodeItemInstance& Item : Inventory->GetItems())
	{
		UDualNodeInventorySlotViewModel* SlotVM = NewObject<UDualNodeInventorySlotViewModel>(this);
		SlotVM->UpdateSlot(Item);
		NewSlotModels.Add(SlotVM);
	}
	SetSlotViewModels(NewSlotModels);
}

void UDualNodeInventoryViewModel::SetDisplayWeight(FText NewValue) { UE_MVVM_SET_PROPERTY_VALUE(DisplayWeight, NewValue); }
void UDualNodeInventoryViewModel::SetWeightPercent(float NewValue) { UE_MVVM_SET_PROPERTY_VALUE(WeightPercent, NewValue); }
void UDualNodeInventoryViewModel::SetSlotViewModels(TArray<TObjectPtr<UDualNodeInventorySlotViewModel>> NewValue) { UE_MVVM_SET_PROPERTY_VALUE(SlotViewModels, NewValue); }