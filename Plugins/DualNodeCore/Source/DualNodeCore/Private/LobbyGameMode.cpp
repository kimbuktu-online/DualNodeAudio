// Copyright dual node interactive. All Rights Reserved.

#include "LobbyGameMode.h"
#include "DualNodeCoreGameState.h"
#include "DualNodeCorePlayerState.h"
#include "DualNodeCoreSettings.h"
#include "GameFramework/GameStateBase.h"
#include "Kismet/GameplayStatics.h"

ALobbyGameMode::ALobbyGameMode()
{
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.TickInterval = 1.0f;

	CountdownDuration = 10.0f;
}

void ALobbyGameMode::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);
	CheckIfAllPlayersAreReady();
}

void ALobbyGameMode::Logout(AController* Exiting)
{
	Super::Logout(Exiting);
	CheckIfAllPlayersAreReady();
}

void ALobbyGameMode::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	CheckIfAllPlayersAreReady();
}

void ALobbyGameMode::CheckIfAllPlayersAreReady()
{
	if (GetWorld()->GetTimerManager().IsTimerActive(CountdownTimerHandle))
	{
		return;
	}
	
	bool bAllPlayersReady = true;
	for (APlayerState* PS : GetGameState<AGameStateBase>()->PlayerArray)
	{
		ADualNodeCorePlayerState* CorePS = Cast<ADualNodeCorePlayerState>(PS);
		if (!CorePS || !CorePS->IsReady())
		{
			bAllPlayersReady = false;
			break;
		}
	}

	if (bAllPlayersReady && GetGameState<AGameStateBase>()->PlayerArray.Num() > 0)
	{
		StartGameCountdown();
	}
}

void ALobbyGameMode::StartGameCountdown()
{
	UE_LOG(LogTemp, Log, TEXT("All players are ready. Starting countdown."));
	
	ADualNodeCoreGameState* CoreGameState = GetGameState<ADualNodeCoreGameState>();
	if (CoreGameState)
	{
		CoreGameState->SetGamePhase(EGamePhase::PreGameplay);
	}

	GetWorld()->GetTimerManager().SetTimer(CountdownTimerHandle, this, &ALobbyGameMode::StartGame, CountdownDuration, false);
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
