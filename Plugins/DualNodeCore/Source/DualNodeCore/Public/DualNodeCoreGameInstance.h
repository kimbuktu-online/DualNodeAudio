// Copyright dual node interactive. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "Interfaces/OnlineSessionInterface.h"
#include "FindSessionsCallbackProxy.h" // Required for FOnlineSessionSearchResult
#include "DualNodeCoreGameInstance.generated.h"

/**
 * Base GameInstance class for DualNode projects.
 * Manages Online Subsystem sessions and the overall game lifecycle.
 */
UCLASS()
class DUALNODECORE_API UDualNodeCoreGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:
	UDualNodeCoreGameInstance();

	virtual void Init() override;

	//~================================================================================================================
	// Session Management
	//~================================================================================================================

	UFUNCTION(BlueprintCallable, Category = "DualNodeCore|Session")
	void HostGame(bool bIsLAN, int32 MaxPlayers, const FString& ServerName);

	UFUNCTION(BlueprintCallable, Category = "DualNodeCore|Session")
	void FindGames(bool bIsLAN);

	// We use a Blueprint-compatible wrapper for the search result to avoid further include issues in other headers.
	UFUNCTION(BlueprintCallable, Category = "DualNodeCore|Session")
	void JoinGame(const FBlueprintSessionResult& SearchResult);

protected:
	//~================================================================================================================
	// Session Delegate Handlers
	//~================================================================================================================
	
	void OnCreateSessionComplete(FName SessionName, bool bWasSuccessful);
	void OnFindSessionsComplete(bool bWasSuccessful);
	void OnJoinSessionComplete(FName SessionName, EOnJoinSessionCompleteResult::Type Result);

private:
	TSharedPtr<class IOnlineSession, ESPMode::ThreadSafe> SessionInterface;
	TSharedPtr<class FOnlineSessionSearch> SessionSearch;
};
