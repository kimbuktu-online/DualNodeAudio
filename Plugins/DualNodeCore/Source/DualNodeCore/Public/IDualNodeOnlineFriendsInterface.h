// Copyright dual node interactive. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
// Moved .generated.h include here
#include "IDualNodeOnlineFriendsInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI, NotBlueprintable) // Added NotBlueprintable
class UDualNodeOnlineFriendsInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class DUALNODECORE_API IDualNodeOnlineFriendsInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	/**
	 * Retrieves the medium-sized avatar for a given UniqueNetId.
	 * @param UniqueNetId The unique network ID of the player.
	 * @return A UTexture2D representing the avatar, or nullptr if not available.
	 */
	UFUNCTION(BlueprintCallable, Category = "DualNode|OnlineFriends")
	virtual UTexture2D* GetMediumFriendAvatarForUniqueNetId(const FUniqueNetIdRepl& UniqueNetId) const = 0;

	/**
	 * Retrieves the persona name for a given UniqueNetId.
	 * @param UniqueNetId The unique network ID of the player.
	 * @return An FString representing the persona name, or empty string if not available.
	 */
	UFUNCTION(BlueprintCallable, Category = "DualNode|OnlineFriends")
	virtual FString GetFriendPersonaNameForUniqueNetId(const FUniqueNetIdRepl& UniqueNetId) const = 0;
};
