// Copyright dual node interactive. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameState.h"
#include "DualNodeCoreTypes.h"
#include "DualNodeCoreGameState.generated.h"

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

	//~================================================================================================================
	// Game Phase Management
	//~================================================================================================================

	UFUNCTION(BlueprintCallable, Category = "DualNodeCore|GameState")
	EGamePhase GetGamePhase() const { return GamePhase; }

	void SetGamePhase(EGamePhase NewPhase);

protected:
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	UFUNCTION()
	void OnRep_GamePhase();

	UPROPERTY(ReplicatedUsing = OnRep_GamePhase)
	EGamePhase GamePhase;

	// Delegate to broadcast phase changes to UI and other systems
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnGamePhaseChanged, EGamePhase, NewPhase);
	UPROPERTY(BlueprintAssignable, Category = "DualNodeCore|GameState")
	FOnGamePhaseChanged OnGamePhaseChanged;
};
