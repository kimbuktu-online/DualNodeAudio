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
	/** Überprüft, ob ein Actor das DualNodeInterface besitzt und gibt die Komponente zurück */
	UFUNCTION(BlueprintCallable, Category="DualNode|Inventory", meta=(DefaultToSelf="Target"))
	static class UDualNodeInventoryComponent* GetInventoryComponent(AActor* Target);

	/** Hochoptimierte Transfer-Logik zwischen zwei Containern [cite: 42, 43] */
	UFUNCTION(BlueprintCallable, Category="DualNode|Inventory")
	static bool TransferItem(UDualNodeInventoryComponent* Source, UDualNodeInventoryComponent* Destination, const UDualNodeItemDefinition* Item, int32 Amount);

	/** Löst Audio-Events basierend auf Item-Fragmenten aus */
	UFUNCTION(BlueprintCallable, Category="DualNode|Audio")
	static void PlayItemSound(const UDualNodeItemDefinition* Item, FGameplayTag ActionTag, AActor* ContextActor);
};