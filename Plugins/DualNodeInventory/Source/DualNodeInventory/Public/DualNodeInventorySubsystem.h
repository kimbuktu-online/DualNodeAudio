#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "DualNodeInventorySubsystem.generated.h"

UCLASS()
class DUALNODEINVENTORY_API UDualNodeInventorySubsystem : public UWorldSubsystem
{
	GENERATED_BODY()

public:
	/** Findet alle Container in der Welt, die ein bestimmtes Item halten */
	UFUNCTION(BlueprintCallable, Category = "DualNode|Inventory")
	TArray<class UDualNodeInventoryComponent*> GetAllInventoriesWithItem(FPrimaryAssetId ItemId);
};