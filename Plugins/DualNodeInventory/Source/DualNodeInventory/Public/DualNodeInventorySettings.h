#pragma once

#include "CoreMinimal.h"
#include "Engine/DeveloperSettings.h"
#include "GameplayTagContainer.h" // UNBEDINGT ERFORDERLICH
#include "DualNodeInventorySettings.generated.h"

UCLASS(Config=Game, DefaultConfig, meta=(DisplayName="Dual Node Inventory"))
class DUALNODEINVENTORY_API UDualNodeInventorySettings : public UDeveloperSettings
{
	GENERATED_BODY()

public:
	UDualNodeInventorySettings();

	UPROPERTY(Config, EditAnywhere, Category="General")
	int32 DefaultMaxStackSize = 99;

	UPROPERTY(Config, EditAnywhere, Category="General")
	bool bEnableVerboseLogging = false;

	UPROPERTY(Config, EditAnywhere, Category="Networking")
	float ClientPredictionTimeout = 0.5f;

	UPROPERTY(Config, EditAnywhere, Category="Audio")
	FGameplayTag DefaultPickupTag;
};