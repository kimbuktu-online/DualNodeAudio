// Copyright YourCompany. All Rights Reserved.

#include "DualNodeAudio.h"
#include "GameplayTagsManager.h"
#include "Misc/Paths.h"
#include "DualNodeAudioSettings.h" // <--- NEU: Wichtig für den Zugriff auf die Config

#if WITH_EDITOR
#include "Misc/CoreDelegates.h"
#include "AssetToolsModule.h"
#include "Factories/SoundClassFactory.h"
#include "Factories/SoundMixFactory.h"
#include "Sound/SoundClass.h"
#include "Sound/SoundMix.h"
#include "FileHelpers.h"
#include "Misc/PackageName.h"
#endif

#define LOCTEXT_NAMESPACE "FDualNodeAudioModule"

void FDualNodeAudioModule::StartupModule()
{
	// 1. Tags registrieren
	UGameplayTagsManager& Manager = UGameplayTagsManager::Get();

	Manager.AddNativeGameplayTag(FName("Audio"), TEXT("DNA Root Tag"));
	Manager.AddNativeGameplayTag(FName("Audio.Master"), TEXT("Master Volume"));
	Manager.AddNativeGameplayTag(FName("Audio.Music"), TEXT("Music Layer"));
	Manager.AddNativeGameplayTag(FName("Audio.SFX"), TEXT("SFX Layer"));
	Manager.AddNativeGameplayTag(FName("Audio.Voice"), TEXT("Voice Layer"));
	Manager.AddNativeGameplayTag(FName("Audio.UI"), TEXT("UI Layer"));

	Manager.AddNativeGameplayTag(FName("Audio.SFX.Foley"), TEXT("Material Independent Movement"));
	Manager.AddNativeGameplayTag(FName("Audio.SFX.Weapon"), TEXT("Weapon sounds"));
	Manager.AddNativeGameplayTag(FName("Audio.SFX.Impact"), TEXT("Impact sounds"));
	
	Manager.AddNativeGameplayTag(FName("Audio.SFX.Footstep"), TEXT("Footstep Parent"));
	Manager.AddNativeGameplayTag(FName("Audio.SFX.Footstep.Walk"), TEXT("Walk Steps"));
	Manager.AddNativeGameplayTag(FName("Audio.SFX.Footstep.Sprint"), TEXT("Sprint Steps"));
	Manager.AddNativeGameplayTag(FName("Audio.SFX.Impact.Land"), TEXT("Landing Impact"));

	UE_LOG(LogTemp, Log, TEXT("Dual Node Audio: Gameplay Tags registered."));

	// 2. Assets & Settings Generieren (Nur im Editor)
#if WITH_EDITOR
	FCoreDelegates::OnFEngineLoopInitComplete.AddRaw(this, &FDualNodeAudioModule::CheckAndGenerateAssets);
#endif
}

void FDualNodeAudioModule::ShutdownModule()
{
#if WITH_EDITOR
	FCoreDelegates::OnFEngineLoopInitComplete.RemoveAll(this);
#endif
}

