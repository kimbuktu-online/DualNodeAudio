#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "DualNodeItemInstance.h"
#include "DualNodeInventoryTypes.h"
#include "DualNodeInventoryValidator.h"
#include "DualNodeInventoryComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnInventoryUpdated, UDualNodeInventoryComponent*, Component);

/**
 * DNA 2.2 - Core Inventory Component.
 * Fokus: Multiplayer-Sicherheit, Performance (Grid-Caching) und AAA-Konfiguration.
 */
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
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Inventory|Config")
	EDualNodeInventoryType InventoryType = EDualNodeInventoryType::Classic;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Inventory|Config", meta=(EditCondition="InventoryType == EDualNodeInventoryType::Spatial"))
	FIntPoint GridSize = FIntPoint(10, 10);

	UPROPERTY(EditAnywhere, Instanced, Category="Inventory|Config")
	TArray<TObjectPtr<UDualNodeInventoryValidator>> Validators;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Inventory|Config", meta=(ExposeOnSpawn=true))
	int32 MaxSlotCount = 20;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Inventory|Config")
	int32 HUDSlotCount = 5;

	/** Passt die Anzahl der Slots zur Laufzeit an (z.B. durch einen Rucksack). Nur auf dem Server! */
	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category="Inventory")
	void SetMaxSlotCount(int32 NewSlotCount);

	// --- LOGIK API ---

	UFUNCTION(BlueprintPure, Category="Inventory")
	bool CanAddItem(const UDualNodeItemDefinition* ItemDef, int32 Amount, FText& OutFailureReason) const;

	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category="Inventory")
	bool TryAddItem(const UDualNodeItemDefinition* ItemDef, int32 Amount = 1);

	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category="Inventory")
	bool RemoveItem(const UDualNodeItemDefinition* ItemDef, int32 Amount = 1);

	// --- V2.2 SECURITY: SERVER RPCs WITH VALIDATION ---

	UFUNCTION(Server, Reliable, WithValidation, BlueprintCallable, Category="Inventory")
	void Server_SwapSlots(int32 FromIndex, int32 ToIndex);

	UFUNCTION(Server, Reliable, WithValidation, BlueprintCallable, Category="Inventory")
	void Server_DropFromSlot(int32 SlotIndex, int32 Amount);

	UFUNCTION(Server, Reliable, WithValidation, BlueprintCallable, Category="Inventory")
	void Server_UseItemInSlot(int32 SlotIndex);

	UFUNCTION(Server, Reliable, WithValidation, BlueprintCallable, Category="Inventory")
	void Server_TransferQuantity(int32 FromIndex, int32 ToIndex, int32 Quantity, UDualNodeInventoryComponent* TargetInventory = nullptr);

	// --- V2.2 PERFORMANCE: SPATIAL GRID API ---

	UFUNCTION(BlueprintPure, Category="Inventory|Spatial")
	bool IsRegionFree(FIntPoint Location, FIntPoint Size, const FGuid& IgnoreInstance = FGuid()) const;

	UFUNCTION(BlueprintPure, Category="Inventory|Spatial")
	bool FindFirstFreeLocation(FIntPoint ItemSize, FIntPoint& OutLocation) const;

	void RebuildGridCache();

	// --- GETTERS & UTILS ---

	UFUNCTION(BlueprintPure, Category="Inventory|Durability")
	float GetItemDurabilityPercent(int32 SlotIndex) const;

	UFUNCTION(BlueprintPure, Category="Inventory")
	float GetTotalWeight() const;

	UFUNCTION(BlueprintPure, Category="Inventory")
	int32 GetTotalAmountOfItem(const UDualNodeItemDefinition* ItemDef) const;

	UFUNCTION(BlueprintPure, Category="Inventory")
	int32 GetTotalAmountOfItemById(FPrimaryAssetId ItemId) const;

	UFUNCTION(BlueprintPure, Category = "Inventory")
	const TArray<FDualNodeItemInstance>& GetItems() const { return InventoryArray.Items; }

	// --- PERSISTENCE ---

	UFUNCTION(BlueprintCallable, Category="Inventory|Persistence")
	FDualNodeInventorySaveData GetInventorySnapshot() const;

	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category="Inventory|Persistence")
	void LoadInventoryFromSnapshot(const FDualNodeInventorySaveData& Snapshot);

	UFUNCTION(BlueprintCallable, Category = "Inventory")
	void OnRep_Inventory();

protected:
	UPROPERTY(ReplicatedUsing=OnRep_Inventory)
	FDualNodeInventoryArray InventoryArray;

private:
	UPROPERTY(Transient)
	TMap<FIntPoint, FGuid> CachedOccupiedCells;
	
	bool bGridCacheDirty = true;

	int32 FindStackableSlot(const UDualNodeItemDefinition* ItemDef) const;
	int32 FindFirstEmptySlot() const;
	void InitializeDurability(FDualNodeItemInstance& Instance, const UDualNodeItemDefinition* ItemDef);
};