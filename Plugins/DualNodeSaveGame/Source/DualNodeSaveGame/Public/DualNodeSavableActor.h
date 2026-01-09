// Copyright Dual Node.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "DualNodeSaveGameData.h" // Include our new data structure
#include "DualNodeSavableActor.generated.h"

UINTERFACE(MinimalAPI, Blueprintable) // Make it Blueprintable
class UDualNodeSavableActor : public UInterface
{
	GENERATED_BODY()
};

class DUALNODESAVEGAME_API IDualNodeSavableActor
{
	GENERATED_BODY()

public:
	// This function is called by the SaveManager when a save is requested.
	// The actor is responsible for packaging its state into the FActorSaveData struct.
	UFUNCTION(BlueprintNativeEvent, Category = "DualNode Save Game")
	FActorSaveData GetActorSaveData();

	// This function is called by the SaveManager when a load is requested.
	// The actor is responsible for restoring its state from the given FActorSaveData.
	UFUNCTION(BlueprintNativeEvent, Category = "DualNode Save Game")
	void LoadActorSaveData(const FActorSaveData& ActorData);
};
