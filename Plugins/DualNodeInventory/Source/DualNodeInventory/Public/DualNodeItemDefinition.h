#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "GameplayTagContainer.h"
#include "DualNodeItemFragment.h"
#include "DualNodeItemDefinition.generated.h"

UCLASS(BlueprintType, Const)
class DUALNODEINVENTORY_API UDualNodeItemDefinition : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Identity")
	FText ItemName; // [cite: 6]

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Identity", meta=(MultiLine=true))
	FText ItemDescription; // [cite: 6]

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Display")
	TObjectPtr<UTexture2D> ItemIcon; // [cite: 6]

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Config")
	bool bCanStack = true; // [cite: 7]

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Config", meta=(EditCondition="bCanStack"))
	int32 MaxStackSize = 99; // [cite: 7]

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Inventory")
	FGameplayTagContainer ItemTags;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Fragments", Instanced)
	TArray<TObjectPtr<UDualNodeItemFragment>> Fragments; // Modularer Ansatz 

	// Hilfsfunktion zum Finden von Fragmenten
	UFUNCTION(BlueprintCallable, Category="Inventory")
	const UDualNodeItemFragment* FindFragmentByClass(TSubclassOf<UDualNodeItemFragment> FragmentClass) const;

	virtual FPrimaryAssetId GetPrimaryAssetId() const override 
	{ 
		return FPrimaryAssetId("Item", GetFName()); // Saubere Asset Manager Integration [cite: 13, 14]
	}
};