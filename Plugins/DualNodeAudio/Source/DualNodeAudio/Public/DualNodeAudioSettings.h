#pragma once

#include "CoreMinimal.h"
#include "Engine/DeveloperSettings.h"
#include "Engine/EngineTypes.h"
#include "DualNodeAudioRegistry.h"
#include "Sound/SoundMix.h"   // WICHTIG
#include "Sound/SoundClass.h" // WICHTIG
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

	// --- REGISTRIES ---
	UPROPERTY(Config, EditAnywhere, BlueprintReadOnly, Category = "General")
	TArray<TSoftObjectPtr<UDualNodeAudioRegistry>> Registries;

	// --- MIXING (NEU IN V13.0) ---
	// Der Master-Mix, der beim Start automatisch gepusht wird.
	UPROPERTY(Config, EditAnywhere, BlueprintReadOnly, Category = "Mixing")
	TSoftObjectPtr<USoundMix> GlobalSoundMix;

	// Mapping: Welcher Tag gehört zu welcher SoundClass?
	// Key: "Audio.Type.Music" -> Value: SC_Music
	UPROPERTY(Config, EditAnywhere, BlueprintReadOnly, Category = "Mixing")
	TMap<FGameplayTag, TSoftObjectPtr<USoundClass>> TagToSoundClassDefaults;

	// --- DEBUGGING ---
	UPROPERTY(Config, EditAnywhere, Category = "Debugging")
	bool bEnableVerboseLogging = false;

	UPROPERTY(Config, EditAnywhere, Category = "Debugging")
	bool bWarnOnMissingTags = true;

	UPROPERTY(Config, EditAnywhere, Category = "Debugging")
	bool bDrawDebugSpheres = false;

	// --- BEHAVIOR ---
	UPROPERTY(Config, EditAnywhere, Category = "Behavior")
	bool bPauseMusicOnGamePause = true;

	UPROPERTY(Config, EditAnywhere, Category = "Behavior")
	bool bResetStackOnLevelChange = true;

	UPROPERTY(Config, EditAnywhere, Category = "Behavior")
	bool bEnableServerTimeSync = true;

	// --- PHYSICS ---
	UPROPERTY(Config, EditAnywhere, Category = "Physics")
	bool bEnablePhysicsMaterialSupport = true;

	UPROPERTY(Config, EditAnywhere, Category = "Physics", meta=(EditCondition="bEnablePhysicsMaterialSupport"))
	TEnumAsByte<ECollisionChannel> PhysicsTraceChannel = ECC_Visibility;

	static const UDualNodeAudioSettings* Get() { return GetDefault<UDualNodeAudioSettings>(); }
};