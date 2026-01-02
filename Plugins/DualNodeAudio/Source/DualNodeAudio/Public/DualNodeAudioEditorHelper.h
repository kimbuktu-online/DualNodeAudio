// Copyright YourCompany. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "DualNodeAudioEditorHelper.generated.h"

/**
 * Hilfsklasse für den Editor.
 * Generiert automatisch die benötigten Assets (SoundClasses, Mixes) für DNA.
 */
UCLASS()
class DUALNODEAUDIO_API UDualNodeAudioEditorHelper : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	/**
	 * Erstellt die Standard-Assets (/DualNodeAudio/Defaults/...), 
	 * die in den Project Settings referenziert werden.
	 */
	UFUNCTION(BlueprintCallable, Category = "Dual Node Audio|Editor")
	static void GenerateDefaultAudioAssets();
};