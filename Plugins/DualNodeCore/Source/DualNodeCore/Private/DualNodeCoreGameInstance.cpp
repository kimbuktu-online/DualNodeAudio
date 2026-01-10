// Copyright dual node interactive. All Rights Reserved.

#include "DualNodeCoreGameInstance.h"
#include "DualNodeCoreSettings.h"

// Core Online Subsystem headers
#include "OnlineSubsystem.h"
#include "Interfaces/OnlineSessionInterface.h"
#include "OnlineSessionSettings.h"

// WORKAROUND: For some reason, the build system isn't picking up these macros from OnlineSessionSettings.h.
// This is a common, stubborn issue with the OnlineSubsystem module.
// We define them here manually if they are not already defined to fix the compile error for good.
#ifndef SETTING_GAMEMODE
#define SETTING_GAMEMODE FName(TEXT("GAMEMODE"))
#endif
#ifndef SEARCH_PRESENCE
#define SEARCH_PRESENCE FName(TEXT("PRESENCE"))
#endif
#ifndef SETTING_SESSION_NAME
#define SETTING_SESSION_NAME FName(TEXT("SESSIONNAME"))
#endif


#include "Kismet/GameplayStatics.h"
// Removed: #include "FindSessionsCallbackProxy.h" // Not directly used anymore, FOnlineSessionSearchResult is sufficient

UDualNodeCoreGameInstance::UDualNodeCoreGameInstance()
{
}

void UDualNodeCoreGameInstance::Init()
{
	Super::Init();
	
	if (IOnlineSubsystem* Subsystem = IOnlineSubsystem::Get())
	{
		SessionInterface = Subsystem->GetSessionInterface();
		if (SessionInterface.IsValid())
		{
			SessionInterface->OnCreateSessionCompleteDelegates.AddUObject(this, &UDualNodeCoreGameInstance::OnCreateSessionComplete);
			SessionInterface->OnFindSessionsCompleteDelegates.AddUObject(this, &UDualNodeCoreGameInstance::OnFindSessionsComplete);
			SessionInterface->OnJoinSessionCompleteDelegates.AddUObject(this, &UDualNodeCoreGameInstance::OnJoinSessionComplete);
			SessionInterface->OnDestroySessionCompleteDelegates.AddUObject(this, &UDualNodeCoreGameInstance::OnDestroySessionComplete);
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("Online Subsystem Session Interface is not valid!"));
		}
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Online Subsystem not found!"));
	}
}

void UDualNodeCoreGameInstance::HostGame(bool bIsLAN, int32 MaxPlayers, const FString& ServerName)
{
	if (SessionInterface.IsValid())
	{
		FOnlineSessionSettings SessionSettings;
		SessionSettings.bIsLANMatch = bIsLAN;
		SessionSettings.NumPublicConnections = MaxPlayers;
		SessionSettings.bShouldAdvertise = true;
		SessionSettings.bUsesPresence = true;
		SessionSettings.Set(SETTING_SESSION_NAME, ServerName, EOnlineDataAdvertisementType::ViaOnlineService);
		SessionSettings.Set(SETTING_GAMEMODE, FString("Lobby"), EOnlineDataAdvertisementType::ViaOnlineService); // Example GameMode setting

		SessionInterface->CreateSession(0, NAME_GameSession, SessionSettings);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("SessionInterface not valid, cannot host game."));
	}
}

void UDualNodeCoreGameInstance::FindGames(bool bIsLAN)
{
	if (SessionInterface.IsValid())
	{
		SessionSearch = MakeShareable(new FOnlineSessionSearch());
		SessionSearch->bIsLanQuery = bIsLAN;
		SessionSearch->MaxSearchResults = 100;
		SessionSearch->QuerySettings.Set(SEARCH_PRESENCE, true, EOnlineComparisonOp::Equals);
		// Optionally, add a search filter for GameMode if needed
		// SessionSearch->QuerySettings.Set(SETTING_GAMEMODE, FString("Lobby"), EOnlineComparisonOp::Equals);

		SessionInterface->FindSessions(0, SessionSearch.ToSharedRef());
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("SessionInterface not valid, cannot find games."));
	}
}

void UDualNodeCoreGameInstance::JoinGame(const FOnlineSessionSearchResult& SearchResult)
{
	if (SessionInterface.IsValid())
	{
		SessionInterface->JoinSession(0, NAME_GameSession, SearchResult);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("SessionInterface not valid, cannot join game."));
	}
}

