#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "DualNodeItemInstance.h"
#include "DualNodeInventoryTypes.h"
#include "DualNodeInventoryValidator.h"
#include "DualNodeInventoryComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnInventoryUpdated, UDualNodeInventoryComponent*, Component);

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class DUALNODEINVENTORY_API UDualNodeInventoryComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UDualNodeInventoryComponent();

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	/** Prüft rein logisch, ob ein Item hinzugefügt werden könnte (Wichtig für UI) */
	UFUNCTION(BlueprintPure, Category="Inventory")
	bool CanAddItem(const UDualNodeItemDefinition* ItemDef, int32 Amount, FText& OutFailureReason) const;

	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category="Inventory")
	bool TryAddItem(const UDualNodeItemDefinition* ItemDef, int32 Amount = 1);

	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category="Inventory")
	bool RemoveItem(const UDualNodeItemDefinition* ItemDef, int32 Amount = 1);

	UFUNCTION(BlueprintPure, Category="Inventory")
	float GetTotalWeight() const;

	UFUNCTION(BlueprintPure, Category="Inventory")
	int32 GetTotalAmountOfItem(const UDualNodeItemDefinition* ItemDef) const;

	int32 GetTotalAmountOfItemById(FPrimaryAssetId ItemId) const;

	UFUNCTION(BlueprintCallable, Category="Inventory|Persistence")
	FDualNodeInventorySaveData GetInventorySnapshot() const;

	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category="Inventory|Persistence")
	void LoadInventoryFromSnapshot(const FDualNodeInventorySaveData& Snapshot);

	const TArray<FDualNodeItemInstance>& GetItems() const { return InventoryArray.Items; }

protected:
	UPROPERTY(ReplicatedUsing=OnRep_Inventory)
	FDualNodeInventoryArray InventoryArray;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Inventory|Config")
	int32 MaxSlotCount = 20;

	UPROPERTY(EditAnywhere, Instanced, Category="Inventory|Config")
	TArray<TObjectPtr<UDualNodeInventoryValidator>> Validators;

	UFUNCTION()
	void OnRep_Inventory();

	UPROPERTY(BlueprintAssignable, Category="Inventory")
	FOnInventoryUpdated OnInventoryUpdated;

private:
	int32 FindStackableSlot(const UDualNodeItemDefinition* ItemDef) const;
	int32 FindFreeSlot() const;
};