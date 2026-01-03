#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "DualNodeItemInstance.h"
#include "DualNodeInventoryComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnInventoryUpdated, UDualNodeInventoryComponent*, Component);

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class DUALNODEINVENTORY_API UDualNodeInventoryComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UDualNodeInventoryComponent();

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category="Inventory")
	bool TryAddItem(const UDualNodeItemDefinition* ItemDef, int32 Amount = 1); // Implementierung des Add-Flows [cite: 19]

	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category="Inventory")
	bool RemoveItem(const UDualNodeItemDefinition* ItemDef, int32 Amount = 1); // [cite: 24, 25]

protected:
	UPROPERTY(ReplicatedUsing=OnRep_Inventory)
	FDualNodeInventoryArray InventoryArray;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Inventory")
	int32 MaxSlotCount = 20; // [cite: 18]

	UFUNCTION()
	void OnRep_Inventory();

	UPROPERTY(BlueprintAssignable, Category="Inventory")
	FOnInventoryUpdated OnInventoryUpdated;

private:
	int32 FindStackableSlot(const UDualNodeItemDefinition* ItemDef) const; // [cite: 20]
	int32 FindFreeSlot() const; // [cite: 23]
};