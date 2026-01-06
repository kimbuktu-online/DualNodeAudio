#pragma once

#include "CoreMinimal.h"
#include "MVVMViewModelBase.h"
#include "DualNodeItemInstance.h"
#include "Engine/StreamableManager.h"
#include "DualNodeInventorySlotViewModel.generated.h"

class UTexture2D;

/**
 * DNA 2.2 - Slot ViewModel mit Unterstützung für asynchrones Icon-Loading.
 */
UCLASS(BlueprintType)
class DUALNODEINVENTORY_API UDualNodeInventorySlotViewModel : public UMVVMViewModelBase
{
	GENERATED_BODY()

public:
	/** Aktualisiert die Slot-Daten und stößt bei Bedarf das asynchrone Laden des Icons an. */
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

private:
	/** Handle für den laufenden Ladevorgang (Vermeidet doppeltes Laden) */
	TSharedPtr<FStreamableHandle> IconLoadHandle;
	
	/** Callback für den AssetManager */
	void OnIconLoaded(TSoftObjectPtr<UTexture2D> LoadedIcon);

protected:
	void SetItemIcon(TObjectPtr<UTexture2D> NewValue);
	void SetItemName(FText NewValue);
	void SetItemDescription(FText NewValue);
	void SetQuantityText(FText NewValue);
	void SetStackCount(int32 NewValue);
	void SetRarityColor(FColor NewValue);
	void SetDurabilityPercent(float NewValue);
	void SetDurabilityCountdownText(FText NewValue);
	void SetShowDurability(bool bNewValue);
	void SetIsUsable(bool bNewValue);

public:
	UFUNCTION(BlueprintCallable, Category = "DualNode|Inventory")
	void SetIsSourceOfTransfer(bool bNewValue) { UE_MVVM_SET_PROPERTY_VALUE(bIsSourceOfTransfer, bNewValue); }
};