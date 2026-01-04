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
	FColor RarityColor;

	UPROPERTY(BlueprintReadOnly, FieldNotify, Category = "Logic")
	bool bIsUsable = false;

	UPROPERTY(BlueprintReadOnly, Category = "Logic")
	int32 SlotIndex = -1;

	UPROPERTY(BlueprintReadOnly, FieldNotify, Category = "Logic")
	FGuid ItemGuid;

protected:
	void SetItemIcon(TObjectPtr<UTexture2D> NewValue);
	void SetItemName(FText NewValue);
	void SetQuantityText(FText NewValue);
	void SetRarityColor(FColor NewValue);
	void SetIsUsable(bool bNewValue);
};