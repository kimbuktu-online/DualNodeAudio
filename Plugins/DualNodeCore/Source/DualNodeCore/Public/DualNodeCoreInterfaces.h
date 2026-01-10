// Copyright dual node interactive. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "OnlineSubsystemTypes.h"
#include "DualNodeCoreInterfaces.generated.h"

// Forward declare to avoid include loops
class APlayerController;

UINTERFACE(MinimalAPI, Blueprintable)
class UDualNodeDevConnectInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * The contract for a subsystem that can identify and handle developers.
 * DualNodeCore uses this to query for developer status without a hard dependency.
 */
class DUALNODECORE_API IDualNodeDevConnectInterface
{
	GENERATED_BODY()

public:
	/**
	 * Checks if the given UniqueNetId belongs to a whitelisted developer.
	 * @param UniqueId The NetId of the player trying to log in.
	 * @return True if the player is a developer, false otherwise.
	 */
	virtual bool IsDeveloper(const FUniqueNetIdRepl& UniqueId) const = 0;

	/**
	 * Handles the post-login logic for a developer, like spawning a spectator pawn.
	 * @param NewPlayer The PlayerController of the developer who just logged in.
	 * @return True if the login was handled (e.g., a pawn was spawned), false to allow default GameMode behavior.
	 */
	virtual bool OnDeveloperPostLogin(APlayerController* NewPlayer) = 0;
};
