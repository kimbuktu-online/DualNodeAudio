#include "DualNodeInventorySubsystem.h"
#include "DualNodeInventoryComponent.h"
#include "EngineUtils.h"

TArray<UDualNodeInventoryComponent*> UDualNodeInventorySubsystem::GetAllInventoriesWithItem(FPrimaryAssetId ItemId)
{
	TArray<UDualNodeInventoryComponent*> FoundInventories;
	for (TObjectIterator<UDualNodeInventoryComponent> It; It; ++It)
	{
		// Sicherstellen, dass die Komponente zur aktuellen Welt gehört
		if (It->GetWorld() == GetWorld())
		{
			if (It->GetTotalAmountOfItemById(ItemId) > 0)
			{
				FoundInventories.Add(*It);
			}
		}
	}
	return FoundInventories;
}