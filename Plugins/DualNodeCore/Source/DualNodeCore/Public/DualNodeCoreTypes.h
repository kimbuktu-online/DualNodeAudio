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
