// Copyright dual node interactive. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "DualNodeCoreTypes.generated.h"

UENUM(BlueprintType)
enum class EGamePhase : uint8
{
	None,
	MainMenu,
	Lobby,
	PreGameplay,
	Gameplay,
	PostGameplay
};

UENUM(BlueprintType)
enum class ELobbyStartMode : uint8
{
	// The game starts after a countdown once all connected players are ready in a dedicated lobby map.
	PlayerReady,

	// The game starts after a countdown once the host manually initiates it in a dedicated lobby map.
	HostInitiatedTimer,

	// Players can form a party in the main menu. The party leader then starts matchmaking.
	MainMenuParty
};
