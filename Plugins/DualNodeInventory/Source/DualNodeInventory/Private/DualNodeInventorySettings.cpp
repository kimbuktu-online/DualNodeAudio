#include "DualNodeInventorySettings.h"

UDualNodeInventorySettings::UDualNodeInventorySettings()
{
	CategoryName = TEXT("Plugins");
	SectionName = TEXT("Dual Node Inventory");

	// FIX: Verhindert Error beim Engine-Start, falls die Tags noch nicht in der .ini stehen
	DefaultPickupTag = FGameplayTag::RequestGameplayTag(TEXT("Event.Inventory.Pickup"), false);
}