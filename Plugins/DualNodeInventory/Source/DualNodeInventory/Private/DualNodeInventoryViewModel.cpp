#include "DualNodeInventoryViewModel.h"
#include "DualNodeInventoryComponent.h"

void UDualNodeInventoryViewModel::UpdateFromInventory(UDualNodeInventoryComponent* Inventory)
{
	if (!Inventory) return;

	// Aktuelles Gewicht abrufen
	float CurrentWeight = Inventory->GetTotalWeight();
	
	// Standardwert festlegen, falls kein Validator gefunden wird
	float MaxWeight = 100.0f; 

	// Dynamisch nach dem Weight-Validator suchen, um das echte Limit zu finden
	// Wir greifen auf die Validators der InventoryComponent zu
	for (auto Validator : Inventory->Validators)
	{
		if (UDualNodeValidator_Weight* WeightValidator = Cast<UDualNodeValidator_Weight>(Validator))
		{
			MaxWeight = WeightValidator->MaxWeight;
			break;
		}
	}
	
	// Prozentwert korrekt berechnen (0.0 bis 1.0)
	SetWeightPercent(FMath::Clamp(CurrentWeight / MaxWeight, 0.0f, 1.0f));
	
	// Textanzeige aktualisieren (z.B. "10 kg")
	FFormatNamedArguments Args;
	Args.Add(TEXT("Current"), FText::AsNumber(CurrentWeight));
	SetDisplayWeight(FText::Format(NSLOCTEXT("InventoryUI", "WeightFormat", "{Current} kg"), Args));

	// Slots wie gehabt aktualisieren
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