#pragma once

#include "CoreMinimal.h"
#include "CommonUserWidget.h"
#include "CommonTileView.h"
#include "DualNodeInventoryViewModel.h"
#include "DualNodeInventoryWidget.generated.h"

UCLASS(Abstract, Blueprintable)
class DUALNODEINVENTORY_API UDualNodeInventoryWidget : public UCommonUserWidget
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = "DualNode|Inventory")
	void InitializeInventory(class UDualNodeInventoryComponent* InventoryComponent);

	/** Öffnet das Kontextmenü exakt an der Cursorposition */
	UFUNCTION(BlueprintCallable, Category = "DualNode|Inventory")
	void OpenContextMenu(UDualNodeInventorySlotViewModel* SlotVM, FVector2D ScreenPosition);

	void HandleSlotClick(UDualNodeInventorySlotViewModel* ClickedSlot);
	
	void ShowTooltip(class UDualNodeInventorySlotViewModel* SlotVM, const FGeometry& SlotGeometry);
	void HideTooltip();

protected:
	virtual FReply NativeOnMouseWheel(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

	UPROPERTY(EditAnywhere, Category = "DualNode|Inventory|Config")
	TSubclassOf<class UDualNodeItemTooltipWidget> TooltipClass;

	UPROPERTY()
	TObjectPtr<UDualNodeItemTooltipWidget> ActiveTooltip;
	
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget), Category = "DualNode|Inventory")
	TObjectPtr<UCommonTileView> InventoryTileView;

	UPROPERTY(BlueprintReadOnly, Category = "DualNode|Inventory")
	TObjectPtr<UDualNodeInventoryViewModel> MainViewModel;

	// --- CONTEXT MENU CONFIG ---
	
	UPROPERTY(EditAnywhere, Category = "DualNode|Inventory|Config")
	TSubclassOf<class UDualNodeContextMenu> ContextMenuClass;

	UPROPERTY()
	TObjectPtr<class UDualNodeContextMenu> ActiveContextMenu;

	// --- HELD ITEM LOGIC ---
	
	UPROPERTY(BlueprintReadOnly, Category = "DualNode|Inventory")
	TObjectPtr<UDualNodeInventorySlotViewModel> HeldItemVM;

	UPROPERTY(BlueprintReadOnly, Category = "DualNode|Inventory")
	int32 TransferAmount = 0;

	UPROPERTY(EditAnywhere, Category = "DualNode|Inventory|Config")
	TSubclassOf<UUserWidget> HeldItemVisualClass;

	UPROPERTY()
	TObjectPtr<UUserWidget> HeldItemVisualInstance;

	UFUNCTION(BlueprintImplementableEvent, Category = "DualNode|Inventory")
	void OnHeldAmountChanged(int32 NewAmount);

private:
	UFUNCTION()
	void HandleSlotViewModelsChanged();
	void HandleSlotViewModelsChanged_Internal(UObject* Source, UE::FieldNotification::FFieldId FieldId);

	UPROPERTY()
	TObjectPtr<class UDualNodeInventoryComponent> CachedComponent;
};