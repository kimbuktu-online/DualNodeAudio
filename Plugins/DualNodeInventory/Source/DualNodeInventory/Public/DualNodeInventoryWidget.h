#pragma once

#include "CoreMinimal.h"
#include "CommonUserWidget.h"
#include "CommonTileView.h"
#include "DualNodeInventoryViewModel.h"
#include "FieldNotificationId.h"
#include "DualNodeInventoryWidget.generated.h"

/**
 * C++ Basis für das Haupt-Inventar-Menü.
 */
UCLASS(Abstract, Blueprintable)
class DUALNODEINVENTORY_API UDualNodeInventoryWidget : public UCommonUserWidget
{
	GENERATED_BODY()

public:
	/** Bindet ein Inventar an dieses UI. */
	UFUNCTION(BlueprintCallable, Category = "DualNode|Inventory")
	void InitializeInventory(class UDualNodeInventoryComponent* InventoryComponent);

protected:
	/** Referenz auf die TileView im Widget-Blueprint. */
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget), Category = "DualNode|Inventory")
	TObjectPtr<UCommonTileView> InventoryTileView;

	/** Das ViewModel für das gesamte Inventar. */
	UPROPERTY(BlueprintReadOnly, Category = "DualNode|Inventory")
	TObjectPtr<UDualNodeInventoryViewModel> MainViewModel;

private:
	/** Aktualisiert die TileView. */
	UFUNCTION()
	void HandleSlotViewModelsChanged();

	/** Interner Callback für das MVVM-System. */
	void HandleSlotViewModelsChanged_Internal(UObject* Source, UE::FieldNotification::FFieldId FieldId);

	UPROPERTY()
	TObjectPtr<class UDualNodeInventoryComponent> CachedComponent;
};