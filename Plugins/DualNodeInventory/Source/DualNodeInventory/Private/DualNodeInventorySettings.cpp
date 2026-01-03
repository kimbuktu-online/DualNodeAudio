#include "DualNodeInventorySettings.h"

UDualNodeInventorySettings::UDualNodeInventorySettings()
{
	CategoryName = TEXT("Plugins");
	SectionName = TEXT("Dual Node Inventory");
    
	// Standard-Tags für die Enterprise Edition [cite: 26, 27]
	DefaultPickupTag = FGameplayTag::RequestGameplayTag(TEXT("Event.Inventory.Pickup"));
}