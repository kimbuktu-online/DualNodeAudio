#pragma once

#include "CoreMinimal.h"
#include "DualNodeItemFragment.h"
#include "GameplayTagContainer.h"
#include "DualNodeItemFragment_Stats.generated.h"

/** Fragment zur Speicherung von Gameplay-Werten wie Schaden oder Haltbarkeit */
UCLASS()
class DUALNODEINVENTORY_API UDualNodeItemFragment_Stats : public UDualNodeItemFragment
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, Category = "Stats")
	TMap<FGameplayTag, float> ItemStats;
};