// Copyright dual node interactive. All Rights Reserved.

#include "Widgets/DualNodeLobbyWidget.h"
#include "DualNodeCore/Public/DualNodeCoreGameState.h"
#include "DualNodeCore/Public/DualNodeCorePlayerState.h"
#include "DualNodeCore/Public/DualNodeCorePlayerController.h"
#include "DualNodeCore/Public/DualNodeCoreSettings.h"
#include "Kismet/GameplayStatics.h"

void UDualNodeLobbyWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (ADualNodeCoreGameState* GS = GetWorld()->GetGameState<ADualNodeCoreGameState>())
	{
		GS->OnLobbyCountdownChanged.AddUniqueDynamic(this, &UDualNodeLobbyWidget::OnLobbyCountdownChanged);
	}

	// Initial UI setup
	OnPlayerListChanged();
	OnLobbyCountdownChanged(0);

	// Determine if we should show the "Start Game" button
	const UDualNodeCoreSettings* Settings = UDualNodeCoreSettings::Get();
	if (Settings && Settings->LobbyStartMode == ELobbyStartMode::HostInitiatedTimer)
	{
		if (GetOwningPlayer()->HasAuthority()) // Only the host can start the game
		{
			SetStartGameButtonVisibility(true);
		}
		else
		{
			SetStartGameButtonVisibility(false);
		}
	}
	else
	{
		SetStartGameButtonVisibility(false);
	}
}

void UDualNodeLobbyWidget::NativeDestruct()
{
	if (ADualNodeCoreGameState* GS = GetWorld()->GetGameState<ADualNodeCoreGameState>())
	{
		GS->OnLobbyCountdownChanged.RemoveDynamic(this, &UDualNodeLobbyWidget::OnLobbyCountdownChanged);
	}
	
	Super::NativeDestruct();
}

void UDualNodeLobbyWidget::OnReadyButtonClicked()
{
	if (ADualNodeCorePlayerState* PS = GetOwningPlayerState<ADualNodeCorePlayerState>())
	{
		PS->Server_SetIsReady(!PS->IsReady());
	}
}

void UDualNodeLobbyWidget::OnStartGameButtonClicked()
{
	if (ADualNodeCorePlayerController* PC = GetOwningPlayer<ADualNodeCorePlayerController>())
	{
		PC->RequestStartGame();
	}
}

void UDualNodeLobbyWidget::OnLobbyCountdownChanged(int32 RemainingTime)
{
	OnCountdownUpdated(RemainingTime);
}
