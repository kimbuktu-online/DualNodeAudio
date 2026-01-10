// Copyright dual node interactive. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "IDualNodeOnlineFriendsInterface.h" // Updated include
#include "DualNodeSteamFriendsSubsystem.generated.h"

/**
 * 
 */
UCLASS()
class DUALNODECORE_API UDualNodeSteamFriendsSubsystem : public UGameInstanceSubsystem, public IDualNodeOnlineFriendsInterface // Updated base interface
{
	GENERATED_BODY()

public:
	// Begin USubsystem
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;
	// End USubsystem

	// Begin IDualNodeOnlineFriendsInterface
	UFUNCTION(BlueprintCallable, Category = "DualNode|OnlineFriends") // Added UFUNCTION macro
	virtual UTexture2D* GetMediumFriendAvatarForUniqueNetId(const FUniqueNetIdRepl& UniqueNetId) const override;
	UFUNCTION(BlueprintCallable, Category = "DualNode|OnlineFriends") // Added UFUNCTION macro
	virtual FString GetFriendPersonaNameForUniqueNetId(const FUniqueNetIdRepl& UniqueNetId) const override;
	// End IDualNodeOnlineFriendsInterface
};
