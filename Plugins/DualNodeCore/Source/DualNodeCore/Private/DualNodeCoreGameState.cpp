// Copyright dual node interactive. All Rights Reserved.

#include "DualNodeCoreGameState.h"
#include "Net/UnrealNetwork.h"

ADualNodeCoreGameState::ADualNodeCoreGameState()
{
	GamePhase = EGamePhase::None;
	LobbyCountdownTime = 0;
}

void ADualNodeCoreGameState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ADualNodeCoreGameState, GamePhase);
	DOREPLIFETIME(ADualNodeCoreGameState, LobbyCountdownTime);
}

void ADualNodeCoreGameState::SetGamePhase(EGamePhase NewPhase)
{
	if (HasAuthority() && GamePhase != NewPhase)
	{
		GamePhase = NewPhase;
		OnRep_GamePhase(); // Call on server as well
	}
}

void ADualNodeCoreGameState::OnRep_GamePhase()
{
	UE_LOG(LogTemp, Log, TEXT("GamePhase changed to: %s"), *UEnum::GetValueAsString(GamePhase));
	OnGamePhaseChanged.Broadcast(GamePhase);
}

void ADualNodeCoreGameState::SetLobbyCountdownTime(int32 NewTime)
{
	if (HasAuthority() && LobbyCountdownTime != NewTime)
	{
		LobbyCountdownTime = NewTime;
		OnRep_LobbyCountdownTime(); // Call on server as well
	}
}

void ADualNodeCoreGameState::OnRep_LobbyCountdownTime()
{
	OnLobbyCountdownChanged.Broadcast(LobbyCountdownTime);
}
