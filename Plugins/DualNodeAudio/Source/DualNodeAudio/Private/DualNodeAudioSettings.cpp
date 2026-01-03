// Copyright YourCompany. All Rights Reserved.

#include "DualNodeAudioSettings.h"
#include "UObject/ConstructorHelpers.h"

// ZENTRALE DEFINITION: Studio Struktur
// "/Game/" entspricht dem "Content"-Ordner des Projekts.
const FString DNA_PROJECT_ROOT = TEXT("/Game/DualNode/Audio/Defaults/");

UDualNodeAudioSettings::UDualNodeAudioSettings(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	CategoryName = TEXT("Game");
	SectionName = TEXT("Dual Node Audio");

	bWarnOnMissingTags = true;
	bEnablePhysicsMaterialSupport = true;
	
	// Helper Lambda für sauberen Code
	auto MakeDNAPath = [&](const FString& FileName) -> FSoftObjectPath
	{
		return FSoftObjectPath(DNA_PROJECT_ROOT + FileName + "." + FileName);
	};

	// --- DEFAULTS ---
	// Diese zeigen jetzt auf Content/DualNode/Audio/Defaults/...
	GlobalSoundMix = TSoftObjectPtr<USoundMix>(MakeDNAPath("Mix_GlobalMaster"));

	TagToSoundClassDefaults.Add(
		FGameplayTag::RequestGameplayTag(FName("Audio.Master")),
		TSoftObjectPtr<USoundClass>(MakeDNAPath("SC_Master"))
	);

	TagToSoundClassDefaults.Add(
		FGameplayTag::RequestGameplayTag(FName("Audio.Music")),
		TSoftObjectPtr<USoundClass>(MakeDNAPath("SC_Music"))
	);

	TagToSoundClassDefaults.Add(
		FGameplayTag::RequestGameplayTag(FName("Audio.SFX")),
		TSoftObjectPtr<USoundClass>(MakeDNAPath("SC_SFX"))
	);
	
	TagToSoundClassDefaults.Add(
		FGameplayTag::RequestGameplayTag(FName("Audio.Voice")),
		TSoftObjectPtr<USoundClass>(MakeDNAPath("SC_Voice"))
	);

	TagToSoundClassDefaults.Add(
		FGameplayTag::RequestGameplayTag(FName("Audio.UI")),
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