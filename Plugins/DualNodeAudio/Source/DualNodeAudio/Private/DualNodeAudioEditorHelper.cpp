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
#endif

void UDualNodeAudioEditorHelper::GenerateDefaultAudioAssets()
{
#if WITH_EDITOR
	FAssetToolsModule& AssetToolsModule = FModuleManager::Get().LoadModuleChecked<FAssetToolsModule>("AssetTools");

	const FString PluginContentPath = TEXT("/DualNodeAudio/Defaults/Mixing"); 

	auto CreateSoundClass = [&](FString Name, USoundClass* Parent) -> USoundClass*
	{
		FString PackageName = PluginContentPath + "/" + Name;
        if (FPackageName::DoesPackageExist(PackageName))
        {
            return LoadObject<USoundClass>(nullptr, * (PackageName + "." + Name));
        }

		USoundClassFactory* Factory = NewObject<USoundClassFactory>();
		UObject* NewAsset = AssetToolsModule.Get().CreateAsset(Name, PluginContentPath, USoundClass::StaticClass(), Factory);
		
		if (USoundClass* SC = Cast<USoundClass>(NewAsset))
		{
			if (Parent)
			{
				SC->ParentClass = Parent;
			}
            SC->MarkPackageDirty();
			return SC;
		}
		return nullptr;
	};

	// Create Classes
	USoundClass* SC_Master = CreateSoundClass("SC_Master", nullptr);
	USoundClass* SC_Music = CreateSoundClass("SC_Music", SC_Master);
	USoundClass* SC_SFX   = CreateSoundClass("SC_SFX", SC_Master);
	USoundClass* SC_Voice = CreateSoundClass("SC_Voice", SC_Master);
    CreateSoundClass("SC_UI", SC_SFX); 

	// Create Global Mix
    FString MixName = "Mix_GlobalMaster";
    FString MixPackageName = PluginContentPath + "/" + MixName;
    
    if (!FPackageName::DoesPackageExist(MixPackageName))
    {
        USoundMixFactory* MixFactory = NewObject<USoundMixFactory>();
        UObject* NewMix = AssetToolsModule.Get().CreateAsset(MixName, PluginContentPath, USoundMix::StaticClass(), MixFactory);
        
        if (USoundMix* Mix = Cast<USoundMix>(NewMix))
        {
            Mix->bApplyEQ = true;
        	// FIX C2039: Wir entfernen die EQ-Zuweisung, da sie optional ist und Versionskonflikte verursacht.
            // Der Mix funktioniert auch mit Default-EQ Werten.
            Mix->MarkPackageDirty();
        }
    }

	UE_LOG(LogTemp, Display, TEXT("DNA: Default Assets generated at %s"), *PluginContentPath);
#endif
}