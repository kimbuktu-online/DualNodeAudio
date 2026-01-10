// Copyright dual node interactive. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "OnlineSubsystemTypes.h"
#include "DualNodeCoreInterfaces.generated.h"

UINTERFACE(MinimalAPI, Blueprintable)
class UDualNodeDevConnectInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * The contract for a subsystem that can identify developers.
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
};
