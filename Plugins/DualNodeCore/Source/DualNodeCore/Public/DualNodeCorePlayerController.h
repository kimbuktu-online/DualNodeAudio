// Copyright dual node interactive. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "DualNodeCorePlayerController.generated.h"

/**
 * Base PlayerController for DualNode projects.
 * Provides hooks for game-specific player actions.
 */
UCLASS()
class DUALNODECORE_API ADualNodeCorePlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	/**
	 * [Client] Called by a UI button to request the server to start the game countdown.
	 * This is only respected by the server if the LobbyStartMode is HostInitiatedTimer.
	 */
	UFUNCTION(BlueprintCallable, Category = "DualNodeCore|Lobby")
	void RequestStartGame();

	/**
	 * [Server] The RPC function that tells the server to attempt starting the game.
	 */
	UFUNCTION(Server, Reliable)
	void Server_RequestStartGame();
};
