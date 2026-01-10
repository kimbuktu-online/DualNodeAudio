// Copyright dual node interactive. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "DualNodeCoreGameMode.generated.h"

/**
 * Base GameMode class for DualNode projects.
 * Handles login and player spawning logic.
 */
UCLASS()
class DUALNODECORE_API ADualNodeCoreGameMode : public AGameMode
{
	GENERATED_BODY()

public:
	ADualNodeCoreGameMode();

	/**
	 * Called before a player logs in. This is the perfect place to implement whitelist/blacklist logic
	 * and to check for developer access via DualNodeDevConnect.
	 */
	virtual void PreLogin(const FString& Options, const FString& Address, const FUniqueNetIdRepl& UniqueId, FString& ErrorMessage) override;

	/**
	 * Called after a player successfully logs in. Here we can decide which pawn to spawn
	 * (e.g., a normal player pawn or a spectator pawn for a developer).
	 */
	virtual void PostLogin(APlayerController* NewPlayer) override;

protected:
	UPROPERTY(EditDefaultsOnly, Category = "DualNodeCore|Slots")
	int32 PublicMaxPlayers;

	UPROPERTY(EditDefaultsOnly, Category = "DualNodeCore|Slots")
	int32 ReservedDevSlots;
};
