// Copyright Dual Node.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "DualNodeSaveGameData.generated.h"

// Struct to hold data for a single saved actor
USTRUCT(BlueprintType)
struct FActorSaveData
{
    GENERATED_BODY()

public:
    // The unique name of the actor instance in the level
    UPROPERTY(BlueprintReadWrite, Category = "DualNode Save Game")
    FName ActorName;

    // The class of the actor, in case we need to spawn it on load
    UPROPERTY(BlueprintReadWrite, Category = "DualNode Save Game")
    TSubclassOf<AActor> ActorClass;

    // Actor's location, rotation, and scale
    UPROPERTY(BlueprintReadWrite, Category = "DualNode Save Game")
    FTransform ActorTransform;

    // A byte array to hold any custom data the actor wants to save (e.g. health, ammo, etc.)
    UPROPERTY(BlueprintReadWrite, Category = "DualNode Save Game")
    TArray<uint8> ActorData;
};


UCLASS()
class DUALNODESAVEGAME_API UDualNodeSaveGame : public USaveGame
{
    GENERATED_BODY()

public:
    // The name of the slot this save game was saved to
    UPROPERTY(VisibleAnywhere, Category = "Basic")
    FString SlotName;

    // The date and time this save was created
    UPROPERTY(VisibleAnywhere, Category = "Basic")
    FDateTime SaveDate;

    // An array holding the data for all saved actors
    UPROPERTY()
    TArray<FActorSaveData> SavedActors;
};
