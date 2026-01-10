// Copyright dual node interactive. All Rights Reserved.

#include "DualNodeCorePlayerState.h"
#include "Net/UnrealNetwork.h"
#include "LobbyGameMode.h" // Needed to notify the GameMode
#include "DualNodeCore/Public/DualNodeOnlineFriendsProvider.h" // Updated include for the provider
#include "DualNodeCore/Public/IDualNodeOnlineFriendsInterface.h" // Include the new interface

ADualNodeCorePlayerState::ADualNodeCorePlayerState()
{
	bIsReady = false;
}

void ADualNodeCorePlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ADualNodeCorePlayerState, bIsReady);
	DOREPLIFETIME(ADualNodeCorePlayerState, PlayerSteamName);
}

void ADualNodeCorePlayerState::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	// On the server, when the PlayerState is initialized, get the Steam name.
	if (HasAuthority())
	{
		// Get the DualNodeOnlineFriendsProvider from the GameInstance and cast to the interface
		if (UGameInstance* GameInstance = GetGameInstance())
		{
			if (IDualNodeOnlineFriendsInterface* OnlineFriendsInterface = GameInstance->GetSubsystem<UDualNodeOnlineFriendsProvider>())
			{
				// Use the interface function to get the persona name
				PlayerSteamName = OnlineFriendsInterface->GetFriendPersonaNameForUniqueNetId(GetUniqueId());
				OnRep_PlayerSteamName(); // Call on server as well
			}
		}
	}
}

void ADualNodeCorePlayerState::Server_SetIsReady_Implementation(bool bReady)
{
	bIsReady = bReady;
	
	// Call OnRep on the server immediately
	OnRep_IsReady();

	// Notify the GameMode on the server that a player's ready status has changed
	if (GetWorld() && GetWorld()->GetAuthGameMode())
	{
		if (ALobbyGameMode* LobbyGM = Cast<ALobbyGameMode>(GetWorld()->GetAuthGameMode()))
		{
			LobbyGM->OnPlayerReadyStatusChanged();
		}
	}
}

void ADualNodeCorePlayerState::OnRep_IsReady()
{
	OnPlayerInfoChanged.Broadcast();
}

void ADualNodeCorePlayerState::OnRep_PlayerSteamName()
{
	OnPlayerInfoChanged.Broadcast();
}
