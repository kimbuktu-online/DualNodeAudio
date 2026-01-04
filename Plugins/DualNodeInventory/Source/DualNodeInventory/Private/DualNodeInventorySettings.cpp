#include "DualNodeInventorySettings.h"

UDualNodeInventorySettings::UDualNodeInventorySettings()
{
	CategoryName = TEXT("Plugins");
	SectionName = TEXT("Dual Node Inventory");
    
	// FIX: Das "false" verhindert den Absturz/Error, wenn der Tag (noch) fehlt
	DefaultPickupTag = FGameplayTag::RequestGameplayTag(TEXT("Event.Inventory.Pickup"), false);
}