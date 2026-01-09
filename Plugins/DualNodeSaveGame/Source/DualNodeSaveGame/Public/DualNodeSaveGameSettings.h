// Copyright Dual Node.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DeveloperSettings.h"
#include "DualNodeSaveGameSettings.generated.h"

UENUM(BlueprintType)
enum class EDualNodeSaveSubsystemStrategy : uint8
{
	Default UMETA(DisplayName = "Default Online Subsystem"),
	SteamCorePro UMETA(DisplayName = "SteamCore-Pro")
};

UCLASS(Config = Game, DefaultConfig, meta = (DisplayName = "DualNode Save Game Settings"))
class DUALNODESAVEGAME_API UDualNodeSaveGameSettings : public UDeveloperSettings
{
	GENERATED_BODY()

public:
	UPROPERTY(Config, EditAnywhere, BlueprintReadOnly, Category = "Online Subsystem")
	EDualNodeSaveSubsystemStrategy SaveSubsystemStrategy = EDualNodeSaveSubsystemStrategy::SteamCorePro;
};
