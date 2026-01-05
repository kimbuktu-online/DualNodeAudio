#include "DualNodeInventoryValidator.h"
#include "DualNodeInventoryComponent.h"
#include "DualNodeItemDefinition.h"
#include "DualNodeItemFragment_Spatial.h"

bool UDualNodeValidator_Category::CanAddItem_Implementation(const UDualNodeInventoryComponent* Inventory, const UDualNodeItemDefinition* Item, int32 Amount, FText& OutFailureMessage) const
{
	if (!Item) return false;
	
	if (AllowedCategories.Num() > 0 && !AllowedCategories.Contains(Item->MainCategory))
	{
		OutFailureMessage = NSLOCTEXT("Inventory", "InvalidCategory", "Gegenstand hat die falsche Kategorie für diesen Container.");
		return false;
	}
	return true;
}

bool UDualNodeValidator_Weight::CanAddItem_Implementation(const UDualNodeInventoryComponent* Inventory, const UDualNodeItemDefinition* Item, int32 Amount, FText& OutFailureMessage) const
{
	if (!Inventory || !Item) return false;

	const float NewWeight = Inventory->GetTotalWeight() + (Item->ItemWeight * (float)Amount);
	if (NewWeight > MaxWeight)
	{
		OutFailureMessage = NSLOCTEXT("Inventory", "TooHeavy", "Nicht genügend Tragekapazität verfügbar.");
		return false;
	}
	return true;
}

bool UDualNodeValidator_GridSpace::CanAddItem_Implementation(const UDualNodeInventoryComponent* Inventory, const UDualNodeItemDefinition* Item, int32 Amount, FText& OutFailureMessage) const
{
	if (!Inventory || !Item || Inventory->InventoryType != EDualNodeInventoryType::Spatial) return true;

	const UDualNodeItemFragment* Frag = Item->FindFragmentByClass(UDualNodeItemFragment_Spatial::StaticClass());
	FIntPoint Dimensions = Frag ? Cast<UDualNodeItemFragment_Spatial>(Frag)->Dimensions : FIntPoint(1, 1);

	FIntPoint DummyLoc;
	if (!Inventory->FindFirstFreeLocation(Dimensions, DummyLoc))
	{
		OutFailureMessage = NSLOCTEXT("Inventory", "NoGridSpace", "Nicht genügend Platz im Grid verfügbar.");
		return false;
	}

	return true;
}