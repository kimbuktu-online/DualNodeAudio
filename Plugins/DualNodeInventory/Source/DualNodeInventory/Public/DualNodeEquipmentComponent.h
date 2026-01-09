#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GameplayTagContainer.h"
#include "DualNodeEquipmentComponent.generated.h"

class UDualNodeItemDefinition;
class USkeletalMeshComponent;
class UStaticMeshComponent;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnEquipmentChanged, FGameplayTag, SlotTag, const UDualNodeItemDefinition*, Item);

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class DUALNODEINVENTORY_API UDualNodeEquipmentComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UDualNodeEquipmentComponent();

	/** Rüstet ein Item visuell aus (wird meist vom Inventar getriggert) */
	UFUNCTION(BlueprintCallable, Category = "Equipment")
	void EquipItem(const UDualNodeItemDefinition* ItemDef);

	/** Entfernt die Visuals für einen bestimmten Slot-Tag */
	UFUNCTION(BlueprintCallable, Category = "Equipment")
	void UnequipSlot(FGameplayTag SlotTag);

	/** Gibt das Item-Asset zurück, das in einem bestimmten Slot ausgerüstet ist. */
	UFUNCTION(BlueprintPure, Category = "Equipment")
	const UDualNodeItemDefinition* GetEquippedItemForSlot(FGameplayTag SlotTag) const;

	/** Wird ausgelöst, wenn sich die Ausrüstung in einem Slot ändert (Item hinzugefügt oder entfernt). */
	UPROPERTY(BlueprintAssignable, Category = "Equipment")
	FOnEquipmentChanged OnEquipmentChanged;

protected:
	/** Speichert das Item-Asset pro Slot. Dies ist die "Quelle der Wahrheit". */
	UPROPERTY()
	TMap<FGameplayTag, TObjectPtr<const UDualNodeItemDefinition>> EquippedItems;

	/** Speichert gespawnte Skeletal Components (für Leader-Pose) */
	UPROPERTY()
	TMap<FGameplayTag, TObjectPtr<USkeletalMeshComponent>> EquippedSkeletalMeshes;

	/** Speichert gespawnte Static Components (für Waffen etc.) */
	UPROPERTY()
	TMap<FGameplayTag, TObjectPtr<UStaticMeshComponent>> EquippedStaticMeshes;

private:
	USkeletalMeshComponent* GetParentMesh() const;
};