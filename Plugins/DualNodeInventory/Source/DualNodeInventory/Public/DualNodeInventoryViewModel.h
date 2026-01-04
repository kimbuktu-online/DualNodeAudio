#pragma once

#include "CoreMinimal.h"
#include "MVVMViewModelBase.h"
#include "DualNodeInventorySlotViewModel.h"
#include "DualNodeInventoryViewModel.generated.h"

UCLASS(BlueprintType)
class DUALNODEINVENTORY_API UDualNodeInventoryViewModel : public UMVVMViewModelBase
{
	GENERATED_BODY()

public:
	/** Signatur angepasst: Kein const, um mit Delegate-Binding kompatibel zu sein */
	UFUNCTION(BlueprintCallable, Category = "DualNode|UI")
	void UpdateFromInventory(class UDualNodeInventoryComponent* Inventory);

	UPROPERTY(BlueprintReadOnly, FieldNotify, Category = "DualNode|UI")
	TArray<TObjectPtr<UDualNodeInventorySlotViewModel>> SlotViewModels;

	UPROPERTY(BlueprintReadOnly, FieldNotify, Category = "DualNode|UI")
	FText DisplayWeight;

	UPROPERTY(BlueprintReadOnly, FieldNotify, Category = "DualNode|UI")
	float WeightPercent = 0.0f;

protected:
	void SetDisplayWeight(FText NewValue);
	void SetWeightPercent(float NewValue);
	void SetSlotViewModels(TArray<TObjectPtr<UDualNodeInventorySlotViewModel>> NewValue);
};