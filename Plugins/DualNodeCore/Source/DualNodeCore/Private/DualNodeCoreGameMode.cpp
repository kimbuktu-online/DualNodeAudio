// Copyright dual node interactive. All Rights Reserved.

#include "DualNodeCoreGameMode.h"
#include "DualNodeCore.h"
#include "DualNodeCoreGameState.h"
#include "DualNodeCorePlayerState.h"
#include "DualNodeCoreInterfaces.h"
#include "GameFramework/GameStateBase.h"
#include "GameFramework/PlayerState.h"

ADualNodeCoreGameMode::ADualNodeCoreGameMode()
{
	GameStateClass = ADualNodeCoreGameState::StaticClass();
	PlayerStateClass = ADualNodeCorePlayerState::StaticClass();

	PublicMaxPlayers = 8;
	ReservedDevSlots = 2;
}

void ADualNodeCoreGameMode::PreLogin(const FString& Options, const FString& Address, const FUniqueNetIdRepl& UniqueId, FString& ErrorMessage)
{
	Super::PreLogin(Options, Address, UniqueId, ErrorMessage);

	const int32 TotalMaxPlayers = PublicMaxPlayers + ReservedDevSlots;
	if (GameState->PlayerArray.Num() >= TotalMaxPlayers)
	{
		ErrorMessage = TEXT("Server is completely full.");
		return;
	}

	bool bIsDeveloper = false;
	if (IDualNodeDevConnectInterface* DevConnect = FDualNodeCoreModule::GetDevConnectInterface())
	{
		if (DevConnect->IsDeveloper(UniqueId))
		{
			bIsDeveloper = true;
		}
	}

	FString UniqueIdString = TEXT("Invalid");
	if (UniqueId.IsValid() && UniqueId.GetUniqueNetId().IsValid())
	{
		UniqueIdString = UniqueId.GetUniqueNetId()->ToString();
	}

	if (bIsDeveloper)
	{
		UE_LOG(LogTemp, Log, TEXT("PreLogin: Developer access granted for %s."), *UniqueIdString);
	}
	else
	{
		if (GameState->PlayerArray.Num() >= PublicMaxPlayers)
		{
			ErrorMessage = TEXT("Server is full for public players.");
			return;
		}
		UE_LOG(LogTemp, Log, TEXT("PreLogin: Public access granted for %s."), *UniqueIdString);
	}
}

void ADualNodeCoreGameMode::PostLogin(APlayerController* NewPlayer)
{
	// First, check if this is a developer and if the DevConnect plugin wants to handle the login.
	if (NewPlayer)
	{
		if (IDualNodeDevConnectInterface* DevConnect = FDualNodeCoreModule::GetDevConnectInterface())
		{
			if (DevConnect->IsDeveloper(*NewPlayer->PlayerState->GetUniqueId()))
			{
				// The DevConnect plugin will handle spawning the pawn (or not).
				if (DevConnect->OnDeveloperPostLogin(NewPlayer))
				{
					UE_LOG(LogTemp, Log, TEXT("PostLogin: Developer %s handled by DevConnect."), *NewPlayer->GetPlayerState<APlayerState>()->GetPlayerName());
					// If it was handled, we skip the default pawn spawning.
					return;
				}
			}
		}
	}
	
	// If not a developer, or if DevConnect didn't handle it, run the default login logic.
	Super::PostLogin(NewPlayer);
	UE_LOG(LogTemp, Log, TEXT("PostLogin: Player %s has joined normally."), *NewPlayer->GetPlayerState<APlayerState>()->GetPlayerName());
}
