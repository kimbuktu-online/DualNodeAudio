#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "DualNodeInteractionComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnFocusedActorChanged, AActor*, NewFocusedActor);

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class DUALNODEINVENTORY_API UDualNodeInteractionComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UDualNodeInteractionComponent();

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	/** Führt die Interaktion mit dem aktuell fokussierten Objekt aus */
	UFUNCTION(BlueprintCallable, Category = "Interaction")
	void PerformInteraction();

	UPROPERTY(BlueprintAssignable, Category = "Interaction")
	FOnFocusedActorChanged OnFocusedActorChanged;

protected:
	UPROPERTY(EditAnywhere, Category = "Interaction|Config")
	float InteractionRange = 250.0f;

	UPROPERTY(EditAnywhere, Category = "Interaction|Config")
	TEnumAsByte<ECollisionChannel> InteractionChannel = ECC_Visibility;

private:
	void TraceForInteractables();

	UPROPERTY()
	TObjectPtr<AActor> CurrentFocusedActor;
};