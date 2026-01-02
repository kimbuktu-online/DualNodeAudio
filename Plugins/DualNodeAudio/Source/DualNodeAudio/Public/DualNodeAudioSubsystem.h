#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "Components/AudioComponent.h"
#include "Engine/StreamableManager.h"
#include "DualNodeAudioTypes.h"
#include "DualNodeAudioRegistry.h"
#include "DualNodeAudioSubsystem.generated.h"

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

	USoundBase* ResolveSoundFromPhysics(const FDualNodeSoundDefinition& BaseDef, const UPhysicalMaterial* PhysMat) const;

	// --- CONTROL API ---
	void SetMusicLayer(EDNAMusicPriority Priority, FGameplayTag MusicTag, const FDualNodePlaybackSettings& Settings, double StartTimestamp = 0.0);
	void ClearMusicLayer(EDNAMusicPriority Priority);
	void PauseMusicLayer(EDNAMusicPriority Priority);
	void ResumeMusicLayer(EDNAMusicPriority Priority);
	void SkipMusicTrack(EDNAMusicPriority Priority);

	void SetSystemTimeOfDay(float NewTime);
	
	// OPTIMIZATION: const FVector&
	bool TryPlayBark(const UObject* WorldContext, FGameplayTag BarkTag, const FVector& Location);

	// --- SAVE & LOAD API ---
	UFUNCTION(BlueprintCallable, Category = "Dual Node Audio|Persistence")
	FDNASaveData GetStateSnapshot() const;

	UFUNCTION(BlueprintCallable, Category = "Dual Node Audio|Persistence")
	void RestoreState(const FDNASaveData& SaveData);

private:
	// --- MEMORY ---
	UPROPERTY(Transient) TMap<FGameplayTag, FDualNodeSoundDefinition> MergedRegistryCache;
	UPROPERTY(Transient) TMap<FGameplayTag, FDualNodeMusicDefinition> MergedMusicCache;
	UPROPERTY(Transient) TMap<FGameplayTag, FDualNodePlaylist> MergedPlaylistCache;
	UPROPERTY(Transient) TMap<FGameplayTag, FDualNodeBarkDefinition> MergedBarkCache;
	void RebuildRegistryCache();

	FStreamableManager StreamableManager;

	// --- STATE ---
	void UpdateMusicSystem();
	void HandleLevelChange();

	UPROPERTY() TMap<EDNAMusicPriority, FActiveMusicLayer> ActiveLayers;
	
	UPROPERTY() TObjectPtr<UAudioComponent> AudioComp_A;
	UPROPERTY() TObjectPtr<UAudioComponent> AudioComp_B;
	bool bIsPlayingA = false;
	FGameplayTag CurrentPlayingTag;

	float CurrentTimeOfDay = 12.0f; 
	TMap<FGameplayTag, double> BarkCooldowns;
};