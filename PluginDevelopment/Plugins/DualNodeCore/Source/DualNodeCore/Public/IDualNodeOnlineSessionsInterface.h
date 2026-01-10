// Copyright dual node interactive. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "OnlineSubsystem.h" // Main Online Subsystem header, should be included early
#include "Interfaces/OnlineSessionInterface.h" // For IOnlineSession and EOnJoinSessionCompleteResult::Type
#include "OnlineSessionSettings.h" // Contains FOnlineSessionSettings, FOnlineSessionSearchResult, FOnlineSessionSearch
#include "OnlineSubsystemTypes.h" // For fundamental Online Subsystem types like FUniqueNetIdRepl
#include "DualNodeOnlineDelegates.h" // Include the new delegates header

#include "IDualNodeOnlineSessionsInterface.generated.h"

UINTERFACE(MinimalAPI, NotBlueprintable)
class UDualNodeOnlineSessionsInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * Generic interface for online session management.
 */
class DUALNODECORE_API IDualNodeOnlineSessionsInterface
{
	GENERATED_BODY()

public:
	// Session Management Functions
	UFUNCTION(BlueprintCallable, Category = "DualNode|OnlineSessions")
	virtual void CreateGameSession(int32 MaxPlayers, FName SessionName, bool bIsLAN, bool bIsPresence, const TMap<FString, FString>& SessionSettings) = 0;

	UFUNCTION(BlueprintCallable, Category = "DualNode|OnlineSessions")
	virtual void FindGameSessions(int32 MaxSearchResults, bool bIsLAN, bool bIsPresence) = 0;

	UFUNCTION(BlueprintCallable, Category = "DualNode|OnlineSessions")
	virtual void JoinGameSession(const FOnlineSessionSearchResult& SessionResult) = 0;

	UFUNCTION(BlueprintCallable, Category = "DualNode|OnlineSessions")
	virtual void DestroyGameSession() = 0;

	// Delegates (These will be moved to the provider class)
	// UPROPERTY(BlueprintAssignable, Category = "DualNode|OnlineSessions")
	// FOnCreateSessionComplete OnCreateSessionCompleteEvent;

	// UPROPERTY(BlueprintAssignable, Category = "DualNode|OnlineSessions")
	// FOnFindSessionsComplete OnFindSessionsCompleteEvent;

	// UPROPERTY(BlueprintAssignable, Category = "DualNode|OnlineSessions")
	// FOnJoinSessionComplete OnJoinSessionCompleteEvent;

	// UPROPERTY(BlueprintAssignable, Category = "DualNode|OnlineSessions")
	// FOnDestroySessionComplete OnDestroySessionCompleteEvent;
};
