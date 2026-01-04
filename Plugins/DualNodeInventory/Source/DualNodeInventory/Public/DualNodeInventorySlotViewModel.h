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
	void UpdateSlot(const FDualNodeItemInstance& ItemInstance, int32 InSlotIndex);

	UPROPERTY(BlueprintReadOnly, FieldNotify, Category = "Display")
	TObjectPtr<UTexture2D> ItemIcon;

	UPROPERTY(BlueprintReadOnly, FieldNotify, Category = "Display")
	FText ItemName;

	UPROPERTY(BlueprintReadOnly, FieldNotify, Category = "Display")
	FText QuantityText;

	UPROPERTY(BlueprintReadOnly, FieldNotify, Category = "Display")
	int32 StackCount = 0;

	UPROPERTY(BlueprintReadOnly, FieldNotify, Category = "Display")
	FColor RarityColor;

	UPROPERTY(BlueprintReadOnly, FieldNotify, Category = "Logic")
	bool bIsUsable = false;

	/** NEU: Markiert diesen Slot als Quelle eines laufenden Transfers (für visuelles Feedback) */
	UPROPERTY(BlueprintReadOnly, FieldNotify, Category = "Logic")
	bool bIsSourceOfTransfer = false;

	UPROPERTY(BlueprintReadOnly, Category = "Logic")
	int32 SlotIndex = -1;

	UPROPERTY(BlueprintReadOnly, FieldNotify, Category = "Logic")
	FGuid ItemGuid;

protected:
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