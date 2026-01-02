#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DualNodeAudioTypes.h"
#include "DualNodeMusicManager.generated.h"

UCLASS()
class DUALNODEAUDIO_API ADualNodeMusicManager : public AActor
{
	GENERATED_BODY()
	
public:	
	ADualNodeMusicManager();

	// --- SFX RPCs ---
	UFUNCTION(NetMulticast, Unreliable)
	void MulticastPlaySoundAtLocation(FGameplayTag SoundTag, FVector Location, FRotator Rotation, float VolumeMult, float PitchMult);

	UFUNCTION(NetMulticast, Reliable)
	void MulticastPlaySoundAtLocation_Reliable(FGameplayTag SoundTag, FVector Location, FRotator Rotation, float VolumeMult, float PitchMult);

	// --- GLOBAL MUSIC RPCs ---
	
	// FIX: Settings Parameter hinzugefügt
	UFUNCTION(NetMulticast, Reliable)
	void Multicast_SetMusicLayer(EDNAMusicPriority Priority, FGameplayTag MusicTag, FDualNodePlaybackSettings Settings, double ServerTimestamp);

	UFUNCTION(NetMulticast, Reliable)
	void Multicast_ClearMusicLayer(EDNAMusicPriority Priority);

	// FIX: Neue Pause/Resume RPCs hinzugefügt
	UFUNCTION(NetMulticast, Reliable)
	void Multicast_PauseMusicLayer(EDNAMusicPriority Priority);

	UFUNCTION(NetMulticast, Reliable)
	void Multicast_ResumeMusicLayer(EDNAMusicPriority Priority);

	UFUNCTION(NetMulticast, Reliable)
	void Multicast_SkipMusicTrack(EDNAMusicPriority Priority);

	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category="Dual Node Audio|Music")
	void Server_SkipMusicTrack(EDNAMusicPriority Priority);

	// Server Helper für Blueprints
	// FIX: Settings Parameter hinzugefügt
	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category="Dual Node Audio|Music")
	void Server_SetGlobalMusic(EDNAMusicPriority Priority, FGameplayTag Tag, FDualNodePlaybackSettings Settings);

	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category="Dual Node Audio|Music")
	void Server_ClearGlobalMusic(EDNAMusicPriority Priority);

protected:
	void PlayLocalSound(FGameplayTag Tag, FVector Location, FRotator Rotation, float Volume, float Pitch);
};