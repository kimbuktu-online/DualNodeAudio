// Copyright dual node interactive. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DeveloperSettings.h"
#include "DualNodeDevConnectSettings.generated.h"

/**
 * Settings for the DualNodeDevConnect plugin.
 * Configure whitelisted developer Steam IDs here.
 */
UCLASS(Config = Game, defaultconfig, meta = (DisplayName = "DualNode DevConnect"))
class DUALNODEDEVCONNECT_API UDualNodeDevConnectSettings : public UDeveloperSettings
{
	GENERATED_BODY()

public:
	UDualNodeDevConnectSettings();

	// A list of Steam IDs (in 64-bit format) that are considered developers.
	// These users will be able to use the DevConnect features.
	UPROPERTY(Config, EditAnywhere, BlueprintReadOnly, Category = "Developer Whitelist")
	TArray<FString> DeveloperSteamIDs;

	static const UDualNodeDevConnectSettings* Get();
};
