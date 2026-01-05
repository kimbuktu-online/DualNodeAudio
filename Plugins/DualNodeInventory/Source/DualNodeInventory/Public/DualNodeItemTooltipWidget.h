#pragma once

#include "CoreMinimal.h"
#include "CommonUserWidget.h"
#include "DualNodeItemTooltipWidget.generated.h"

/**
 * Basisklasse für das Hover-Popover (Version 2.0).
 */
UCLASS(Abstract, Blueprintable)
class DUALNODEINVENTORY_API UDualNodeItemTooltipWidget : public UCommonUserWidget
{
	GENERATED_BODY()

public:
	/** Bindet ein Slot-ViewModel an den Tooltip */
	UFUNCTION(BlueprintCallable, Category = "DualNode|Inventory|UI")
	void SetTooltipData(class UDualNodeInventorySlotViewModel* InSlotVM);

protected:
	UPROPERTY(BlueprintReadOnly, Category = "DualNode|Inventory|UI")
	TObjectPtr<class UDualNodeInventorySlotViewModel> SlotVM;

	/** Event für Blueprints zur visuellen Aktualisierung */
	UFUNCTION(BlueprintImplementableEvent, Category = "DualNode|Inventory|UI")
	void OnTooltipDataUpdated();
};