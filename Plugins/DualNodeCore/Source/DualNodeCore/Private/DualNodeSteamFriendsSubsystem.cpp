// Copyright dual node interactive. All Rights Reserved.

#include "DualNodeSteamFriendsSubsystem.h"
#include "SteamFriends/SteamFriends.h"
#include "SteamFriends/SteamFriendsTypes.h"
#include "Misc/CString.h"
#include "IDualNodeOnlineFriendsInterface.h" // Ensure this is included for the interface

void UDualNodeSteamFriendsSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
	// Any initialization logic for the subsystem
}

void UDualNodeSteamFriendsSubsystem::Deinitialize()
{
	// Any deinitialization logic for the subsystem
	Super::Deinitialize();
}

UTexture2D* UDualNodeSteamFriendsSubsystem::GetMediumFriendAvatarForUniqueNetId(const FUniqueNetIdRepl& UniqueNetId) const
{
	if (!UniqueNetId.IsValid() || !UniqueNetId.GetUniqueNetId().IsValid())
	{
		return nullptr;
	}

	if (const TSharedPtr<const FUniqueNetId> UniqueNetIdPtr = UniqueNetId.GetUniqueNetId())
	{
		if (USteamProFriends* SteamFriends = USteamProFriends::GetSteamFriends())
		{
			uint64 SteamID_UInt64 = FCString::Strtoui64(*UniqueNetIdPtr->ToString(), nullptr, 10);
			FSteamID SteamId(SteamID_UInt64);
			return SteamFriends->GetMediumFriendAvatar(SteamId);
		}
	}

	return nullptr;
}

FString UDualNodeSteamFriendsSubsystem::GetFriendPersonaNameForUniqueNetId(const FUniqueNetIdRepl& UniqueNetId) const
{
	if (!UniqueNetId.IsValid() || !UniqueNetId.GetUniqueNetId().IsValid())
	{
		return FString();
	}

	if (const TSharedPtr<const FUniqueNetId> UniqueNetIdPtr = UniqueNetId.GetUniqueNetId())
	{
		if (USteamProFriends* SteamFriends = USteamProFriends::GetSteamFriends())
		{
			uint64 SteamID_UInt64 = FCString::Strtoui64(*UniqueNetIdPtr->ToString(), nullptr, 10);
			FSteamID SteamId(SteamID_UInt64);
			return SteamFriends->GetFriendPersonaName(SteamId);
		}
	}

	return FString();
}