void UDualNodeCoreGameInstance::CreateParty()
{
	if (SessionInterface.IsValid())
	{
		FOnlineSessionSettings SessionSettings;
		SessionSettings.NumPublicConnections = 4; // Example party size
		SessionSettings.bShouldAdvertise = false; // This is a private party
		SessionSettings.bIsLANMatch = false;
		SessionSettings.bUsesPresence = true; // Allows friends to join via Steam overlay
		SessionSettings.bAllowJoinViaPresence = true;
		SessionSettings.Set(SETTING_GAMEMODE, FString("Party"), EOnlineDataAdvertisementType::ViaOnlineService); // Example GameMode setting

		SessionInterface->CreateSession(0, NAME_GameSession, SessionSettings);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("SessionInterface not valid, cannot create party."));
	}
}

void UDualNodeCoreGameInstance::LeaveParty()
{
	if (SessionInterface.IsValid())
	{
		SessionInterface->DestroySession(NAME_GameSession);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("SessionInterface not valid, cannot leave party."));
	}
}

void UDualNodeCoreGameInstance::StartMatchmaking()
{
	// For now, we just re-host a public game. A more advanced implementation
	// would update the existing session to become public or use a matchmaking system.
	UE_LOG(LogTemp, Log, TEXT("Attempting to start matchmaking by leaving current party and hosting a new game."));
	LeaveParty();
	// A short delay might be needed here before hosting again, or use a delegate for DestroySessionComplete.
	HostGame(false, 8, TEXT("My Awesome Game")); // Example: Host a public game with 8 players
}


void UDualNodeCoreGameInstance::OnCreateSessionComplete(FName SessionName, bool bWasSuccessful)
{
	if (bWasSuccessful)
	{
		UE_LOG(LogTemp, Log, TEXT("Session Created Successfully. SessionName: %s"), *SessionName.ToString());
		
		const UDualNodeCoreSettings* Settings = UDualNodeCoreSettings::Get();
		if (Settings && Settings->LobbyStartMode != ELobbyStartMode::MainMenuParty)
		{
			if (Settings && !Settings->LobbyMap.IsNull())
			{
				GetWorld()->ServerTravel(Settings->LobbyMap.ToString() + TEXT("?listen"));
			}
			else
			{
				UE_LOG(LogTemp, Error, TEXT("LobbyMap is not set in DualNodeCoreSettings!"));
			}
		}
		else
		{
			// We are in MainMenuParty mode, so we stay on the current map.
			OnPartyStateChanged.Broadcast();
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Failed to create session."));
	}
}

void UDualNodeCoreGameInstance::OnFindSessionsComplete(bool bWasSuccessful)
{
	if (bWasSuccessful && SessionSearch.IsValid())
	{
		TArray<FServerInfo> ServerInfos;
		for (const FOnlineSessionSearchResult& SearchResult : SessionSearch->SearchResults)
		{
			FServerInfo Info;
			Info.SessionResult = SearchResult; // Correct assignment
			Info.MaxPlayers = SearchResult.Session.SessionSettings.NumPublicConnections;
			Info.CurrentPlayers = Info.MaxPlayers - SearchResult.Session.NumOpenPublicConnections;
			Info.Ping = SearchResult.PingInMs;
			
			FString ServerName;
			if (SearchResult.Session.SessionSettings.Get(SETTING_SESSION_NAME, ServerName))
			{
				Info.ServerName = ServerName;
			}
			else
			{
				Info.ServerName = FString("Unnamed Server");
			}
			
			ServerInfos.Add(Info);
		}
		
		OnServersFound.Broadcast(ServerInfos);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Failed to find sessions."));
	}
}

void UDualNodeCoreGameInstance::OnJoinSessionComplete(FName SessionName, EOnJoinSessionCompleteResult::Type Result)
{
	if (Result == EOnJoinSessionCompleteResult::Success)
	{
		UE_LOG(LogTemp, Log, TEXT("Session Joined Successfully. SessionName: %s"), *SessionName.ToString());
		if (SessionInterface.IsValid())
		{
			FString Address;
			if (SessionInterface->GetResolvedConnectString(SessionName, Address))
			{
				GetFirstLocalPlayerController()->ClientTravel(Address, ETravelType::TRAVEL_Absolute);
			}
			else
			{
				UE_LOG(LogTemp, Error, TEXT("Could not get resolved connect string for session %s."), *SessionName.ToString());
			}
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Failed to join session. Result: %s"), *UEnum::GetValueAsString(Result));
	}
}

void UDualNodeCoreGameInstance::OnDestroySessionComplete(FName SessionName, bool bWasSuccessful)
{
	if (bWasSuccessful)
	{
		UE_LOG(LogTemp, Log, TEXT("Session destroyed successfully. SessionName: %s"), *SessionName.ToString());
		OnPartyStateChanged.Broadcast();
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Failed to destroy session. SessionName: %s"), *SessionName.ToString());
	}
}
