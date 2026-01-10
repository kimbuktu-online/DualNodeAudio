// Copyright dual node interactive. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "Interfaces/OnlineSessionInterface.h" // Re-added for IOnlineSession and EOnJoinSessionCompleteResult::Type
#include "OnlineSessionSettings.h" // Re-added for FOnlineSessionSettings, FOnlineSessionSearchResult
#include "DualNodeOnlineDelegates.h" // Added to use the custom delegates
#include "DualNodeCoreGameInstance.generated.h"

// Forward declaration for SteamCorePro session result, or a custom struct
// USTRUCT(BlueprintType)
// struct FSteamCoreProSessionResult; // Placeholder for SteamCorePro specific session result

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

	// Re-added FOnlineSessionSearchResult
	FOnlineSessionSearchResult SessionResult; 
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
	// Session Management (will be implemented using SteamCorePro)
	//~================================================================================================================

	UFUNCTION(BlueprintCallable, Category = "DualNodeCore|Session")
	void HostGame(bool bIsLAN, int32 MaxPlayers, const FString& ServerName);

	UFUNCTION(BlueprintCallable, Category = "DualNodeCore|Session")
	void FindGames(bool bIsLAN);

	UFUNCTION(BlueprintCallable, Category = "DualNodeCore|Session")
	void JoinGame(const FOnlineSessionSearchResult& SearchResult); // Parameter updated for FOnlineSessionSearchResult

	UPROPERTY(BlueprintAssignable, Category = "DualNodeCore|Session")
	FOnServersFound OnServersFound;

	//~================================================================================================================
	// Party Management (will be implemented using SteamCorePro)
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
	// Re-added generic session delegate handlers.
	void OnCreateSessionComplete(FName SessionName, bool bWasSuccessful);
	void OnFindSessionsComplete(bool bWasSuccessful);
	void OnJoinSessionComplete(FName SessionName, EOnJoinSessionCompleteResult::Type Result);
	void OnDestroySessionComplete(FName SessionName, bool bWasSuccessful);

private:
	// Re-added generic Online Subsystem session interface and search object.
	TSharedPtr<class IOnlineSession, ESPMode::ThreadSafe> SessionInterface;
	TSharedPtr<class FOnlineSessionSearch> SessionSearch;
};
