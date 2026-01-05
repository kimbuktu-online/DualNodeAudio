#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GameplayTagContainer.h"
#include "DualNodeEquipmentComponent.generated.h"

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class DUALNODEINVENTORY_API UDualNodeEquipmentComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UDualNodeEquipmentComponent();

	/** Rüstet ein Item visuell aus (wird meist vom Inventar getriggert) */
	UFUNCTION(BlueprintCallable, Category = "Equipment")
	void EquipItem(const class UDualNodeItemDefinition* ItemDef);

	/** Entfernt die Visuals für einen bestimmten Slot-Tag */
	UFUNCTION(BlueprintCallable, Category = "Equipment")
	void UnequipSlot(FGameplayTag SlotTag);

protected:
	/** Speichert gespawnte Skeletal Components (für Leader-Pose) */
	UPROPERTY()
	TMap<FGameplayTag, TObjectPtr<USkeletalMeshComponent>> EquippedSkeletalMeshes;

	/** Speichert gespawnte Static Components (für Waffen etc.) */
	UPROPERTY()
	TMap<FGameplayTag, TObjectPtr<UStaticMeshComponent>> EquippedStaticMeshes;

private:
	USkeletalMeshComponent* GetParentMesh() const;
};
