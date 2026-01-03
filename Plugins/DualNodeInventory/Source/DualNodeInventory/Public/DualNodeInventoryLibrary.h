#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "DualNodeItemDefinition.h"
#include "DualNodeInventoryLibrary.generated.h"

UCLASS()
class DUALNODEINVENTORY_API UDualNodeInventoryLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	/** Holt die Inventar-Komponente sicher über das Interface oder direkten Cast */
	UFUNCTION(BlueprintCallable, Category="DualNode|Inventory")
	static class UDualNodeInventoryComponent* GetInventoryComponent(AActor* Target);

	UFUNCTION(BlueprintCallable, Category="DualNode|Inventory")
	static bool TransferItem(UDualNodeInventoryComponent* Source, UDualNodeInventoryComponent* Destination, const UDualNodeItemDefinition* Item, int32 Amount);

	/** Sortiert das Inventar (Server-Side) */
	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category="DualNode|Inventory")
	static void SortInventory(UDualNodeInventoryComponent* Inventory);

	/** Nutzt einen Gegenstand und führt dessen Aktionen aus */
	UFUNCTION(BlueprintCallable, Category = "DualNode|Inventory")
	static bool UseItem(AActor* User, const UDualNodeItemDefinition* ItemDef);

	UFUNCTION(BlueprintCallable, Category="DualNode|Audio")
	static void PlayItemSound(const UDualNodeItemDefinition* Item, FGameplayTag ActionTag, AActor* ContextActor);
};