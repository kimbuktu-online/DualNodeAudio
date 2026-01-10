// Copyright dual node interactive. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "DualNodeCorePlayerState.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnPlayerInfoChanged);

/**
 * Base PlayerState class for DualNode projects.
 * Replicates player-specific information like ready status and Steam name.
 */
UCLASS()
class DUALNODECORE_API ADualNodeCorePlayerState : public APlayerState
{
	GENERATED_BODY()

public:
	ADualNodeCorePlayerState();

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	virtual void PostInitializeComponents() override;

	//~================================================================================================================
	// Lobby Ready State
	//~================================================================================================================

	UFUNCTION(BlueprintCallable, Category = "DualNodeCore|PlayerState")
	bool IsReady() const { return bIsReady; }

	UFUNCTION(Server, Reliable, BlueprintCallable, Category = "DualNodeCore|PlayerState")
	void Server_SetIsReady(bool bReady);

	//~================================================================================================================
	// Player Info
	//~================================================================================================================

	UFUNCTION(BlueprintPure, Category = "DualNodeCore|PlayerState")
	const FString& GetPlayerSteamName() const { return PlayerSteamName; }

	// Delegate broadcast when any player info (name, etc.) changes.
	UPROPERTY(BlueprintAssignable, Category = "DualNodeCore|PlayerState")
	FOnPlayerInfoChanged OnPlayerInfoChanged;

protected:
	UFUNCTION()
	void OnRep_IsReady();

	UPROPERTY(ReplicatedUsing = OnRep_IsReady)
	bool bIsReady;

	UFUNCTION()
	void OnRep_PlayerSteamName();

	UPROPERTY(ReplicatedUsing = OnRep_PlayerSteamName)
	FString PlayerSteamName;
};
