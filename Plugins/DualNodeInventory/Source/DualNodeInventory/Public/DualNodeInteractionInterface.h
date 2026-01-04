#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "DualNodeInteractionInterface.generated.h"

UINTERFACE(MinimalAPI, Blueprintable)
class UDualNodeInteractionInterface : public UInterface
{
	GENERATED_BODY()
};

class DUALNODEINVENTORY_API IDualNodeInteractionInterface
{
	GENERATED_BODY()

public:
	/** Wird aufgerufen, wenn der Spieler die Interaktionstaste drückt */
	UFUNCTION(BlueprintNativeEvent, Category = "Interaction")
	void Interact(AActor* Interactor);

	/** Liefert Informationen für das UI (z.B. "E - Aufheben") */
	UFUNCTION(BlueprintNativeEvent, Category = "Interaction")
	FText GetInteractionText() const;

	/** Prüft, ob eine Interaktion aktuell möglich ist */
	UFUNCTION(BlueprintNativeEvent, Category = "Interaction")
	bool CanInteract(AActor* Interactor) const;
};