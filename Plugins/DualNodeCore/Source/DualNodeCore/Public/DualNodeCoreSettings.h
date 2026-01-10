// Copyright dual node interactive. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DeveloperSettings.h"
#include "DualNodeCoreSettings.generated.h"

/**
 * Project-wide settings for the DualNodeCore plugin.
 * Configure maps and other core properties here in Project Settings > Plugins > DualNode Core.
 */
UCLASS(Config = Game, defaultconfig, meta = (DisplayName = "DualNode Core"))
class DUALNODECORE_API UDualNodeCoreSettings : public UDeveloperSettings
{
	GENERATED_BODY()

public:
	UDualNodeCoreSettings();

	// The map to travel to when a session is hosted or joined.
	UPROPERTY(Config, EditAnywhere, BlueprintReadOnly, Category = "Maps")
	TSoftObjectPtr<UWorld> LobbyMap;

	// The map to travel to when the lobby countdown finishes.
	UPROPERTY(Config, EditAnywhere, BlueprintReadOnly, Category = "Maps")
	TSoftObjectPtr<UWorld> GameplayMap;

	// Helper function to get the settings instance
	static const UDualNodeCoreSettings* Get();
};
