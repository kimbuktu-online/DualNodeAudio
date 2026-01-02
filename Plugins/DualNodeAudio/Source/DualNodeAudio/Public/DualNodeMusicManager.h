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

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	// --- SFX RPCs ---
	UFUNCTION(NetMulticast, Unreliable)
	void MulticastPlaySoundAtLocation(FGameplayTag SoundTag, FVector Location, FRotator Rotation, float VolumeMult, float PitchMult);

	UFUNCTION(NetMulticast, Reliable)
	void MulticastPlaySoundAtLocation_Reliable(FGameplayTag SoundTag, FVector Location, FRotator Rotation, float VolumeMult, float PitchMult);

	// --- MUSIC CONTROL ---
	
	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category="Dual Node Audio|Music")
	void Server_SetGlobalMusic(EDNAMusicPriority Priority, FGameplayTag Tag, FDualNodePlaybackSettings Settings);

	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category="Dual Node Audio|Music")
	void Server_ClearGlobalMusic(EDNAMusicPriority Priority);

	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category="Dual Node Audio|Music")
	void Server_PauseMusic(EDNAMusicPriority Priority);

	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category="Dual Node Audio|Music")
	void Server_ResumeMusic(EDNAMusicPriority Priority);

	// FIX: Hinzugefügt für Library Error C2039
	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category="Dual Node Audio|Music")
	void Server_SkipMusicTrack(EDNAMusicPriority Priority);

	// Interne Multicast für den Skip-Befehl
	UFUNCTION(NetMulticast, Reliable)
	void Multicast_SkipMusicTrack(EDNAMusicPriority Priority);

protected:
	UPROPERTY(ReplicatedUsing = OnRep_MusicStates)
	TArray<FReplicatedMusicState> ActiveMusicStates;

	UFUNCTION()
	void OnRep_MusicStates();

	void PlayLocalSound(FGameplayTag Tag, FVector Location, FRotator Rotation, float Volume, float Pitch);
};