// Copyright dual node interactive. All Rights Reserved.

#include "DualNodeUIBlueprintLibrary.h"
#include "GameFramework/PlayerState.h"
#include "Engine/Texture2D.h"
#include "DualNodeCore/Public/DualNodeSteamFriendsSubsystem.h" // Include the new subsystem
#include "DualNodeCore/Public/IDualNodeOnlineFriendsInterface.h" // Include the new interface

UTexture2D* UDualNodeUIBlueprintLibrary::GetSteamAvatar(APlayerState* PlayerState)
{
	if (!PlayerState || !PlayerState->GetUniqueId().IsValid())
	{
		return nullptr;
	}

	// Get the DualNodeSteamFriendsSubsystem from the GameInstance
	if (UGameInstance* GameInstance = PlayerState->GetGameInstance())
	{
		// Use the interface to get the avatar, casting the subsystem to the interface type
		if (IDualNodeOnlineFriendsInterface* OnlineFriendsInterface = GameInstance->GetSubsystem<UDualNodeSteamFriendsSubsystem>())
		{
			return OnlineFriendsInterface->GetMediumFriendAvatarForUniqueNetId(PlayerState->GetUniqueId());
		}
	}

	return nullptr;
}
