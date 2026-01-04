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
	/** FIX: BlueprintCallable hinzugefügt */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Interaction")
	void Interact(AActor* Interactor);

	/** FIX: BlueprintCallable hinzugefügt */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Interaction")
	FText GetInteractionText() const;

	/** FIX: BlueprintCallable hinzugefügt */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Interaction")
	bool CanInteract(AActor* Interactor) const;
};