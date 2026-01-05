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
	
	/** V2.0: Bestimmt das Verhalten des Inventars (Classic vs. Spatial) */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Inventory|Config")
	EDualNodeInventoryType InventoryType = EDualNodeInventoryType::Classic;

	UPROPERTY(EditAnywhere, Instanced, Category="Inventory|Config")
	TArray<TObjectPtr<UDualNodeInventoryValidator>> Validators;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Inventory|Config")
	int32 MaxSlotCount = 20;

	/** V2.0: Anzahl der Slots, die als HUD/Hotbar priorisiert werden (Standard: 0-4) */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Inventory|Config")
	int32 HUDSlotCount = 5;

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

	UFUNCTION(Server, Reliable, BlueprintCallable, Category="Inventory")
	void Server_TransferQuantity(int32 FromIndex, int32 ToIndex, int32 Quantity, UDualNodeInventoryComponent* TargetInventory = nullptr);

	// --- V2.0 HELPER ---

	/** Berechnet die verbleibende Haltbarkeit eines Items in Prozent (0.0 - 1.0) */
	UFUNCTION(BlueprintPure, Category="Inventory|Durability")
	float GetItemDurabilityPercent(int32 SlotIndex) const;

	UFUNCTION(BlueprintPure, Category="Inventory")
	float GetTotalWeight() const;

	UFUNCTION(BlueprintPure, Category="Inventory")
	int32 GetTotalAmountOfItem(const UDualNodeItemDefinition* ItemDef) const;

	UFUNCTION(BlueprintCallable, Category = "Inventory")
	void OnRep_Inventory();

protected:
	UPROPERTY(ReplicatedUsing=OnRep_Inventory)
	FDualNodeInventoryArray InventoryArray;

private:
	/** V2.0: Sucht Stackable Slots mit Priorität auf HUD-Bereich */
	int32 FindStackableSlot(const UDualNodeItemDefinition* ItemDef) const;
	
	/** V2.0: Sucht freien Slot mit Priorität auf HUD-Bereich */
	int32 FindFirstEmptySlot() const;

	/** V2.0: Initialisiert die Haltbarkeit für eine neue Instanz */
	void InitializeDurability(FDualNodeItemInstance& Instance, const UDualNodeItemDefinition* ItemDef);
};