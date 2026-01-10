// Copyright dual node interactive. All Rights Reserved.

#include "LobbyGameMode.h"
#include "DualNodeCoreGameState.h"
#include "DualNodeCorePlayerState.h"
#include "DualNodeCoreSettings.h"
#include "GameFramework/GameStateBase.h"
#include "Kismet/GameplayStatics.h"
#include "TimerManager.h"

ALobbyGameMode::ALobbyGameMode()
{
	PrimaryActorTick.bCanEverTick = false;

	MinPlayersToStart = 1; // Can be 1 for host-initiated mode
	CountdownDuration = 10;
}

void ALobbyGameMode::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);
	// In PlayerReady mode, a new player joining could change the state.
	if (const UDualNodeCoreSettings* Settings = UDualNodeCoreSettings::Get())
	{
		if (Settings->LobbyStartMode == ELobbyStartMode::PlayerReady)
		{
			CheckPlayerReadyStatus();
		}
	}
}

void ALobbyGameMode::Logout(AController* Exiting)
{
	Super::Logout(Exiting);
	// In PlayerReady mode, a player leaving could change the state.
	if (const UDualNodeCoreSettings* Settings = UDualNodeCoreSettings::Get())
	{
		if (Settings->LobbyStartMode == ELobbyStartMode::PlayerReady)
		{
			CheckPlayerReadyStatus();
		}
	}
}

void ALobbyGameMode::OnPlayerReadyStatusChanged()
{
	if (const UDualNodeCoreSettings* Settings = UDualNodeCoreSettings::Get())
	{
		if (Settings->LobbyStartMode == ELobbyStartMode::PlayerReady)
		{
			CheckPlayerReadyStatus();
		}
	}
}

void ALobbyGameMode::RequestStartGame(APlayerController* RequestingPlayer)
{
	if (const UDualNodeCoreSettings* Settings = UDualNodeCoreSettings::Get())
	{
		if (Settings->LobbyStartMode == ELobbyStartMode::HostInitiatedTimer)
		{
			// Check if the requesting player is the host (first player)
			if (GetWorld()->GetFirstPlayerController() == RequestingPlayer)
			{
				if (GetGameState<AGameStateBase>()->PlayerArray.Num() >= MinPlayersToStart)
				{
					if (!GetWorld()->GetTimerManager().IsTimerActive(CountdownTimerHandle))
					{
						StartGameCountdown();
					}
				}
			}
		}
	}
}

void ALobbyGameMode::CheckPlayerReadyStatus()
{
	int32 NumberOfReadyPlayers = 0;
	for (APlayerState* PS : GetGameState<AGameStateBase>()->PlayerArray)
	{
		ADualNodeCorePlayerState* CorePS = Cast<ADualNodeCorePlayerState>(PS);
		if (CorePS && CorePS->IsReady())
		{
			NumberOfReadyPlayers++;
		}
	}

	const int32 TotalPlayers = GetGameState<AGameStateBase>()->PlayerArray.Num();
	const bool bEnoughPlayers = TotalPlayers >= MinPlayersToStart;
	const bool bAllPlayersReady = NumberOfReadyPlayers == TotalPlayers;

	if (bEnoughPlayers && bAllPlayersReady)
	{
		if (!GetWorld()->GetTimerManager().IsTimerActive(CountdownTimerHandle))
		{
			StartGameCountdown();
		}
	}
	else
	{
		if (GetWorld()->GetTimerManager().IsTimerActive(CountdownTimerHandle))
		{
			CancelGameCountdown();
		}
	}
}

void ALobbyGameMode::StartGameCountdown()
{
	UE_LOG(LogTemp, Log, TEXT("Conditions met. Starting countdown."));
	
	ADualNodeCoreGameState* CoreGameState = GetGameState<ADualNodeCoreGameState>();
	if (CoreGameState)
	{
		CoreGameState->SetGamePhase(EGamePhase::PreGameplay);
		CoreGameState->SetLobbyCountdownTime(CountdownDuration);
	}

	GetWorld()->GetTimerManager().SetTimer(CountdownTimerHandle, this, &ALobbyGameMode::UpdateCountdown, 1.0f, true);
}

void ALobbyGameMode::CancelGameCountdown()
{
	UE_LOG(LogTemp, Log, TEXT("Conditions no longer met. Cancelling countdown."));
	GetWorld()->GetTimerManager().ClearTimer(CountdownTimerHandle);

	ADualNodeCoreGameState* CoreGameState = GetGameState<ADualNodeCoreGameState>();
	if (CoreGameState)
	{
		CoreGameState->SetLobbyCountdownTime(0);
		CoreGameState->SetGamePhase(EGamePhase::Lobby);
	}
}

void ALobbyGameMode::UpdateCountdown()
{
	ADualNodeCoreGameState* CoreGameState = GetGameState<ADualNodeCoreGameState>();
	if (CoreGameState)
	{
		const int32 NewTime = CoreGameState->GetLobbyCountdownTime() - 1;
		CoreGameState->SetLobbyCountdownTime(NewTime);

		if (NewTime <= 0)
		{
			GetWorld()->GetTimerManager().ClearTimer(CountdownTimerHandle);
			StartGame();
		}
	}
}

void ALobbyGameMode::StartGame()
{
    UE_LOG(LogTemp, Log, TEXT("Countdown finished. Traveling to gameplay level."));
    
    const UDualNodeCoreSettings* Settings = UDualNodeCoreSettings::Get();
    if (Settings && !Settings->GameplayMap.IsNull())
    {
    	GetWorld()->ServerTravel(Settings->GameplayMap.ToString() + TEXT("?listen"));
    }
    else
    {
    	UE_LOG(LogTemp, Error, TEXT("GameplayMap is not set in DualNodeCoreSettings!"));
    }
}
