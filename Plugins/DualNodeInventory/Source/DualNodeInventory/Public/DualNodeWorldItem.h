#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DualNodeItemInstance.h"
#include "DualNodeWorldItem.generated.h"

/**
 * Der Master Pickup Actor für die Spielwelt.
 * Erlaubt das Platzieren von Items im Level mit automatischer visueller Vorschau.
 */
UCLASS()
class DUALNODEINVENTORY_API ADualNodeWorldItem : public AActor
{
	GENERATED_BODY()

public:
	ADualNodeWorldItem();

	/** Initialisiert den Actor zur Laufzeit (z.B. beim Droppen aus dem Inventar) */
	UFUNCTION(BlueprintCallable, Category = "DualNode|Inventory")
	void InitializeItem(const class UDualNodeItemDefinition* InDefinition, int32 InAmount = 1);

	/** Versucht, den Inhalt in das Inventar des Interactors zu übertragen */
	UFUNCTION(BlueprintCallable, Category = "DualNode|Inventory")
	bool PickUp(AActor* Interactor);

protected:
	/** Wird aufgerufen, wenn Variablen im Editor geändert werden (Vorschau-Logik) */
	virtual void OnConstruction(const FTransform& Transform) override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<USceneComponent> Root;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<UStaticMeshComponent> MeshComponent;

	/** Welches Item soll dieser Actor repräsentieren? (Einstellbar im Editor) */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Config", Replicated)
	FPrimaryAssetId ItemToGive;

	/** Wie viele Einheiten sind im Stack? */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Config", Replicated, meta=(ClampMin="1"))
	int32 Amount = 1;

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

private:
	/** Interne Hilfsfunktion zum Aktualisieren des Meshes basierend auf Item-Fragmenten */
	void UpdateVisualsFromDefinition(const class UDualNodeItemDefinition* Definition);
};