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

#include "Kismet/GameplayStatics.h"
#include "FindSessionsCallbackProxy.h"

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
		}
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
		SessionSettings.Set(SETTING_GAMEMODE, ServerName, EOnlineDataAdvertisementType::ViaOnlineServiceAndPing);

		SessionInterface->CreateSession(0, NAME_GameSession, SessionSettings);
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

		SessionInterface->FindSessions(0, SessionSearch.ToSharedRef());
	}
}

void UDualNodeCoreGameInstance::JoinGame(const FBlueprintSessionResult& SearchResult)
{
	if (SessionInterface.IsValid())
	{
		SessionInterface->JoinSession(0, NAME_GameSession, SearchResult.OnlineResult);
	}
}

void UDualNodeCoreGameInstance::OnCreateSessionComplete(FName SessionName, bool bWasSuccessful)
{
	if (bWasSuccessful)
	{
		UE_LOG(LogTemp, Log, TEXT("Session Created Successfully."));
		
		const UDualNodeCoreSettings* Settings = UDualNodeCoreSettings::Get();
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
		UE_LOG(LogTemp, Warning, TEXT("Failed to create session."));
	}
}

void UDualNodeCoreGameInstance::OnFindSessionsComplete(bool bWasSuccessful)
{
	if (bWasSuccessful && SessionSearch.IsValid())
	{
		UE_LOG(LogTemp, Log, TEXT("Found %d sessions."), SessionSearch->SearchResults.Num());
		// Broadcast results to UI
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
		if (SessionInterface.IsValid())
		{
			FString Address;
			if (SessionInterface->GetResolvedConnectString(SessionName, Address))
			{
				GetFirstLocalPlayerController()->ClientTravel(Address, ETravelType::TRAVEL_Absolute);
			}
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Failed to join session."));
	}
}
