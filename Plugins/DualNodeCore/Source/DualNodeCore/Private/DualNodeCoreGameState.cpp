// Copyright dual node interactive. All Rights Reserved.

#include "DualNodeCoreGameState.h"
#include "Net/UnrealNetwork.h"

ADualNodeCoreGameState::ADualNodeCoreGameState()
{
	GamePhase = EGamePhase::None;
}

void ADualNodeCoreGameState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ADualNodeCoreGameState, GamePhase);
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
