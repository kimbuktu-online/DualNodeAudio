#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "DualNodeAudioTypes.h"
#include "DualNodeAudioRegistry.generated.h"

UCLASS(BlueprintType)
class DUALNODEAUDIO_API UDualNodeAudioRegistry : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Registry", meta=(ForceInlineRow))
	TMap<FGameplayTag, FDualNodeSoundDefinition> SoundDatabase;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Registry", meta=(ForceInlineRow))
	TMap<FGameplayTag, FDualNodeMusicDefinition> MusicDatabase;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Registry", meta=(ForceInlineRow))
	TMap<FGameplayTag, FDualNodePlaylist> PlaylistDatabase;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Registry", meta=(ForceInlineRow))
	TMap<FGameplayTag, FDualNodeBarkDefinition> BarkDatabase;
};