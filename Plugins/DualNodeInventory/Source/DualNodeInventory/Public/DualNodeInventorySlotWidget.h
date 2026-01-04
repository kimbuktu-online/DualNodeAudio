#pragma once

#include "CoreMinimal.h"
#include "CommonButtonBase.h"
#include "Blueprint/IUserObjectListEntry.h"
#include "DualNodeInventorySlotViewModel.h"
#include "DualNodeInventorySlotWidget.generated.h"

/**
 * C++ Basis für einen Inventar-Slot.
 */
UCLASS(Abstract, Blueprintable)
class DUALNODEINVENTORY_API UDualNodeInventorySlotWidget : public UCommonButtonBase, public IUserObjectListEntry
{
	GENERATED_BODY()

public:
	// --- IUserObjectListEntry Interface ---
	// FIX: Wir überschreiben die Native-Variante des Interfaces
	virtual void NativeOnListItemObjectSet(UObject* ListItemObject) override;

protected:
	UPROPERTY(BlueprintReadOnly, Category = "DualNode|Inventory")
	TObjectPtr<UDualNodeInventorySlotViewModel> SlotViewModel;

	/** Event für Blueprints, um z.B. Animationen zu triggern */
	UFUNCTION(BlueprintImplementableEvent, Category = "DualNode|Inventory")
	void OnSlotUpdated();
};