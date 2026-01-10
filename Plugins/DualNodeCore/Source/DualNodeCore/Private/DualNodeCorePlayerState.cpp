// Copyright dual node interactive. All Rights Reserved.

#include "DualNodeCorePlayerState.h"
#include "Net/UnrealNetwork.h"

ADualNodeCorePlayerState::ADualNodeCorePlayerState()
{
	bIsReady = false;
}

void ADualNodeCorePlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ADualNodeCorePlayerState, bIsReady);
}

void ADualNodeCorePlayerState::Server_SetIsReady_Implementation(bool bReady)
{
	bIsReady = bReady;
	OnRep_IsReady(); // Call on server as well
}

void ADualNodeCorePlayerState::OnRep_IsReady()
{
	OnReadyStateChanged.Broadcast(bIsReady);
}
