#pragma once

#include "CoreMinimal.h"
#include "CommonUserWidget.h"
#include "DualNodeContextMenu.generated.h"

class UDualNodeInventorySlotViewModel;
class UDualNodeInventoryComponent;
class UDualNodeItemDefinition;

/**
 * Basisklasse für das Kontextmenü (WBP_ContextMenu).
 * Bietet sicheren Zugriff auf Inventar-Daten für Blueprints.
 */
UCLASS(Abstract, Blueprintable)
class DUALNODEINVENTORY_API UDualNodeContextMenu : public UCommonUserWidget
{
	GENERATED_BODY()

public:
	/** Initialisiert das Menü mit den Daten des Slots */
	UFUNCTION(BlueprintCallable, Category = "DualNode|Inventory")
	void InitializeMenu(UDualNodeInventorySlotViewModel* InSlotVM);

	/** Schließt das Menü sauber */
	UFUNCTION(BlueprintCallable, Category = "DualNode|Inventory")
	void CloseMenu();

	// --- SPRECHWERTIGE GETTER FÜR BLUEPRINTS ---

	/** * Gibt die Inventar-Komponente des Besitzers zurück. 
	 * Sicherer als der manuelle Aufruf der Library in BPs.
	 */
	UFUNCTION(BlueprintPure, Category = "DualNode|Inventory")
	UDualNodeInventoryComponent* GetOwningInventory() const;

	/** Gibt die Item-Definition des ausgewählten Slots zurück */
	UFUNCTION(BlueprintPure, Category = "DualNode|Inventory")
	const UDualNodeItemDefinition* GetTargetItemDefinition() const;

	// --- ACTIONS ---

	UFUNCTION(BlueprintCallable, Category = "DualNode|Inventory")
	void ExecuteUse();

	UFUNCTION(BlueprintCallable, Category = "DualNode|Inventory")
	void ExecuteDrop(int32 Amount);

	/** Fordert die Mengenauswahl im UI an */
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "DualNode|Inventory")
	void RequestAmountSelection(bool bIsForDrop);

protected:
	UPROPERTY(BlueprintReadOnly, Category = "DualNode|Inventory")
	TObjectPtr<UDualNodeInventorySlotViewModel> TargetSlotVM;

	virtual void NativeOnMouseLeave(const FPointerEvent& InMouseEvent) override;
};