// Copyright YourCompany. All Rights Reserved.

#include "DualNodeAudioSettings.h"
#include "UObject/ConstructorHelpers.h"

UDualNodeAudioSettings::UDualNodeAudioSettings(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	// Standardwerte initialisieren
	CategoryName = TEXT("Game");
	SectionName = TEXT("Dual Node Audio");

	// Default Primitives
	bWarnOnMissingTags = true;
	bEnablePhysicsMaterialSupport = true;

	// Fix C2597 & C2671: Wir greifen direkt auf 'this' Member zu, keine Lambda-Scope Probleme mehr.
	// Fix C2665: Explizite Konstruktion von TSoftObjectPtr
	GlobalSoundMix = TSoftObjectPtr<USoundMix>(FSoftObjectPath(TEXT("/DualNodeAudio/Defaults/Mixing/Mix_GlobalMaster.Mix_GlobalMaster")));

	// Helper Funktion lokal definieren ist okay, aber wir müssen sicherstellen, dass wir keine Member capturen,
	// die wir nicht brauchen, oder wir machen es einfach direkt (sauberer):
	
	// Default Map Einträge setzen
	// Hinweis: Wir nutzen explizit FGameplayTag::RequestGameplayTag, um sicherzugehen, dass der Tag existiert oder erstellt wird.
	
	const FName MusicTag = FName("Audio.Music");
	const FString MusicPath = TEXT("/DualNodeAudio/Defaults/Mixing/SC_Music.SC_Music");
	
	TagToSoundClassDefaults.Add(
		FGameplayTag::RequestGameplayTag(MusicTag),
		TSoftObjectPtr<USoundClass>(FSoftObjectPath(MusicPath))
	);

	const FName SFXTag = FName("Audio.SFX");
	const FString SFXPath = TEXT("/DualNodeAudio/Defaults/Mixing/SC_SFX.SC_SFX");

	TagToSoundClassDefaults.Add(
		FGameplayTag::RequestGameplayTag(SFXTag),
		TSoftObjectPtr<USoundClass>(FSoftObjectPath(SFXPath))
	);
	
	const FName VoiceTag = FName("Audio.Voice");
	const FString VoicePath = TEXT("/DualNodeAudio/Defaults/Mixing/SC_Voice.SC_Voice");

	TagToSoundClassDefaults.Add(
		FGameplayTag::RequestGameplayTag(VoiceTag),
		TSoftObjectPtr<USoundClass>(FSoftObjectPath(VoicePath))
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