// Copyright YourCompany. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DeveloperSettings.h"
#include "GameplayTagContainer.h"
#include "Sound/SoundMix.h"
#include "Sound/SoundClass.h"
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

	/** Der globale SoundMix, der beim Start geladen werden soll */
	UPROPERTY(Config, EditAnywhere, Category = "Audio Setup", meta = (AllowedClasses = "/Script/Engine.SoundMix"))
	TSoftObjectPtr<USoundMix> GlobalSoundMix;

	/** Mapping von GameplayTags zu Standard SoundClasses */
	UPROPERTY(Config, EditAnywhere, Category = "Audio Setup")
	TMap<FGameplayTag, TSoftObjectPtr<USoundClass>> TagToSoundClassDefaults;

	/** Soll eine Warnung ausgegeben werden, wenn Tags fehlen? */
	UPROPERTY(Config, EditAnywhere, Category = "Debugging")
	bool bWarnOnMissingTags;

	/** Aktiviert Support für Physics Materials im Audio System */
	UPROPERTY(Config, EditAnywhere, Category = "Physics Integration")
	bool bEnablePhysicsMaterialSupport;

#if WITH_EDITOR
	virtual FText GetSectionText() const override;
	virtual FName GetCategoryName() const override;
#endif
};