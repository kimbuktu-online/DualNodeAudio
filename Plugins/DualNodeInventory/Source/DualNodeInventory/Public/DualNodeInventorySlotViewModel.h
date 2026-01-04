#pragma once

#include "CoreMinimal.h"
#include "MVVMViewModelBase.h"
#include "DualNodeItemInstance.h"
#include "DualNodeInventorySlotViewModel.generated.h"

/**
 * ViewModel für einen einzelnen Inventar-Slot.
 * Wird vom MVVM-System genutzt, um Daten an das Slot-Widget zu binden.
 */
UCLASS(BlueprintType)
class DUALNODEINVENTORY_API UDualNodeInventorySlotViewModel : public UMVVMViewModelBase
{
	GENERATED_BODY()

public:
	/** Aktualisiert die Daten aus einer Item-Instanz */
	void UpdateSlot(const FDualNodeItemInstance& ItemInstance);

	/** Das Icon des Gegenstands */
	UPROPERTY(BlueprintReadOnly, FieldNotify, Category = "Display")
	TObjectPtr<UTexture2D> ItemIcon;

	/** Der Name des Gegenstands */
	UPROPERTY(BlueprintReadOnly, FieldNotify, Category = "Display")
	FText ItemName;

	/** Die Menge als Text (z.B. "x5" oder leer bei 1) */
	UPROPERTY(BlueprintReadOnly, FieldNotify, Category = "Display")
	FText QuantityText;

	/** Die Farbe der Seltenheit für das UI-Styling */
	UPROPERTY(BlueprintReadOnly, FieldNotify, Category = "Display")
	FColor RarityColor;

	/** Die zugrunde liegende Instanz-GUID für Interaktionen */
	UPROPERTY(BlueprintReadOnly, FieldNotify, Category = "Logic")
	FGuid ItemGuid;

protected:
	void SetItemIcon(TObjectPtr<UTexture2D> NewValue);
	void SetItemName(FText NewValue);
	void SetQuantityText(FText NewValue);
	void SetRarityColor(FColor NewValue);
};