#pragma once

#include "CoreMinimal.h"
#include "Engine/DeveloperSettings.h"
#include "Engine/EngineTypes.h" // Für ECollisionChannel
#include "DualNodeAudioRegistry.h"
#include "DualNodeAudioSettings.generated.h"

UCLASS(Config=Game, DefaultConfig, meta=(DisplayName="Dual Node Audio"))
class DUALNODEAUDIO_API UDualNodeAudioSettings : public UDeveloperSettings
{
	GENERATED_BODY()

public:
	UDualNodeAudioSettings() 
	{
		CategoryName = TEXT("Game");
	}

	// --- DATA ---
	UPROPERTY(Config, EditAnywhere, BlueprintReadOnly, Category = "General")
	TArray<TSoftObjectPtr<UDualNodeAudioRegistry>> Registries;

	// --- DEBUGGING (DNA 8.0/11.0) ---
	UPROPERTY(Config, EditAnywhere, Category = "Debugging")
	bool bEnableVerboseLogging = false;

	UPROPERTY(Config, EditAnywhere, Category = "Debugging")
	bool bWarnOnMissingTags = true;

	UPROPERTY(Config, EditAnywhere, Category = "Debugging")
	bool bDrawDebugSpheres = false;

	// --- BEHAVIOR (DNA 8.0) ---
	UPROPERTY(Config, EditAnywhere, Category = "Behavior")
	bool bPauseMusicOnGamePause = true;

	UPROPERTY(Config, EditAnywhere, Category = "Behavior")
	bool bResetStackOnLevelChange = true;

	UPROPERTY(Config, EditAnywhere, Category = "Behavior")
	bool bEnableServerTimeSync = true;

	// --- PHYSICS (DNA 9.0) ---
	UPROPERTY(Config, EditAnywhere, Category = "Physics")
	bool bEnablePhysicsMaterialSupport = true;

	UPROPERTY(Config, EditAnywhere, Category = "Physics", meta=(EditCondition="bEnablePhysicsMaterialSupport"))
	TEnumAsByte<ECollisionChannel> PhysicsTraceChannel = ECC_Visibility;

	static const UDualNodeAudioSettings* Get() { return GetDefault<UDualNodeAudioSettings>(); }
};