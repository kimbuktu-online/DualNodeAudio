// Copyright dual node interactive. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "DualNodeCoreGameMode.h"
#include "MainMenuGameMode.generated.h"

/**
 * A very simple GameMode for the Main Menu.
 * Its primary purpose is to allow players to join the host's session for a party
 * and assign the correct PlayerController.
 */
UCLASS()
class DUALNODECORE_API AMainMenuGameMode : public ADualNodeCoreGameMode
{
	GENERATED_BODY()

public:
	AMainMenuGameMode();
};
