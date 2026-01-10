// Copyright dual node interactive. All Rights Reserved.

#include "DualNodeCorePlayerController.h"
#include "LobbyGameMode.h"

void ADualNodeCorePlayerController::RequestStartGame()
{
	// This is called on the client, so we send an RPC to the server.
	Server_RequestStartGame();
}

void ADualNodeCorePlayerController::Server_RequestStartGame_Implementation()
{
	// This runs on the server.
	if (GetWorld() && GetWorld()->GetAuthGameMode())
	{
		if (ALobbyGameMode* LobbyGM = Cast<ALobbyGameMode>(GetWorld()->GetAuthGameMode()))
		{
			LobbyGM->RequestStartGame(this);
		}
	}
}
