#include "DualNodeInventorySettings.h"

UDualNodeInventorySettings::UDualNodeInventorySettings()
{
	CategoryName = TEXT("Plugins");
	SectionName = TEXT("Dual Node Inventory");
    
	DefaultPickupTag = FGameplayTag::RequestGameplayTag(TEXT("Event.Inventory.Pickup"));
}