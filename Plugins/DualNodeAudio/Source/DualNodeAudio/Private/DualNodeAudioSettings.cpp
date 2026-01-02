// Copyright YourCompany. All Rights Reserved.

#include "DualNodeAudioSettings.h"
#include "UObject/ConstructorHelpers.h"

UDualNodeAudioSettings::UDualNodeAudioSettings(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	CategoryName = TEXT("Game");
	SectionName = TEXT("Dual Node Audio");

	bWarnOnMissingTags = true;
	bEnablePhysicsMaterialSupport = true;

	GlobalSoundMix = TSoftObjectPtr<USoundMix>(FSoftObjectPath(TEXT("/DualNodeAudio/Defaults/Mixing/Mix_GlobalMaster.Mix_GlobalMaster")));

	// --- NEU: Master Tag Mapping ---
	const FName MasterTag = FName("Audio.Master");
	const FString MasterPath = TEXT("/DualNodeAudio/Defaults/Mixing/SC_Master.SC_Master");
	TagToSoundClassDefaults.Add(FGameplayTag::RequestGameplayTag(MasterTag), TSoftObjectPtr<USoundClass>(FSoftObjectPath(MasterPath)));

	// Music
	const FName MusicTag = FName("Audio.Music");
	const FString MusicPath = TEXT("/DualNodeAudio/Defaults/Mixing/SC_Music.SC_Music");
	TagToSoundClassDefaults.Add(FGameplayTag::RequestGameplayTag(MusicTag), TSoftObjectPtr<USoundClass>(FSoftObjectPath(MusicPath)));

	// SFX
	const FName SFXTag = FName("Audio.SFX");
	const FString SFXPath = TEXT("/DualNodeAudio/Defaults/Mixing/SC_SFX.SC_SFX");
	TagToSoundClassDefaults.Add(FGameplayTag::RequestGameplayTag(SFXTag), TSoftObjectPtr<USoundClass>(FSoftObjectPath(SFXPath)));
	
	// Voice
	const FName VoiceTag = FName("Audio.Voice");
	const FString VoicePath = TEXT("/DualNodeAudio/Defaults/Mixing/SC_Voice.SC_Voice");
	TagToSoundClassDefaults.Add(FGameplayTag::RequestGameplayTag(VoiceTag), TSoftObjectPtr<USoundClass>(FSoftObjectPath(VoicePath)));

	// UI
	const FName UITag = FName("Audio.UI");
	const FString UIPath = TEXT("/DualNodeAudio/Defaults/Mixing/SC_UI.SC_UI");
	TagToSoundClassDefaults.Add(FGameplayTag::RequestGameplayTag(UITag), TSoftObjectPtr<USoundClass>(FSoftObjectPath(UIPath)));
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