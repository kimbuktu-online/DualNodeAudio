#include "DualNodeInventoryValidator.h"
#include "DualNodeItemDefinition.h"

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