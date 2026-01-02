#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "Components/AudioComponent.h"
#include "Engine/StreamableManager.h"
#include "DualNodeAudioTypes.h"
#include "DualNodeAudioSubsystem.generated.h"

class ADualNodeMusicManager;
struct FDualNodeAudioRegistry;

DECLARE_DELEGATE_OneParam(FOnSoundLoaded, USoundBase*);

UCLASS()
class DUALNODEAUDIO_API UDualNodeAudioSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;

	// --- REGISTRY API ---
	bool GetSoundDefinition(const FGameplayTag& Tag, FDualNodeSoundDefinition& OutDef) const;
	bool GetMusicDefinition(const FGameplayTag& Tag, FDualNodeMusicDefinition& OutDef) const;
	bool GetPlaylistDefinition(const FGameplayTag& Tag, FDualNodePlaylist& OutDef) const;
	bool GetBarkDefinition(const FGameplayTag& Tag, FDualNodeBarkDefinition& OutDef) const;

	// FIX: Wiederhergestellt für Physics Support
	// Gibt SoftPointer zurück für Async Loading
	TSoftObjectPtr<USoundBase> ResolveSoundFromPhysics(const FDualNodeSoundDefinition& BaseDef, const UPhysicalMaterial* PhysMat) const;

	// --- MIXING API ---
	void SetVolumeByClassTag(FGameplayTag ClassTag, float Volume, float FadeTime = 0.5f);
	void MuteClassTag(FGameplayTag ClassTag, bool bMuted, float FadeTime = 0.5f);
	void PushGlobalMix();

	// --- ASYNC HELPER ---
	void ExecuteWhenSoundLoaded(const TSoftObjectPtr<USoundBase>& SoftSound, FOnSoundLoaded OnLoaded);
	void PreloadSoundGroup(FGameplayTag RootTag);

	// --- PLAY API ---
	void PlaySound2D(const UObject* WorldContext, FGameplayTag Tag, float Vol, float Pitch);
	void PlaySoundAtLocation(const UObject* WorldContext, FGameplayTag Tag, FVector Location, FRotator Rotation, float Vol, float Pitch);
	
	// FIX: Neue Overload für direktes Asset-Abspielen (wichtig für Foley/Physics)
	void PlaySoundAtLocation(const UObject* WorldContext, TSoftObjectPtr<USoundBase> SoundToPlay, FVector Location, FRotator Rotation, float Vol, float Pitch, const FDualNodeSoundDefinition* OptionalDefSettings = nullptr);

	UAudioComponent* SpawnSoundAttached(const UObject* WorldContext, FGameplayTag Tag, USceneComponent* Parent, FName Socket, bool bAutoDestroy);

	// --- MUSIC CONTROL ---
	void SetMusicLayer(EDNAMusicPriority Priority, FGameplayTag MusicTag, const FDualNodePlaybackSettings& Settings, double StartTimestamp = 0.0);
	void ClearMusicLayer(EDNAMusicPriority Priority);
	void PauseMusicLayer(EDNAMusicPriority Priority);
	void ResumeMusicLayer(EDNAMusicPriority Priority);
	void SkipMusicTrack(EDNAMusicPriority Priority);
	
	ADualNodeMusicManager* GetOrSpawnMusicManager(const UObject* WorldContext);
	void SetSystemTimeOfDay(float NewTime);
	bool TryPlayBark(const UObject* WorldContext, FGameplayTag BarkTag, const FVector& Location);

	// --- PERSISTENCE ---
	UFUNCTION(BlueprintCallable, Category = "Dual Node Audio|Persistence")
	FDNASaveData GetStateSnapshot() const;

	UFUNCTION(BlueprintCallable, Category = "Dual Node Audio|Persistence")
	void RestoreState(const FDNASaveData& SaveData);

private:
	void RebuildRegistryCache();
	FStreamableManager StreamableManager;

	UPROPERTY(Transient) TMap<FGameplayTag, FDualNodeSoundDefinition> MergedRegistryCache;
	UPROPERTY(Transient) TMap<FGameplayTag, FDualNodeMusicDefinition> MergedMusicCache;
	UPROPERTY(Transient) TMap<FGameplayTag, FDualNodePlaylist> MergedPlaylistCache;
	UPROPERTY(Transient) TMap<FGameplayTag, FDualNodeBarkDefinition> MergedBarkCache;

	void UpdateMusicSystem();
	
	UPROPERTY() TMap<EDNAMusicPriority, FActiveMusicLayer> ActiveLayers;
	UPROPERTY() TObjectPtr<UAudioComponent> AudioComp_A;
	UPROPERTY() TObjectPtr<UAudioComponent> AudioComp_B;
	bool bIsPlayingA = false;
	FGameplayTag CurrentPlayingTag;

	float CurrentTimeOfDay = 12.0f; 
	
	UPROPERTY() TMap<FGameplayTag, double> BarkCooldowns;
	void PruneBarkCooldowns(double CurrentTime);
};