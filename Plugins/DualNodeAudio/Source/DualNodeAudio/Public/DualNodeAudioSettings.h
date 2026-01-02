// Copyright YourCompany. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DeveloperSettings.h"
#include "GameplayTagContainer.h"
#include "Sound/SoundMix.h"
#include "Sound/SoundClass.h"
#include "Engine/EngineTypes.h" // Für ECollisionChannel
#include "DualNodeAudioRegistry.h"
#include "DualNodeAudioSettings.generated.h"

/**
 * Konfiguration für das DualNode Audio System.
 * Erscheint in Project Settings -> Game -> Dual Node Audio.
 */
UCLASS(Config=Game, defaultconfig, meta=(DisplayName="Dual Node Audio"))
class DUALNODEAUDIO_API UDualNodeAudioSettings : public UDeveloperSettings
{
	GENERATED_BODY()

public:
	UDualNodeAudioSettings(const FObjectInitializer& ObjectInitializer);

	// --- HELPER ---
	static const UDualNodeAudioSettings* Get() { return GetDefault<UDualNodeAudioSettings>(); }

	// --- ASSET SETUP ---
	/** Der globale SoundMix, der beim Start geladen werden soll */
	UPROPERTY(Config, EditAnywhere, Category = "Audio Setup", meta = (AllowedClasses = "/Script/Engine.SoundMix"))
	TSoftObjectPtr<USoundMix> GlobalSoundMix;

	/** Mapping von GameplayTags zu Standard SoundClasses */
	UPROPERTY(Config, EditAnywhere, Category = "Audio Setup")
	TMap<FGameplayTag, TSoftObjectPtr<USoundClass>> TagToSoundClassDefaults;
	
	/** Liste der aktiven Audio Registries (Data Assets) */
	UPROPERTY(Config, EditAnywhere, BlueprintReadOnly, Category = "Audio Setup")
	TArray<TSoftObjectPtr<UDualNodeAudioRegistry>> Registries;

	// --- DEBUGGING ---
	UPROPERTY(Config, EditAnywhere, Category = "Debugging")
	bool bWarnOnMissingTags;

	UPROPERTY(Config, EditAnywhere, Category = "Debugging")
	bool bDrawDebugSpheres;

	// --- BEHAVIOR ---
	UPROPERTY(Config, EditAnywhere, Category = "Behavior")
	bool bEnableServerTimeSync = true;

	UPROPERTY(Config, EditAnywhere, Category = "Behavior")
	bool bResetStackOnLevelChange = true;

	// --- PHYSICS ---
	UPROPERTY(Config, EditAnywhere, Category = "Physics Integration")
	bool bEnablePhysicsMaterialSupport;

	UPROPERTY(Config, EditAnywhere, Category = "Physics Integration", meta=(EditCondition="bEnablePhysicsMaterialSupport"))
	TEnumAsByte<ECollisionChannel> PhysicsTraceChannel = ECC_Visibility;

#if WITH_EDITOR
	virtual FText GetSectionText() const override;
	virtual FName GetCategoryName() const override;
#endif
};