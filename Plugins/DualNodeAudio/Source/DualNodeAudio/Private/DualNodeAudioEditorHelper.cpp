// Copyright YourCompany. All Rights Reserved.

#include "DualNodeAudioEditorHelper.h"

#if WITH_EDITOR
#include "AssetToolsModule.h"
#include "Factories/SoundClassFactory.h"
#include "Factories/SoundMixFactory.h"
#include "Sound/SoundClass.h"
#include "Sound/SoundMix.h"
#include "UObject/SavePackage.h"
#include "Misc/PackageName.h"
#include "FileHelpers.h"
#endif

// ZIEL: Content/DualNode/Audio/Defaults/
const FString DNA_GENERATION_PATH = TEXT("/Game/DualNode/Audio/Defaults"); 

void UDualNodeAudioEditorHelper::GenerateDefaultAudioAssets()
{
#if WITH_EDITOR
	FAssetToolsModule& AssetToolsModule = FModuleManager::Get().LoadModuleChecked<FAssetToolsModule>("AssetTools");

	// HINWEIS: Wir prüfen nicht mehr auf Plugin-Validität, da /Game/ immer existiert.
	
	TArray<USoundClass*> CreatedClasses;

	// Helper Lambda
	auto CreateSoundClass = [&](FString Name, USoundClass* Parent) -> USoundClass*
	{
		FString PackageName = DNA_GENERATION_PATH + "/" + Name;
		USoundClass* ResultSC = nullptr;

        if (FPackageName::DoesPackageExist(PackageName))
        {
            ResultSC = LoadObject<USoundClass>(nullptr, * (PackageName + "." + Name));
        }
		else
		{
			// AssetTools erstellt automatisch fehlende Unterordner (DualNode/Audio/...)
			USoundClassFactory* Factory = NewObject<USoundClassFactory>();
			UObject* NewAsset = AssetToolsModule.Get().CreateAsset(Name, DNA_GENERATION_PATH, USoundClass::StaticClass(), Factory);
			ResultSC = Cast<USoundClass>(NewAsset);
		}
		
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
			CreatedClasses.Add(ResultSC);
			return ResultSC;
		}
		return nullptr;
	};

	// 1. Hierarchie erstellen
	USoundClass* SC_Master = CreateSoundClass("SC_Master", nullptr);
	USoundClass* SC_Music  = CreateSoundClass("SC_Music", SC_Master);
	USoundClass* SC_SFX    = CreateSoundClass("SC_SFX", SC_Master);
	USoundClass* SC_Voice  = CreateSoundClass("SC_Voice", SC_Master);
    USoundClass* SC_UI     = CreateSoundClass("SC_UI", SC_SFX); 

	// 2. Global Mix erstellen
    FString MixName = "Mix_GlobalMaster";
    FString MixPackageName = DNA_GENERATION_PATH + "/" + MixName;
	USoundMix* Mix = nullptr;
    
    if (FPackageName::DoesPackageExist(MixPackageName))
    {
		Mix = LoadObject<USoundMix>(nullptr, * (MixPackageName + "." + MixName));
	}
	else
	{
        USoundMixFactory* MixFactory = NewObject<USoundMixFactory>();
        UObject* NewMix = AssetToolsModule.Get().CreateAsset(MixName, DNA_GENERATION_PATH, USoundMix::StaticClass(), MixFactory);
		Mix = Cast<USoundMix>(NewMix);
	}

    if (Mix)
    {
        Mix->bApplyEQ = false; 
		// FIX: Zeile gelöscht, da Property nicht in USoundMix existiert.
		// Die Logik wird unten in 'NewAdj.bApplyToChildren' behandelt.
		Mix->Duration = -1.0f;

		for (USoundClass* SC : CreatedClasses)
		{
			bool bFound = false;
			for (const FSoundClassAdjuster& Adj : Mix->SoundClassEffects)
			{
				if (Adj.SoundClassObject == SC) { bFound = true; break; }
			}

			if (!bFound)
			{
				FSoundClassAdjuster NewAdj;
				NewAdj.SoundClassObject = SC;
				NewAdj.VolumeAdjuster = 1.0f;
				NewAdj.PitchAdjuster = 1.0f;
				
				// HIER ist der korrekte Ort für die Property:
				NewAdj.bApplyToChildren = true;
				
				Mix->SoundClassEffects.Add(NewAdj);
			}
		}
        Mix->MarkPackageDirty();
    }

	// 3. Speichern
	TArray<UPackage*> PackagesToSave;
	for (USoundClass* SC : CreatedClasses) if (SC) PackagesToSave.Add(SC->GetPackage());
	if (Mix) PackagesToSave.Add(Mix->GetPackage());
	
	if (PackagesToSave.Num() > 0)
	{
		FEditorFileUtils::PromptForCheckoutAndSave(PackagesToSave, false, false);
	}

	UE_LOG(LogTemp, Display, TEXT("DNA: Default Assets verified at Project Location: %s"), *DNA_GENERATION_PATH);
#endif
}