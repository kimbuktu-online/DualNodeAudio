#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "DualNodeAudioTypes.h"
#include "Components/AudioComponent.h"
#include "DualNodeAudioLibrary.generated.h"

class APlayerController;

/**
 * DNA 13.0 Library
 */
UCLASS()
class DUALNODEAUDIO_API UDualNodeAudioLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:

	// ==========================================================================================
	// MIXING (V13.0)
	// ==========================================================================================
	
	/**
	 * Sets the volume for a specific Class Tag defined in Project Settings.
	 * @param ClassTag The Gameplay Tag mapped to a SoundClass (e.g. "Audio.Music").
	 * @param Volume 0.0 to 1.0 (or higher).
	 */
	UFUNCTION(BlueprintCallable, Category = "Dual Node Audio|Mixing", meta = (WorldContext = "WorldContextObject"))
	static void SetVolumeByClassTag(const UObject* WorldContextObject, FGameplayTag ClassTag, float Volume, float FadeTime = 0.5f);

	/**
	 * Mutes or Unmutes a Class Tag.
	 */
	UFUNCTION(BlueprintCallable, Category = "Dual Node Audio|Mixing", meta = (WorldContext = "WorldContextObject"))
	static void MuteClassTag(const UObject* WorldContextObject, FGameplayTag ClassTag, bool bMuted, float FadeTime = 0.5f);

	// ==========================================================================================
	// OPTIMIZATION & PRELOADING
	// ==========================================================================================

	UFUNCTION(BlueprintCallable, Category = "Dual Node Audio|Optimization", meta = (WorldContext = "WorldContextObject"))
	static void PreloadSoundGroup(const UObject* WorldContextObject, FGameplayTag RootTag);

	UFUNCTION(BlueprintCallable, Category = "Dual Node Audio|Debug", meta = (WorldContext = "WorldContextObject"))
	static void DebugAudioState(const UObject* WorldContextObject);

	// ==========================================================================================
	// SFX & ONE-SHOTS
	// ==========================================================================================

	UFUNCTION(BlueprintCallable, Category = "Dual Node Audio|Local", meta = (WorldContext = "WorldContextObject"))
	static void PlaySoundByTag_2D(const UObject* WorldContextObject, FGameplayTag Tag, float VolumeMultiplier = 1.0f, float PitchMultiplier = 1.0f);

	UFUNCTION(BlueprintCallable, Category = "Dual Node Audio|Local", meta = (WorldContext = "WorldContextObject"))
	static void PlaySoundByTag_AtLocation(const UObject* WorldContextObject, FGameplayTag Tag, const FVector& Location, FRotator Rotation = FRotator::ZeroRotator, float VolumeMultiplier = 1.0f, float PitchMultiplier = 1.0f);

	UFUNCTION(BlueprintCallable, Category = "Dual Node Audio|Physics", meta = (WorldContext = "WorldContextObject"))
	static void PlaySoundByTag_WithPhysics(const UObject* WorldContextObject, FGameplayTag Tag, FHitResult Hit, float VolumeMultiplier = 1.0f, float PitchMultiplier = 1.0f);

	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category = "Dual Node Audio|Network", meta = (WorldContext = "WorldContextObject"))
	static void BroadcastSoundByTag_AtLocation(const UObject* WorldContextObject, FGameplayTag Tag, const FVector& Location, FRotator Rotation = FRotator::ZeroRotator, bool bReliable = false);

	UFUNCTION(BlueprintCallable, Category = "Dual Node Audio|Components", meta = (WorldContext = "WorldContextObject"))
	static UAudioComponent* SpawnSoundByTag_Attached(const UObject* WorldContextObject, FGameplayTag Tag, USceneComponent* AttachToComponent, FName SocketName = NAME_None, bool bAutoDestroy = true);

	UFUNCTION(BlueprintCallable, Category = "Dual Node Audio|AI", meta = (WorldContext = "WorldContextObject"))
	static bool PlayBarkByTag(const UObject* WorldContextObject, FGameplayTag BarkTag, const FVector& Location);

	// ==========================================================================================
	// GLOBAL MUSIC (SERVER AUTHORITY)
	// ==========================================================================================

	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category = "Dual Node Audio|Music", meta = (WorldContext = "WorldContextObject"))
	static void SetGlobalMusic(const UObject* WorldContextObject, EDNAMusicPriority Priority, FGameplayTag MusicTag, FDualNodePlaybackSettings Settings);

	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category = "Dual Node Audio|Music", meta = (WorldContext = "WorldContextObject"))
	static void ClearGlobalMusic(const UObject* WorldContextObject, EDNAMusicPriority Priority);

	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category = "Dual Node Audio|Music", meta = (WorldContext = "WorldContextObject"))
	static void PauseGlobalMusic(const UObject* WorldContextObject, EDNAMusicPriority Priority);

	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category = "Dual Node Audio|Music", meta = (WorldContext = "WorldContextObject"))
	static void ResumeGlobalMusic(const UObject* WorldContextObject, EDNAMusicPriority Priority);

	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category = "Dual Node Audio|Music", meta = (WorldContext = "WorldContextObject"))
	static void SkipGlobalMusicTrack(const UObject* WorldContextObject, EDNAMusicPriority Priority);

	// ==========================================================================================
	// LOCAL MUSIC
	// ==========================================================================================

	UFUNCTION(BlueprintCallable, Category = "Dual Node Audio|Music", meta = (WorldContext = "WorldContextObject"))
	static void SetLocalMusic(const UObject* WorldContextObject, EDNAMusicPriority Priority, FGameplayTag MusicTag, FDualNodePlaybackSettings Settings);

	UFUNCTION(BlueprintCallable, Category = "Dual Node Audio|Music", meta = (WorldContext = "WorldContextObject"))
	static void ClearLocalMusic(const UObject* WorldContextObject, EDNAMusicPriority Priority);

	UFUNCTION(BlueprintCallable, Category = "Dual Node Audio|Music", meta = (WorldContext = "WorldContextObject"))
	static void SkipLocalMusicTrack(const UObject* WorldContextObject, EDNAMusicPriority Priority);

	// ==========================================================================================
	// TARGETED MUSIC
	// ==========================================================================================

	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category = "Dual Node Audio|Music")
	static void ClientMusicPush(APlayerController* TargetPlayer, EDNAMusicPriority Priority, FGameplayTag MusicTag, FDualNodePlaybackSettings Settings);
	
	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category = "Dual Node Audio|Music")
	static void ClientMusicClear(APlayerController* TargetPlayer, EDNAMusicPriority Priority);

	// ==========================================================================================
	// WORLD
	// ==========================================================================================

	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category = "Dual Node Audio|World", meta = (WorldContext = "WorldContextObject"))
	static void SetSystemTimeOfDay(const UObject* WorldContextObject, float Hour);
};