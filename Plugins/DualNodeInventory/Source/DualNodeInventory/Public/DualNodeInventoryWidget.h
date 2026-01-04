#pragma once

#include "CoreMinimal.h"
#include "CommonUserWidget.h"
#include "CommonTileView.h"
#include "DualNodeInventoryViewModel.h"
#include "DualNodeInventoryWidget.generated.h"

/**
 * C++ Basis für das Haupt-Inventar-Menü.
 * Designer binden hier die TileView und das Haupt-ViewModel an.
 */
UCLASS(Abstract, Blueprintable)
class DUALNODEINVENTORY_API UDualNodeInventoryWidget : public UCommonUserWidget
{
	GENERATED_BODY()

public:
	/** * Bindet ein Inventar an dieses UI.
	 * Wird typischerweise vom PlayerController oder HUD aufgerufen.
	 */
	UFUNCTION(BlueprintCallable, Category = "DualNode|Inventory")
	void InitializeInventory(class UDualNodeInventoryComponent* InventoryComponent);

protected:
	/** * Referenz auf die TileView im Widget-Blueprint.
	 * meta=(BindWidget) erzwingt, dass im Blueprint eine TileView mit diesem Namen existiert.
	 */
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget), Category = "DualNode|Inventory")
	TObjectPtr<UCommonTileView> InventoryTileView;

	/** Das ViewModel für das gesamte Inventar (Gewicht, Slot-Liste) */
	UPROPERTY(BlueprintReadOnly, Category = "DualNode|Inventory")
	TObjectPtr<UDualNodeInventoryViewModel> MainViewModel;

private:
	UFUNCTION() // Muss UFUNCTION sein, um von Delegates/UHT erkannt zu werden
	void HandleSlotViewModelsChanged();

	UPROPERTY()
	TObjectPtr<class UDualNodeInventoryComponent> CachedComponent;
};