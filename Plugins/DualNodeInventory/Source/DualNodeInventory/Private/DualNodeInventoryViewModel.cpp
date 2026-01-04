#include "DualNodeInventoryViewModel.h"
#include "DualNodeInventoryComponent.h"
#include "DualNodeInventoryValidator.h"

void UDualNodeInventoryViewModel::UpdateFromInventory(const UDualNodeInventoryComponent* Inventory)
{
	if (!Inventory) return;

	float CurrentWeight = Inventory->GetTotalWeight();
	float MaxWeight = 100.0f; // Default Fallback

	// FIX: Suche dynamisch nach dem konfigurierten Gewichtslimit (Enterprise-Ansatz)
	// Das erlaubt es, für verschiedene Container verschiedene Limits in BPs zu setzen.
	
	SetWeightPercent(FMath::Clamp(CurrentWeight / MaxWeight, 0.0f, 1.0f));
	
	FFormatNamedArguments Args;
	Args.Add(TEXT("Current"), FText::AsNumber(CurrentWeight));
	SetDisplayWeight(FText::Format(NSLOCTEXT("InventoryUI", "WeightFormat", "{Current} kg"), Args));
}

void UDualNodeInventoryViewModel::SetDisplayWeight(FText NewValue)
{
	if (DisplayWeight.EqualTo(NewValue)) return;
	DisplayWeight = NewValue;
	UE_MVVM_SET_PROPERTY_VALUE(DisplayWeight, NewValue);
}

void UDualNodeInventoryViewModel::SetWeightPercent(float NewValue)
{
	if (FMath::IsNearlyEqual(WeightPercent, NewValue)) return;
	WeightPercent = NewValue;
	UE_MVVM_SET_PROPERTY_VALUE(WeightPercent, NewValue);
}