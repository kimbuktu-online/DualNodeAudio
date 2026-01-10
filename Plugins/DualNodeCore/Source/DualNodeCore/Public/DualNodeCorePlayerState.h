// Copyright dual node interactive. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "DualNodeCorePlayerState.generated.h"

/**
 * Base PlayerState class for DualNode projects.
 * Replicates player-specific information.
 */
UCLASS()
class DUALNODECORE_API ADualNodeCorePlayerState : public APlayerState
{
	GENERATED_BODY()

public:
	ADualNodeCorePlayerState();

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	//~================================================================================================================
	// Lobby Ready State
	//~================================================================================================================

	UFUNCTION(BlueprintCallable, Category = "DualNodeCore|PlayerState")
	bool IsReady() const { return bIsReady; }

	UFUNCTION(Server, Reliable, BlueprintCallable, Category = "DualNodeCore|PlayerState")
	void Server_SetIsReady(bool bReady);

protected:
	UFUNCTION()
	void OnRep_IsReady();

	UPROPERTY(ReplicatedUsing = OnRep_IsReady)
	bool bIsReady;

	// Delegate to broadcast ready state changes to UI
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnReadyStateChanged, bool, bNewReadyState);
	UPROPERTY(BlueprintAssignable, Category = "DualNodeCore|PlayerState")
	FOnReadyStateChanged OnReadyStateChanged;
};
