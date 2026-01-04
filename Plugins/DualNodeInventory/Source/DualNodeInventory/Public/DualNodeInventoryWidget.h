#pragma once

#include "CoreMinimal.h"
#include "CommonUserWidget.h"
#include "CommonTileView.h"
#include "DualNodeInventoryViewModel.h"
#include "FieldNotificationId.h"
#include "DualNodeInventoryWidget.generated.h"

UCLASS(Abstract, Blueprintable)
class DUALNODEINVENTORY_API UDualNodeInventoryWidget : public UCommonUserWidget
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = "DualNode|Inventory")
	void InitializeInventory(class UDualNodeInventoryComponent* InventoryComponent);

	/** Wird vom Slot aufgerufen, wenn er angeklickt wird */
	void HandleSlotClick(UDualNodeInventorySlotViewModel* ClickedSlot);

protected:
	// --- INPUT OVERRIDES ---
	virtual FReply NativeOnMouseWheel(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget), Category = "DualNode|Inventory")
	TObjectPtr<UCommonTileView> InventoryTileView;

	UPROPERTY(BlueprintReadOnly, Category = "DualNode|Inventory")
	TObjectPtr<UDualNodeInventoryViewModel> MainViewModel;

	// --- HELD ITEM LOGIC ---
	
	/** Das aktuell "aufgehobene" Item-ViewModel */
	UPROPERTY(BlueprintReadOnly, Category = "DualNode|Inventory")
	TObjectPtr<UDualNodeInventorySlotViewModel> HeldItemVM;

	/** Die Menge, die man gerade am Cursor hält */
	UPROPERTY(BlueprintReadOnly, Category = "DualNode|Inventory")
	int32 TransferAmount = 0;

	/** Blueprint-Klasse für das visuelle Held-Item Widget */
	UPROPERTY(EditAnywhere, Category = "DualNode|Inventory|Config")
	TSubclassOf<UUserWidget> HeldItemVisualClass;

	UPROPERTY()
	TObjectPtr<UUserWidget> HeldItemVisualInstance;

	/** Hilfsfunktion: Aktualisiert das visuelle Feedback am Cursor */
	UFUNCTION(BlueprintImplementableEvent, Category = "DualNode|Inventory")
	void OnHeldAmountChanged(int32 NewAmount);

private:
	UFUNCTION()
	void HandleSlotViewModelsChanged();
	void HandleSlotViewModelsChanged_Internal(UObject* Source, UE::FieldNotification::FFieldId FieldId);

	UPROPERTY()
	TObjectPtr<class UDualNodeInventoryComponent> CachedComponent;
};