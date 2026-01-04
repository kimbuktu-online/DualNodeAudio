#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "DualNodeItemDefinition.h"
#include "DualNodeItemInstance.h"
#include "DualNodeInventoryLibrary.generated.h"

UCLASS()
class DUALNODEINVENTORY_API UDualNodeInventoryLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	/** Sucht sicher nach der Inventar-Komponente via Interface oder Cast */
	UFUNCTION(BlueprintCallable, Category="DualNode|Inventory")
	static class UDualNodeInventoryComponent* GetInventoryComponent(AActor* Target);

	/** Löst die statischen Daten (Node A) aus einer Instanz (Node B) auf */
	UFUNCTION(BlueprintCallable, Category = "DualNode|Inventory")
	static bool ResolveItemDefinition(UPARAM(ref) FDualNodeItemInstance& ItemInstance);

	/** Überträgt eine Menge eines Items zwischen Containern */
	UFUNCTION(BlueprintCallable, Category="DualNode|Inventory")
	static bool TransferItem(UDualNodeInventoryComponent* Source, UDualNodeInventoryComponent* Destination, const UDualNodeItemDefinition* Item, int32 Amount);

	/** Erzeugt einen World-Actor und entfernt das Item aus dem Inventar */
	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category = "DualNode|Inventory")
	static bool DropItem(AActor* Dropper, const UDualNodeItemDefinition* Item, int32 Amount);

	/** Sortiert das Inventar stabil nach Kategorie und Name */
	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category="DualNode|Inventory")
	static void SortInventory(UDualNodeInventoryComponent* Inventory);

	/** Nutzt einen Gegenstand und triggert dessen 'UseActions' */
	UFUNCTION(BlueprintCallable, Category = "DualNode|Inventory")
	static bool UseItem(AActor* User, const UDualNodeItemDefinition* ItemDef);

	UFUNCTION(BlueprintCallable, Category="DualNode|Audio")
	static void PlayItemSound(const UDualNodeItemDefinition* Item, FGameplayTag ActionTag, AActor* ContextActor);
};