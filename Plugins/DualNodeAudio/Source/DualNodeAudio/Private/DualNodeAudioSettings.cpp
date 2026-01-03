// Copyright YourCompany. All Rights Reserved.

#include "DualNodeAudioSettings.h"
#include "UObject/ConstructorHelpers.h"

// ZENTRALE DEFINITION: Studio Struktur
const FString DNA_PROJECT_ROOT = TEXT("/Game/DualNode/Audio/Defaults/");

UDualNodeAudioSettings::UDualNodeAudioSettings(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	CategoryName = TEXT("Game");
	SectionName = TEXT("Dual Node Audio");

	bWarnOnMissingTags = true;
	bEnablePhysicsMaterialSupport = true;
	
	auto MakeDNAPath = [&](const FString& FileName) -> FSoftObjectPath
	{
		return FSoftObjectPath(DNA_PROJECT_ROOT + FileName + "." + FileName);
	};

	GlobalSoundMix = TSoftObjectPtr<USoundMix>(MakeDNAPath("Mix_GlobalMaster"));

	// FIX: "false" als zweiter Parameter verhindert Crash beim Engine-Start
	
	TagToSoundClassDefaults.Add(
		FGameplayTag::RequestGameplayTag(FName("Audio.Master"), false),
		TSoftObjectPtr<USoundClass>(MakeDNAPath("SC_Master"))
	);

	TagToSoundClassDefaults.Add(
		FGameplayTag::RequestGameplayTag(FName("Audio.Music"), false),
		TSoftObjectPtr<USoundClass>(MakeDNAPath("SC_Music"))
	);

	TagToSoundClassDefaults.Add(
		FGameplayTag::RequestGameplayTag(FName("Audio.SFX"), false),
		TSoftObjectPtr<USoundClass>(MakeDNAPath("SC_SFX"))
	);
	
	TagToSoundClassDefaults.Add(
		FGameplayTag::RequestGameplayTag(FName("Audio.Voice"), false),
		TSoftObjectPtr<USoundClass>(MakeDNAPath("SC_Voice"))
	);

	TagToSoundClassDefaults.Add(
		FGameplayTag::RequestGameplayTag(FName("Audio.UI"), false),
		TSoftObjectPtr<USoundClass>(MakeDNAPath("SC_UI"))
	);
}

#if WITH_EDITOR
FText UDualNodeAudioSettings::GetSectionText() const
{
	return NSLOCTEXT("DualNodeAudio", "DualNodeAudioSettingsSection", "Dual Node Audio");
}

FName UDualNodeAudioSettings::GetCategoryName() const
{
	return TEXT("Game");
}
#endif