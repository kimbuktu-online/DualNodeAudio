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

	virtual void BeginPlay() override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	UPROPERTY(BlueprintAssignable, Category="Inventory")
	FOnInventoryUpdated OnInventoryUpdated;
	
	UPROPERTY(EditAnywhere, Instanced, Category="Inventory|Config")
	TArray<TObjectPtr<UDualNodeInventoryValidator>> Validators;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Inventory|Config")
	int32 MaxSlotCount = 20;

	UFUNCTION(BlueprintPure, Category="Inventory")
	bool CanAddItem(const UDualNodeItemDefinition* ItemDef, int32 Amount, FText& OutFailureReason) const;

	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category="Inventory")
	bool TryAddItem(const UDualNodeItemDefinition* ItemDef, int32 Amount = 1);

	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category="Inventory")
	bool RemoveItem(const UDualNodeItemDefinition* ItemDef, int32 Amount = 1);

	// --- SLOT INTERAKTIONEN (SERVER) ---

	UFUNCTION(Server, Reliable, BlueprintCallable, Category="Inventory")
	void Server_SwapSlots(int32 FromIndex, int32 ToIndex);

	UFUNCTION(Server, Reliable, BlueprintCallable, Category="Inventory")
	void Server_DropFromSlot(int32 SlotIndex, int32 Amount);

	UFUNCTION(Server, Reliable, BlueprintCallable, Category="Inventory")
	void Server_UseItemInSlot(int32 SlotIndex);

	/** Verschiebt eine Menge zwischen Slots oder Inventaren (für Quick Move & Split) */
	UFUNCTION(Server, Reliable, BlueprintCallable, Category="Inventory")
	void Server_TransferQuantity(int32 FromIndex, int32 ToIndex, int32 Quantity, UDualNodeInventoryComponent* TargetInventory = nullptr);

	UFUNCTION(BlueprintPure, Category="Inventory")
	float GetTotalWeight() const;

	UFUNCTION(BlueprintPure, Category="Inventory")
	int32 GetTotalAmountOfItem(const UDualNodeItemDefinition* ItemDef) const;

	UFUNCTION(BlueprintPure, Category="Inventory")
	int32 GetTotalAmountOfItemById(FPrimaryAssetId ItemId) const;

	UFUNCTION(BlueprintCallable, Category="Inventory|Persistence")
	FDualNodeInventorySaveData GetInventorySnapshot() const;

	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category="Inventory|Persistence")
	void LoadInventoryFromSnapshot(const FDualNodeInventorySaveData& Snapshot);

	UFUNCTION(BlueprintPure, Category = "Inventory")
	const TArray<FDualNodeItemInstance>& GetItems() const { return InventoryArray.Items; }

	UFUNCTION(BlueprintCallable, Category = "Inventory")
	void OnRep_Inventory();

protected:
	UPROPERTY(ReplicatedUsing=OnRep_Inventory)
	FDualNodeInventoryArray InventoryArray;

private:
	int32 FindStackableSlot(const UDualNodeItemDefinition* ItemDef) const;
	int32 FindFirstEmptySlot() const;
};