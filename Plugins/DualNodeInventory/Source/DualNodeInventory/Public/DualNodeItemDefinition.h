#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "GameplayTagContainer.h"
#include "DualNodeItemFragment.h"
#include "DualNodeInventoryTypes.h"
#include "DualNodeItemDefinition.generated.h"

/** * Node A: Die statische Definition eines Items.
 * Hier liegen alle Daten, die sich zur Laufzeit nicht ändern[cite: 5, 13].
 */
UCLASS(BlueprintType, Const)
class DUALNODEINVENTORY_API UDualNodeItemDefinition : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Identity")
	FText ItemName;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Identity", meta = (MultiLine = true))
	FText ItemDescription;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Display")
	TObjectPtr<UTexture2D> ItemIcon;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Identity")
	EDualNodeMainCategory MainCategory = EDualNodeMainCategory::None;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Identity")
	TObjectPtr<UDualNodeRarityDefinition> Rarity;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Config")
	bool bCanStack = true;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Config", meta = (EditCondition = "bCanStack"))
	int32 MaxStackSize = 99;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Logic")
	float ItemWeight = 0.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Fragments", Instanced)
	TArray<TObjectPtr<UDualNodeItemFragment>> Fragments;

	UFUNCTION(BlueprintCallable, Category = "Inventory")
	const UDualNodeItemFragment* FindFragmentByClass(TSubclassOf<UDualNodeItemFragment> FragmentClass) const;

	/** Asset Manager Integration [cite: 13] */
	virtual FPrimaryAssetId GetPrimaryAssetId() const override 
	{ 
		return FPrimaryAssetId("Item", GetFName()); 
	}
};