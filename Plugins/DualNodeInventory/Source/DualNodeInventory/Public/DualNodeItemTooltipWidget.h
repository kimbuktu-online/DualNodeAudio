#pragma once

#include "CoreMinimal.h"
#include "CommonUserWidget.h"
#include "DualNodeItemTooltipWidget.generated.h"

UCLASS(Abstract, Blueprintable)
class DUALNODEINVENTORY_API UDualNodeItemTooltipWidget : public UCommonUserWidget
{
	GENERATED_BODY()

public:
	/** Bindet ein Slot-ViewModel an den Tooltip */
	UFUNCTION(BlueprintCallable, Category = "Inventory|UI")
	void SetTooltipData(class UDualNodeInventorySlotViewModel* InSlotVM);

protected:
	UPROPERTY(BlueprintReadOnly, Category = "Inventory|UI")
	TObjectPtr<class UDualNodeInventorySlotViewModel> SlotVM;

	/** Event für Blueprints, um das Design zu aktualisieren */
	UFUNCTION(BlueprintImplementableEvent, Category = "Inventory|UI")
	void OnTooltipDataUpdated();
};