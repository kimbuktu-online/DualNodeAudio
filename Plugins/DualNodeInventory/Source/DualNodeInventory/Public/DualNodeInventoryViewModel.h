#pragma once

#include "CoreMinimal.h"
#include "MVVMViewModelBase.h"
#include "DualNodeInventoryViewModel.generated.h"

/**
 * ViewModel zur performanten Anbindung des Inventars an CommonUI via MVVM.
 * Triggert Updates nur bei tatsächlichen Datenänderungen.
 */
UCLASS(BlueprintType)
class DUALNODEINVENTORY_API UDualNodeInventoryViewModel : public UMVVMViewModelBase
{
	GENERATED_BODY()

public:
	/** Aktualisiert die UI-Daten basierend auf dem aktuellen Inventar-Zustand */
	UFUNCTION(BlueprintCallable, Category = "DualNode|UI")
	void UpdateFromInventory(const class UDualNodeInventoryComponent* Inventory);

	/** Das formatierte Gesamtgewicht (z.B. "12.5 / 100 kg") */
	UPROPERTY(BlueprintReadOnly, FieldNotify, Category = "DualNode|UI")
	FText DisplayWeight;

	/** Der prozentuale Füllstand für ProgressBar-Anzeigen (0.0 - 1.0) */
	UPROPERTY(BlueprintReadOnly, FieldNotify, Category = "DualNode|UI")
	float WeightPercent = 0.0f;

protected:
	// Helper zur Benachrichtigung des UI-Systems über Änderungen
	void SetDisplayWeight(FText NewValue);
	void SetWeightPercent(float NewValue);
};