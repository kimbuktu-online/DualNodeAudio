// Copyright dual node interactive. All Rights Reserved.

#include "DualNodeCoreGameMode.h"
#include "DualNodeCore.h"
#include "DualNodeCoreGameState.h"
#include "DualNodeCorePlayerState.h"
#include "DualNodeCoreInterfaces.h"
#include "GameFramework/GameStateBase.h"

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

	if (bIsDeveloper)
	{
		UE_LOG(LogTemp, Log, TEXT("PreLogin: Developer access granted for %s."), *UniqueId.ToString());
	}
	else
	{
		if (GameState->PlayerArray.Num() >= PublicMaxPlayers)
		{
			ErrorMessage = TEXT("Server is full for public players.");
			return;
		}
		UE_LOG(LogTemp, Log, TEXT("PreLogin: Public access granted for %s."), *UniqueId.ToString());
	}
}

void ADualNodeCoreGameMode::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);

	// TODO: Implement soft check for DualNodeDevConnect here to spawn a spectator pawn.
	// This will use the same interface-based approach.
	UE_LOG(LogTemp, Log, TEXT("PostLogin: Player %s has joined."), *NewPlayer->GetPlayerState<APlayerState>()->GetPlayerName());
}
