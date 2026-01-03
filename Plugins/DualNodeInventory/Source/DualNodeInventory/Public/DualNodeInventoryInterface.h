#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "DualNodeInventoryInterface.generated.h"

UINTERFACE(MinimalAPI, Blueprintable)
class UDualNodeInventoryInterface : public UInterface
{
	GENERATED_BODY()
};

class DUALNODEINVENTORY_API IDualNodeInventoryInterface
{
	GENERATED_BODY()

public:
	/** Gibt die Inventar-Komponente des Actors zurück */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Inventory")
	class UDualNodeInventoryComponent* GetInventoryComponent() const;

	/** Prüft, ob ein Actor (z.B. Spieler) das Inventar aktuell nutzen darf */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Inventory")
	bool CanUseInventory(AActor* User) const;
};