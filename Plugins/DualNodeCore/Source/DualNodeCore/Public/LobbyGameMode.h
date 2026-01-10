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

	virtual void Tick(float DeltaSeconds) override;

protected:
	void CheckIfAllPlayersAreReady();

	void StartGameCountdown();
	
	void StartGame();

	UPROPERTY(EditDefaultsOnly, Category = "DualNodeCore|Lobby")
	float CountdownDuration;

	FTimerHandle CountdownTimerHandle;
};
