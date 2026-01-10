// Copyright dual node interactive. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "Interfaces/OnlineSessionInterface.h"
#include "FindSessionsCallbackProxy.h" // Required for FBlueprintSessionResult
#include "DualNodeCoreGameInstance.generated.h"

USTRUCT(BlueprintType)
struct FServerInfo
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly, Category = "Server Info")
	FString ServerName;

	UPROPERTY(BlueprintReadOnly, Category = "Server Info")
	int32 CurrentPlayers;

	UPROPERTY(BlueprintReadOnly, Category = "Server Info")
	int32 MaxPlayers;

	UPROPERTY(BlueprintReadOnly, Category = "Server Info")
	int32 Ping;

	FBlueprintSessionResult SessionResult;
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnServersFound, const TArray<FServerInfo>&, ServerList);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnPartyStateChanged);


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

	UFUNCTION(BlueprintCallable, Category = "DualNodeCore|Session")
	void JoinGame(const FBlueprintSessionResult& SearchResult);

	UPROPERTY(BlueprintAssignable, Category = "DualNodeCore|Session")
	FOnServersFound OnServersFound;

	//~================================================================================================================
	// Party Management
	//~================================================================================================================

	UFUNCTION(BlueprintCallable, Category = "DualNodeCore|Party")
	void CreateParty();

	UFUNCTION(BlueprintCallable, Category = "DualNodeCore|Party")
	void LeaveParty();
	
	UFUNCTION(BlueprintCallable, Category = "DualNodeCore|Party")
	void StartMatchmaking();

	UPROPERTY(BlueprintAssignable, Category = "DualNodeCore|Party")
	FOnPartyStateChanged OnPartyStateChanged;

protected:
	//~================================================================================================================
	// Session Delegate Handlers
	//~================================================================================================================
	
	void OnCreateSessionComplete(FName SessionName, bool bWasSuccessful);
	void OnFindSessionsComplete(bool bWasSuccessful);
	void OnJoinSessionComplete(FName SessionName, EOnJoinSessionCompleteResult::Type Result);
	void OnDestroySessionComplete(FName SessionName, bool bWasSuccessful);

private:
	TSharedPtr<class IOnlineSession, ESPMode::ThreadSafe> SessionInterface;
	TSharedPtr<class FOnlineSessionSearch> SessionSearch;
};
