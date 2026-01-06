#include "DualNodeInventorySubsystem.h"
#include "DualNodeInventoryComponent.h"
#include "UObject/UObjectIterator.h"

TArray<UDualNodeInventoryComponent*> UDualNodeInventorySubsystem::GetAllInventoriesWithItem(FPrimaryAssetId ItemId)
{
	TArray<UDualNodeInventoryComponent*> FoundInventories;

	// Moderne und sicherere Methode, um über alle Instanzen einer Klasse zu iterieren.
	for (UDualNodeInventoryComponent* Inventory : TObjectRange<UDualNodeInventoryComponent>())
	{
		// Sicherstellen, dass die Komponente gültig ist und zur aktuellen Welt gehört
		if (Inventory && Inventory->GetWorld() == GetWorld())
		{
			if (Inventory->GetTotalAmountOfItemById(ItemId) > 0)
			{
				FoundInventories.Add(Inventory);
			}
		}
	}
	return FoundInventories;
}