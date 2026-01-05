#pragma once

#include "CoreMinimal.h"
#include "MVVMViewModelBase.h"
#include "DualNodeItemInstance.h"
#include "DualNodeInventorySlotViewModel.generated.h"

UCLASS(BlueprintType)
class DUALNODEINVENTORY_API UDualNodeInventorySlotViewModel : public UMVVMViewModelBase
{
	GENERATED_BODY()

public:
	/** V2.0: Benötigt nun die Komponente für die Haltbarkeits-Berechnung */
	void UpdateSlot(const FDualNodeItemInstance& ItemInstance, int32 InSlotIndex, class UDualNodeInventoryComponent* InInventory);

	UPROPERTY(BlueprintReadOnly, FieldNotify, Category = "Display")
	TObjectPtr<UTexture2D> ItemIcon;

	UPROPERTY(BlueprintReadOnly, FieldNotify, Category = "Display")
	FText ItemName;

	UPROPERTY(BlueprintReadOnly, FieldNotify, Category = "Display")
	FText ItemDescription;

	UPROPERTY(BlueprintReadOnly, FieldNotify, Category = "Display")
	FText QuantityText;

	UPROPERTY(BlueprintReadOnly, FieldNotify, Category = "Display")
	int32 StackCount = 0;

	UPROPERTY(BlueprintReadOnly, FieldNotify, Category = "Display")
	FColor RarityColor;

	// --- V2.0 DURABILITY DATA ---

	UPROPERTY(BlueprintReadOnly, FieldNotify, Category = "Display")
	float DurabilityPercent = 1.0f;

	UPROPERTY(BlueprintReadOnly, FieldNotify, Category = "Display")
	FText DurabilityCountdownText;

	UPROPERTY(BlueprintReadOnly, FieldNotify, Category = "Logic")
	bool bShowDurability = false;

	UPROPERTY(BlueprintReadOnly, FieldNotify, Category = "Logic")
	bool bIsUsable = false;

	UPROPERTY(BlueprintReadOnly, FieldNotify, Category = "Logic")
	bool bIsSourceOfTransfer = false;

	UPROPERTY(BlueprintReadOnly, Category = "Logic")
	int32 SlotIndex = -1;

	UPROPERTY(BlueprintReadOnly, FieldNotify, Category = "Logic")
	FGuid ItemGuid;

protected:
	void SetDurabilityPercent(float NewValue);
	void SetDurabilityCountdownText(FText NewValue);
	void SetShowDurability(bool bNewValue);
	void SetItemIcon(TObjectPtr<UTexture2D> NewValue);
	void SetItemName(FText NewValue);
	void SetQuantityText(FText NewValue);
	void SetStackCount(int32 NewValue);
	void SetRarityColor(FColor NewValue);
	void SetIsUsable(bool bNewValue);

public:
	/** Hilfsfunktion zum Setzen des Ghost-Status */
	UFUNCTION(BlueprintCallable, Category = "DualNode|Inventory")
	void SetIsSourceOfTransfer(bool bNewValue) { UE_MVVM_SET_PROPERTY_VALUE(bIsSourceOfTransfer, bNewValue); }
};