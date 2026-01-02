// Copyright YourCompany. All Rights Reserved.

#include "DualNodeAudio.h"
#include "GameplayTagsManager.h"
#include "Misc/Paths.h"
#include "ShaderCore.h"

#define LOCTEXT_NAMESPACE "FDualNodeAudioModule"

void FDualNodeAudioModule::StartupModule()
{
	// 1. Native Gameplay Tags registrieren
	// Das prüft automatisch, ob sie existieren. Wenn nicht, werden sie erstellt.
	// Wenn sie existieren, werden sie als "Native" markiert (nicht löschbar).
	
	UGameplayTagsManager& Manager = UGameplayTagsManager::Get();

	// Root Tag
	Manager.AddNativeGameplayTag(FName("Audio"), TEXT("DNA Root Tag"));

	// Hierarchy Tags
	Manager.AddNativeGameplayTag(FName("Audio.Master"), TEXT("Master Volume & Mix Control"));
	Manager.AddNativeGameplayTag(FName("Audio.Music"), TEXT("Music Track Layer"));
	Manager.AddNativeGameplayTag(FName("Audio.SFX"), TEXT("Sound Effects Layer"));
	Manager.AddNativeGameplayTag(FName("Audio.Voice"), TEXT("Voice Over / Dialogue Layer"));
	Manager.AddNativeGameplayTag(FName("Audio.UI"), TEXT("User Interface Sounds"));

	// Optional: Standard Sub-Kategorien für SFX (Best Practice)
	Manager.AddNativeGameplayTag(FName("Audio.SFX.Foley"), TEXT("Physical interactions"));
	Manager.AddNativeGameplayTag(FName("Audio.SFX.Weapon"), TEXT("Weapon sounds"));
	Manager.AddNativeGameplayTag(FName("Audio.SFX.Impact"), TEXT("Impact sounds"));

	UE_LOG(LogTemp, Log, TEXT("Dual Node Audio: Native Gameplay Tags registered."));
}

void FDualNodeAudioModule::ShutdownModule()
{
	// Beim Shutdown müssen wir native Tags normalerweise nicht entfernen, 
	// da der Tag Manager beim Beenden ohnehin bereinigt wird.
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FDualNodeAudioModule, DualNodeAudio)