#if WITH_EDITOR
void FDualNodeAudioModule::CheckAndGenerateAssets()
{
	const FString GenerationPath = TEXT("/Game/DualNode/Audio/Defaults");
	FAssetToolsModule& AssetToolsModule = FModuleManager::Get().LoadModuleChecked<FAssetToolsModule>("AssetTools");
	TArray<USoundClass*> CreatedClasses;

	// --- STEP A: Assets erstellen/laden ---

	auto GetOrCreateSoundClass = [&](FString Name, USoundClass* Parent) -> USoundClass*
	{
		FString PackageName = GenerationPath + "/" + Name;
		if (FPackageName::DoesPackageExist(PackageName))
		{
			return LoadObject<USoundClass>(nullptr, * (PackageName + "." + Name));
		}

		USoundClassFactory* Factory = NewObject<USoundClassFactory>();
		UObject* NewAsset = AssetToolsModule.Get().CreateAsset(Name, GenerationPath, USoundClass::StaticClass(), Factory);
		USoundClass* ResultSC = Cast<USoundClass>(NewAsset);

		if (ResultSC)
		{
			if (Parent)
			{
				ResultSC->ParentClass = Parent;
				if (!Parent->ChildClasses.Contains(ResultSC))
				{
					Parent->ChildClasses.Add(ResultSC);
					Parent->MarkPackageDirty();
				}
			}
			ResultSC->MarkPackageDirty();
			UE_LOG(LogTemp, Log, TEXT("DNA: Created missing SoundClass %s"), *Name);
		}
		return ResultSC;
	};

	USoundClass* SC_Master = GetOrCreateSoundClass("SC_Master", nullptr);
	USoundClass* SC_Music = nullptr;
	USoundClass* SC_SFX = nullptr;
	USoundClass* SC_Voice = nullptr;
	USoundClass* SC_UI = nullptr;

	if (SC_Master)
	{
		SC_Music = GetOrCreateSoundClass("SC_Music", SC_Master);
		SC_SFX   = GetOrCreateSoundClass("SC_SFX", SC_Master);
		SC_Voice = GetOrCreateSoundClass("SC_Voice", SC_Master);
		SC_UI    = GetOrCreateSoundClass("SC_UI", SC_SFX); 
		
		CreatedClasses.Add(SC_Master);
		if (SC_Music) CreatedClasses.Add(SC_Music);
		if (SC_SFX) CreatedClasses.Add(SC_SFX);
		if (SC_Voice) CreatedClasses.Add(SC_Voice);
		if (SC_UI) CreatedClasses.Add(SC_UI);
	}

	FString MixName = "Mix_GlobalMaster";
	FString MixPackageName = GenerationPath + "/" + MixName;
	USoundMix* GlobalMix = nullptr;

	if (FPackageName::DoesPackageExist(MixPackageName))
	{
		GlobalMix = LoadObject<USoundMix>(nullptr, * (MixPackageName + "." + MixName));
	}
	else
	{
		USoundMixFactory* MixFactory = NewObject<USoundMixFactory>();
		UObject* NewMix = AssetToolsModule.Get().CreateAsset(MixName, GenerationPath, USoundMix::StaticClass(), MixFactory);
		GlobalMix = Cast<USoundMix>(NewMix);
		
		if (GlobalMix)
		{
			for (USoundClass* SC : CreatedClasses)
			{
				FSoundClassAdjuster Adjuster;
				Adjuster.SoundClassObject = SC;
				Adjuster.VolumeAdjuster = 1.0f;
				Adjuster.PitchAdjuster = 1.0f;
				Adjuster.bApplyToChildren = true;
				GlobalMix->SoundClassEffects.Add(Adjuster);
			}
			GlobalMix->MarkPackageDirty();
			UE_LOG(LogTemp, Log, TEXT("DNA: Created missing SoundMix %s"), *MixName);
		}
	}

	// Speichern der Assets
	if (CreatedClasses.Num() > 0 || GlobalMix)
	{
		TArray<UPackage*> PackagesToSave;
		FEditorFileUtils::GetDirtyContentPackages(PackagesToSave);
		if (PackagesToSave.Num() > 0)
		{
			FEditorFileUtils::SaveDirtyPackages(false, false, true, false, false);
		}
	}

	// --- STEP B: Auto-Link in Project Settings (NEU) ---
	
	UDualNodeAudioSettings* Settings = GetMutableDefault<UDualNodeAudioSettings>();
	bool bSettingsChanged = false;

	// 1. Global Mix verlinken (nur wenn leer)
	if (Settings->GlobalSoundMix.IsNull() && GlobalMix)
	{
		Settings->GlobalSoundMix = GlobalMix;
		bSettingsChanged = true;
	}

	// 2. Tags zu SoundClasses mappen (nur wenn Eintrag fehlt oder leer ist)
	auto LinkTagToClass = [&](FName TagName, USoundClass* ClassAsset)
	{
		if (!ClassAsset) return;
		FGameplayTag Tag = FGameplayTag::RequestGameplayTag(TagName);
		
		// Wenn der Key nicht existiert ODER der Wert Null ist -> Setzen.
		// Wenn der User da schon was anderes eingestellt hat -> Finger weg.
		if (!Settings->TagToSoundClassDefaults.Contains(Tag) || Settings->TagToSoundClassDefaults[Tag].IsNull())
		{
			Settings->TagToSoundClassDefaults.Add(Tag, ClassAsset);
			bSettingsChanged = true;
		}
	};

	LinkTagToClass(FName("Audio.Master"), SC_Master);
	LinkTagToClass(FName("Audio.Music"), SC_Music);
	LinkTagToClass(FName("Audio.SFX"), SC_SFX);
	LinkTagToClass(FName("Audio.Voice"), SC_Voice);
	LinkTagToClass(FName("Audio.UI"), SC_UI);

	// 3. Settings speichern
	if (bSettingsChanged)
	{
		Settings->TryUpdateDefaultConfigFile(); // Schreibt in DefaultGame.ini
		UE_LOG(LogTemp, Log, TEXT("DNA: Auto-configured Project Settings with default assets."));
	}
}
#endif

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FDualNodeAudioModule, DualNodeAudio)