// Copyright dual node interactive. All Rights Reserved.

#include "Widgets/DualNodeMainMenuWidget.h"
#include "DualNodeCore/Public/DualNodeCoreGameInstance.h"

void UDualNodeMainMenuWidget::HostGame(int32 MaxPlayers, const FString& ServerName)
{
	if (UDualNodeCoreGameInstance* GI = GetGameInstance<UDualNodeCoreGameInstance>())
	{
		GI->HostGame(false, MaxPlayers, ServerName);
	}
}

void UDualNodeMainMenuWidget::CreateParty()
{
	if (UDualNodeCoreGameInstance* GI = GetGameInstance<UDualNodeCoreGameInstance>())
	{
		GI->CreateParty();
	}
}
