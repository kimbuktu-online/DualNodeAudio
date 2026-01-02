#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "Components/AudioComponent.h"
#include "Engine/StreamableManager.h"
#include "DualNodeAudioTypes.h"
#include "DualNodeAudioSubsystem.generated.h"

class ADualNodeMusicManager;
struct FDualNodeAudioRegistry;

// Delegate für Async Loading
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

	// --- ASYNC HELPER ---
	void ExecuteWhenSoundLoaded(const TSoftObjectPtr<USoundBase>& SoftSound, FOnSoundLoaded OnLoaded);
	
	// FIX: Diese Funktion fehlte in der Deklaration
	void PreloadSoundGroup(FGameplayTag RootTag);

	// --- PLAY API (Async Safe) ---
	void PlaySound2D(const UObject* WorldContext, FGameplayTag Tag, float Vol, float Pitch);
	void PlaySoundAtLocation(const UObject* WorldContext, FGameplayTag Tag, FVector Location, FRotator Rotation, float Vol, float Pitch);
	UAudioComponent* SpawnSoundAttached(const UObject* WorldContext, FGameplayTag Tag, USceneComponent* Parent, FName Socket, bool bAutoDestroy);

	// --- MUSIC CONTROL ---
	void SetMusicLayer(EDNAMusicPriority Priority, FGameplayTag MusicTag, const FDualNodePlaybackSettings& Settings, double StartTimestamp = 0.0);
	void ClearMusicLayer(EDNAMusicPriority Priority);
	void PauseMusicLayer(EDNAMusicPriority Priority);
	void ResumeMusicLayer(EDNAMusicPriority Priority);
	
	// FIX: Diese Funktion fehlte in der Deklaration
	void SkipMusicTrack(EDNAMusicPriority Priority);
	
	// Server Helper: Findet oder spawnt den Manager
	ADualNodeMusicManager* GetOrSpawnMusicManager(const UObject* WorldContext);

	void SetSystemTimeOfDay(float NewTime);
	bool TryPlayBark(const UObject* WorldContext, FGameplayTag BarkTag, const FVector& Location);

	// --- PERSISTENCE ---
	UFUNCTION(BlueprintCallable, Category = "Dual Node Audio|Persistence")
	FDNASaveData GetStateSnapshot() const;

	UFUNCTION(BlueprintCallable, Category = "Dual Node Audio|Persistence")
	void RestoreState(const FDNASaveData& SaveData);

private:
	// --- CORE ---
	void RebuildRegistryCache();
	
	// FIX: StreamableManager muss hier deklariert sein
	FStreamableManager StreamableManager;

	UPROPERTY(Transient) TMap<FGameplayTag, FDualNodeSoundDefinition> MergedRegistryCache;
	UPROPERTY(Transient) TMap<FGameplayTag, FDualNodeMusicDefinition> MergedMusicCache;
	UPROPERTY(Transient) TMap<FGameplayTag, FDualNodePlaylist> MergedPlaylistCache;
	UPROPERTY(Transient) TMap<FGameplayTag, FDualNodeBarkDefinition> MergedBarkCache;

	// --- MUSIC STATE ---
	void UpdateMusicSystem();
	
	UPROPERTY() TMap<EDNAMusicPriority, FActiveMusicLayer> ActiveLayers;
	UPROPERTY() TObjectPtr<UAudioComponent> AudioComp_A;
	UPROPERTY() TObjectPtr<UAudioComponent> AudioComp_B;
	bool bIsPlayingA = false;
	FGameplayTag CurrentPlayingTag;

	float CurrentTimeOfDay = 12.0f; 
	
	// --- BARK MANAGEMENT ---
	UPROPERTY() TMap<FGameplayTag, double> BarkCooldowns;
	void PruneBarkCooldowns(double CurrentTime);
};