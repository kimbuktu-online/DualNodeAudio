#pragma once

#include "CoreMinimal.h"
#include "CommonButtonBase.h"
#include "Blueprint/IUserObjectListEntry.h"
#include "DualNodeInventorySlotViewModel.h"
#include "DualNodeInventorySlotWidget.generated.h"

UCLASS(Abstract, Blueprintable)
class DUALNODEINVENTORY_API UDualNodeInventorySlotWidget : public UCommonButtonBase, public IUserObjectListEntry
{
	GENERATED_BODY()

public:
	virtual void NativeOnListItemObjectSet(UObject* ListItemObject) override;

	virtual FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	virtual void NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent, UDragDropOperation*& OutOperation) override;
	virtual bool NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation) override;

protected:
	UPROPERTY(BlueprintReadOnly, Category = "DualNode|Inventory")
	TObjectPtr<UDualNodeInventorySlotViewModel> SlotViewModel;

	UFUNCTION(BlueprintImplementableEvent, Category = "DualNode|Inventory")
	void OnRightClicked();

	UFUNCTION(BlueprintImplementableEvent, Category = "DualNode|Inventory")
	void OnSlotUpdated();
};