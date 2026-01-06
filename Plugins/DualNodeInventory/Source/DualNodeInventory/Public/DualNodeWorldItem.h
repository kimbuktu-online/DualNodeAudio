#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DualNodeItemInstance.h"
#include "DualNodeInteractionInterface.h" // Zwingend erforderlich
#include "DualNodeWorldItem.generated.h"

class UStaticMeshComponent;

/**
 * Der Master Pickup Actor für die Spielwelt.
 */
UCLASS()
class DUALNODEINVENTORY_API ADualNodeWorldItem : public AActor, public IDualNodeInteractionInterface // FIX: Vererbung hinzufügen
{
	GENERATED_BODY()

public:
	ADualNodeWorldItem();

	/** Initialisiert den Actor zur Laufzeit */
	UFUNCTION(BlueprintCallable, Category = "DualNode|Inventory")
	void InitializeItem(const class UDualNodeItemDefinition* InDefinition, int32 InAmount = 1);

	/** Versucht, den Inhalt in das Inventar des Interactors zu übertragen */
	UFUNCTION(BlueprintCallable, Category = "DualNode|Inventory")
	bool PickUp(AActor* Interactor);

	// --- IDualNodeInteractionInterface Implementation ---
	virtual void Interact_Implementation(AActor* Interactor) override { PickUp(Interactor); }
	virtual bool CanInteract_Implementation(AActor* Interactor) const override { return true; }
	virtual FText GetInteractionText_Implementation() const override { return NSLOCTEXT("Interaction", "Pickup", "Aufnehmen"); }

protected:
	virtual void OnConstruction(const FTransform& Transform) override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<USceneComponent> Root;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<UStaticMeshComponent> MeshComponent;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Config", Replicated)
	FPrimaryAssetId ItemToGive;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Config", Replicated, meta=(ClampMin="1"))
	int32 Amount = 1;

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

private:
	void UpdateVisualsFromDefinition(const class UDualNodeItemDefinition* Definition);
};