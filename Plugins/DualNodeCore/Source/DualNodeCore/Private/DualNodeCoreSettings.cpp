// Copyright dual node interactive. All Rights Reserved.

#include "DualNodeCoreSettings.h"

UDualNodeCoreSettings::UDualNodeCoreSettings()
{
	// Set default maps here, they can be overridden in the project settings
	LobbyMap = TSoftObjectPtr<UWorld>(FSoftObjectPath(TEXT("/Game/Maps/LobbyMap.LobbyMap")));
	GameplayMap = TSoftObjectPtr<UWorld>(FSoftObjectPath(TEXT("/Game/Maps/GameplayMap.GameplayMap")));
}

const UDualNodeCoreSettings* UDualNodeCoreSettings::Get()
{
	return GetDefault<UDualNodeCoreSettings>();
}
