// Copyright dual node interactive. All Rights Reserved.

#include "Widgets/DualNodeDevConnectWidget.h"

void UDualNodeDevConnectWidget::NativeConstruct()
{
	Super::NativeConstruct();

	CoreGameInstance = GetGameInstance<UDualNodeCoreGameInstance>();
	if (CoreGameInstance)
	{
		CoreGameInstance->OnServersFound.AddUniqueDynamic(this, &UDualNodeDevConnectWidget::OnServersFound);
	}
}

void UDualNodeDevConnectWidget::NativeDestruct()
{
	if (CoreGameInstance)
	{
		CoreGameInstance->OnServersFound.RemoveDynamic(this, &UDualNodeDevConnectWidget::OnServersFound);
	}
	
	Super::NativeDestruct();
}

void UDualNodeDevConnectWidget::FindServers()
{
	if (CoreGameInstance)
	{
		CoreGameInstance->FindGames(false); // Find non-LAN games
	}
}

void UDualNodeDevConnectWidget::JoinServer(FServerInfo ServerToJoin)
{
	if (CoreGameInstance)
	{
		CoreGameInstance->JoinGame(ServerToJoin.SessionResult);
	}
}

void UDualNodeDevConnectWidget::OnServersFound(const TArray<FServerInfo>& ServerList)
{
	// Pass the data to the Blueprint widget to be displayed.
	OnServerListUpdated(ServerList);
}
