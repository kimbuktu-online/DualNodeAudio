#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "DualNodeInventoryTypes.generated.h"

UENUM(BlueprintType)
enum class EDualNodeMainCategory : uint8
{
	None,
	Resources,
	Equipment,
	Consumables,
	QuestItems
};

UENUM(BlueprintType)
enum class EDualNodeResourceSubCategory : uint8
{
	None,
	Wood,
	Ore,
	Fiber
};

UCLASS(BlueprintType)
class DUALNODEINVENTORY_API UDualNodeRarityDefinition : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Display")
	FText RarityName;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Display")
	FColor RarityColor = FColor::White;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Display")
	TObjectPtr<UTexture2D> RarityPattern;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Logic")
	int32 SortOrder = 0;
};