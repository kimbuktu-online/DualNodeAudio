// Copyright YourCompany. All Rights Reserved.

#include "DualNodeAudio.h"
#include "GameplayTagsManager.h"
#include "Misc/Paths.h"
#include "ShaderCore.h"

#define LOCTEXT_NAMESPACE "FDualNodeAudioModule"

void FDualNodeAudioModule::StartupModule()
{
	UGameplayTagsManager& Manager = UGameplayTagsManager::Get();

	// Root Tag
	Manager.AddNativeGameplayTag(FName("Audio"), TEXT("DNA Root Tag"));

	// Hierarchy Tags
	Manager.AddNativeGameplayTag(FName("Audio.Master"), TEXT("Master Volume & Mix Control"));
	Manager.AddNativeGameplayTag(FName("Audio.Music"), TEXT("Music Track Layer"));
	Manager.AddNativeGameplayTag(FName("Audio.SFX"), TEXT("Sound Effects Layer"));
	Manager.AddNativeGameplayTag(FName("Audio.Voice"), TEXT("Voice Over / Dialogue Layer"));
	Manager.AddNativeGameplayTag(FName("Audio.UI"), TEXT("User Interface Sounds"));

	// Sub-Kategorien (WICHTIG: Hier fehlten die Footstep Tags!)
	Manager.AddNativeGameplayTag(FName("Audio.SFX.Foley"), TEXT("Physical interactions"));
	Manager.AddNativeGameplayTag(FName("Audio.SFX.Weapon"), TEXT("Weapon sounds"));
	Manager.AddNativeGameplayTag(FName("Audio.SFX.Impact"), TEXT("Impact sounds"));
	
	// FIX: Spezifische Tags für Foley Component registrieren
	Manager.AddNativeGameplayTag(FName("Audio.SFX.Footstep"), TEXT("Footstep parent"));
	Manager.AddNativeGameplayTag(FName("Audio.SFX.Footstep.Walk"), TEXT("Walking sounds"));
	Manager.AddNativeGameplayTag(FName("Audio.SFX.Footstep.Sprint"), TEXT("Sprinting sounds"));
	Manager.AddNativeGameplayTag(FName("Audio.SFX.Impact.Land"), TEXT("Landing impact sounds"));

	UE_LOG(LogTemp, Log, TEXT("Dual Node Audio: Native Gameplay Tags registered."));
}

void FDualNodeAudioModule::ShutdownModule()
{
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FDualNodeAudioModule, DualNodeAudio)