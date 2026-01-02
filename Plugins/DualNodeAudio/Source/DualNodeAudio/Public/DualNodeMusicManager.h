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

	// --- SFX RPCs (Bleiben RPCs, da "Fire-and-Forget") ---
	UFUNCTION(NetMulticast, Unreliable)
	void MulticastPlaySoundAtLocation(FGameplayTag SoundTag, FVector Location, FRotator Rotation, float VolumeMult, float PitchMult);

	UFUNCTION(NetMulticast, Reliable)
	void MulticastPlaySoundAtLocation_Reliable(FGameplayTag SoundTag, FVector Location, FRotator Rotation, float VolumeMult, float PitchMult);

	// --- GLOBAL MUSIC STATE (Neu: Replicated Variable) ---
	
	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category="Dual Node Audio|Music")
	void Server_SetGlobalMusic(EDNAMusicPriority Priority, FGameplayTag Tag, FDualNodePlaybackSettings Settings);

	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category="Dual Node Audio|Music")
	void Server_ClearGlobalMusic(EDNAMusicPriority Priority);

	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category="Dual Node Audio|Music")
	void Server_PauseMusic(EDNAMusicPriority Priority);

	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category="Dual Node Audio|Music")
	void Server_ResumeMusic(EDNAMusicPriority Priority);

protected:
	// Array of Replicated States (Index = Priority)
	// Wir nutzen ein fixes Array für die Prioritäten 1-5, um Overhead zu sparen.
	// Index 0 ist Dummy.
	UPROPERTY(ReplicatedUsing = OnRep_MusicStates)
	TArray<FReplicatedMusicState> ActiveMusicStates;

	UFUNCTION()
	void OnRep_MusicStates();

	void PlayLocalSound(FGameplayTag Tag, FVector Location, FRotator Rotation, float Volume, float Pitch);
};