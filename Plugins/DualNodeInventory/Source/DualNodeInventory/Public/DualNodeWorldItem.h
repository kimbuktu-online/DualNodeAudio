#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DualNodeItemInstance.h"
#include "DualNodeWorldItem.generated.h"

UCLASS()
class DUALNODEINVENTORY_API ADualNodeWorldItem : public AActor
{
	GENERATED_BODY()

public:
	ADualNodeWorldItem();

	/** Initialisiert den Actor mit einer Item-Definition */
	UFUNCTION(BlueprintCallable, Category = "DualNode|Inventory")
	void InitializeItem(const class UDualNodeItemDefinition* InDefinition, int32 InAmount = 1);

	/** Versucht, das Item in das Inventar eines anderen Actors zu legen */
	UFUNCTION(BlueprintCallable, Category = "DualNode|Inventory")
	bool PickUp(AActor* Interactor);

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<USceneComponent> Root;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<UStaticMeshComponent> MeshComponent;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Config", Replicated)
	FPrimaryAssetId ItemToGive;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Config", Replicated)
	int32 Amount = 1;

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
};