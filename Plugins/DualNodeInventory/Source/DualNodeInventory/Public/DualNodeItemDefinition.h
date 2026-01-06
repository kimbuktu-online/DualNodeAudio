#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "GameplayTagContainer.h"
#include "DualNodeItemFragment.h"
#include "DualNodeInventoryTypes.h"
#include "DualNodeItemDefinition.generated.h"

class UTexture2D;

/**
 * DNA 2.2 - System-Definition für Inventar-Gegenstände.
 * Erweitert um asynchrones Asset-Loading und logische Abfragen via Gameplay Tags.
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

	/** V2.2: Soft Pointer verhindert "Hitches" durch asynchrones Streaming der Texturen. */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Display")
	TSoftObjectPtr<UTexture2D> ItemIcon;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Identity")
	EDualNodeMainCategory MainCategory = EDualNodeMainCategory::None;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Identity")
	TObjectPtr<UDualNodeRarityDefinition> Rarity;

	// --- V2.2 LOGIC ---
	
	/** Designer-Abfrage für komplexe Bedingungen (z.B. Level-Requirements). */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Logic")
	FGameplayTagQuery ApplicationRequirements;

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

	virtual FPrimaryAssetId GetPrimaryAssetId() const override 
	{ 
		return FPrimaryAssetId("Item", GetFName()); 
	}
};