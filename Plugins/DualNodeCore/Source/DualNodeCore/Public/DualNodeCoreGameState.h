// Copyright dual node interactive. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameState.h"
#include "DualNodeCoreTypes.h"
#include "DualNodeCoreGameState.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnGamePhaseChanged, EGamePhase, NewPhase);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnLobbyCountdownChanged, int32, RemainingTime);

/**
 * Base GameState class for DualNode projects.
 * Replicates the global game state to all clients.
 */
UCLASS()
class DUALNODECORE_API ADualNodeCoreGameState : public AGameState
{
	GENERATED_BODY()

public:
	ADualNodeCoreGameState();

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	//~================================================================================================================
	// Game Phase Management
	//~================================================================================================================

	UFUNCTION(BlueprintCallable, Category = "DualNodeCore|GameState")
	EGamePhase GetGamePhase() const { return GamePhase; }

	void SetGamePhase(EGamePhase NewPhase);

	UPROPERTY(BlueprintAssignable, Category = "DualNodeCore|GameState")
	FOnGamePhaseChanged OnGamePhaseChanged;

	//~================================================================================================================
	// Lobby Countdown
	//~================================================================================================================

	UFUNCTION(BlueprintCallable, Category = "DualNodeCore|GameState")
	int32 GetLobbyCountdownTime() const { return LobbyCountdownTime; }
	
	void SetLobbyCountdownTime(int32 NewTime);

	UPROPERTY(BlueprintAssignable, Category = "DualNodeCore|GameState")
	FOnLobbyCountdownChanged OnLobbyCountdownChanged;

protected:
	UFUNCTION()
	void OnRep_GamePhase();

	UPROPERTY(ReplicatedUsing = OnRep_GamePhase)
	EGamePhase GamePhase;

	UFUNCTION()
	void OnRep_LobbyCountdownTime();

	UPROPERTY(ReplicatedUsing = OnRep_LobbyCountdownTime)
	int32 LobbyCountdownTime;
};
