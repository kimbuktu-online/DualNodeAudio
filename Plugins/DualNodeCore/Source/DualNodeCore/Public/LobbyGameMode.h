// Copyright dual node interactive. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "DualNodeCoreGameMode.h"
#include "LobbyGameMode.generated.h"

/**
 * GameMode for the pre-game lobby.
 * Handles ready checks and starts the countdown to travel to the gameplay level.
 */
UCLASS()
class DUALNODECORE_API ALobbyGameMode : public ADualNodeCoreGameMode
{
	GENERATED_BODY()

public:
	ALobbyGameMode();

	virtual void PostLogin(APlayerController* NewPlayer) override;
	virtual void Logout(AController* Exiting) override;

	// Called from PlayerState when a player's ready status changes
	void OnPlayerReadyStatusChanged();

	// Called from PlayerController when a host requests to start the game
	void RequestStartGame(APlayerController* RequestingPlayer);

protected:
	void CheckPlayerReadyStatus();

	void StartGameCountdown();
	
	void UpdateCountdown();

	void CancelGameCountdown();

	void StartGame();

	UPROPERTY(EditDefaultsOnly, Category = "DualNodeCore|Lobby")
	int32 MinPlayersToStart;

	UPROPERTY(EditDefaultsOnly, Category = "DualNodeCore|Lobby")
	int32 CountdownDuration;

	FTimerHandle CountdownTimerHandle;
};
