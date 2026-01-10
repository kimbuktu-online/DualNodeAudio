// Copyright dual node interactive. All Rights Reserved.

#include "MainMenuGameMode.h"
#include "DualNodeCorePlayerController.h"
#include "DualNodeCorePlayerState.h"

AMainMenuGameMode::AMainMenuGameMode()
{
	PlayerControllerClass = ADualNodeCorePlayerController::StaticClass();
	PlayerStateClass = ADualNodeCorePlayerState::StaticClass();
	
	// No pawn in the main menu
	DefaultPawnClass = nullptr;
}
