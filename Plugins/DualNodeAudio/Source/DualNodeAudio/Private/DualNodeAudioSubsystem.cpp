#include "DualNodeAudioSubsystem.h"
#include "DualNodeAudioSettings.h"
#include "Engine/AssetManager.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/World.h"

// --- HELPER ---
// Static linkage to avoid collisions
static FDualNodeMusicDefinition ApplyOverrides(const FDualNodeMusicDefinition& BaseDef, const FDualNodePlaybackSettings& Overrides)
{
	FDualNodeMusicDefinition Final = BaseDef;
	if (Overrides.FadeInDuration >= 0.f) Final.FadeInDuration = Overrides.FadeInDuration;
	if (Overrides.FadeOutDuration >= 0.f) Final.FadeOutDuration = Overrides.FadeOutDuration;
	if (Overrides.VolumeMultiplier >= 0.f) Final.VolumeMultiplier = Overrides.VolumeMultiplier;
	if (Overrides.PitchMultiplier >= 0.f) Final.PitchMultiplier = Overrides.PitchMultiplier;
	if (Overrides.StartTime >= 0.f) Final.StartTime = Overrides.StartTime;
	return Final;
}

static USoundBase* LoadSoundSync(const TSoftObjectPtr<USoundBase>& SoftSound)
{
	if (SoftSound.IsNull()) return nullptr;
	if (USoundBase* Loaded = SoftSound.Get()) return Loaded; 
	return SoftSound.LoadSynchronous(); 
}

// --- LIFECYCLE ---

void UDualNodeAudioSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
	RebuildRegistryCache();
}

void UDualNodeAudioSubsystem::Deinitialize()
{
	Super::Deinitialize();
}

void UDualNodeAudioSubsystem::RebuildRegistryCache()
{
	MergedRegistryCache.Reset();
	MergedMusicCache.Reset();
	MergedPlaylistCache.Reset();
	MergedBarkCache.Reset();

	const UDualNodeAudioSettings* Settings = UDualNodeAudioSettings::Get();
	if (!Settings) return;

	for (const TSoftObjectPtr<UDualNodeAudioRegistry>& RegistryPtr : Settings->Registries)
	{
		if (UDualNodeAudioRegistry* Registry = RegistryPtr.LoadSynchronous())
		{
			MergedRegistryCache.Append(Registry->SoundDatabase);
			MergedMusicCache.Append(Registry->MusicDatabase);
			MergedPlaylistCache.Append(Registry->PlaylistDatabase);
			MergedBarkCache.Append(Registry->BarkDatabase);
		}
	}
}

// --- API IMPLEMENTATIONS ---

bool UDualNodeAudioSubsystem::GetSoundDefinition(const FGameplayTag& Tag, FDualNodeSoundDefinition& OutDef) const
{
	if (!Tag.IsValid()) return false;
	// LINTER FIX: Variable moved to if-statement
	if (const FDualNodeSoundDefinition* FoundDef = MergedRegistryCache.Find(Tag))
	{
		OutDef = *FoundDef;
		return true;
	}
	return false;
}

USoundBase* UDualNodeAudioSubsystem::ResolveSoundFromPhysics(const FDualNodeSoundDefinition& BaseDef, const UPhysicalMaterial* PhysMat) const
{
	if (PhysMat && BaseDef.PhysicsMap.Contains(PhysMat))
	{
		return LoadSoundSync(BaseDef.PhysicsMap[PhysMat]);
	}
	return LoadSoundSync(BaseDef.Sound);
}

bool UDualNodeAudioSubsystem::GetMusicDefinition(const FGameplayTag& Tag, FDualNodeMusicDefinition& OutDef) const
{
	if (!Tag.IsValid()) return false;
	// LINTER FIX: Variable moved to if-statement
	if (const FDualNodeMusicDefinition* FoundDef = MergedMusicCache.Find(Tag))
	{
		OutDef = *FoundDef;
		return true;
	}
	return false;
}

bool UDualNodeAudioSubsystem::GetPlaylistDefinition(const FGameplayTag& Tag, FDualNodePlaylist& OutDef) const
{
	if (!Tag.IsValid()) return false;
	// LINTER FIX: Variable moved to if-statement
	if (const FDualNodePlaylist* FoundDef = MergedPlaylistCache.Find(Tag))
	{
		OutDef = *FoundDef;
		return true;
	}
	return false;
}

bool UDualNodeAudioSubsystem::GetBarkDefinition(const FGameplayTag& Tag, FDualNodeBarkDefinition& OutDef) const
{
	if (!Tag.IsValid()) return false;
	// LINTER FIX: Variable moved to if-statement
	if (const FDualNodeBarkDefinition* FoundDef = MergedBarkCache.Find(Tag))
	{
		OutDef = *FoundDef;
		return true;
	}
	return false;
}

// --- SAVE SYSTEM ---

FDNASaveData UDualNodeAudioSubsystem::GetStateSnapshot() const
{
	FDNASaveData Data;
	Data.SavedLayers = ActiveLayers;
	Data.SavedTimeOfDay = CurrentTimeOfDay;
	return Data;
}

void UDualNodeAudioSubsystem::RestoreState(const FDNASaveData& SaveData)
{
	ActiveLayers = SaveData.SavedLayers;
	CurrentTimeOfDay = SaveData.SavedTimeOfDay;
	CurrentPlayingTag = FGameplayTag::EmptyTag; 
	UpdateMusicSystem();
}

// --- LOGIC ---

void UDualNodeAudioSubsystem::SetMusicLayer(EDNAMusicPriority Priority, FGameplayTag MusicTag, const FDualNodePlaybackSettings& Settings, double StartTimestamp)
{
	FActiveMusicLayer& Layer = ActiveLayers.FindOrAdd(Priority);
	
	if (Layer.MusicTag != MusicTag)
	{
		Layer.MusicTag = MusicTag;
		Layer.PlaylistIndex = 0;
		Layer.ShuffleHistory.Empty();
	}

	Layer.bIsActive = true;
	Layer.bIsPaused = false;
	Layer.ActiveSettings = Settings;

	if (StartTimestamp <= 0.0)
	{
		if (const UWorld* World = GetWorld()) Layer.ServerStartTime = World->GetTimeSeconds();
	}
	else
	{
		const UDualNodeAudioSettings* Config = UDualNodeAudioSettings::Get();
		if (Config && !Config->bEnableServerTimeSync)
		{
			if (const UWorld* World = GetWorld()) Layer.ServerStartTime = World->GetTimeSeconds();
		}
		else
		{
			Layer.ServerStartTime = StartTimestamp;
		}
	}

	UpdateMusicSystem();
}

void UDualNodeAudioSubsystem::ClearMusicLayer(EDNAMusicPriority Priority)
{
	if (ActiveLayers.Contains(Priority))
	{
		ActiveLayers.Remove(Priority);
		UpdateMusicSystem();
	}
}

void UDualNodeAudioSubsystem::PauseMusicLayer(EDNAMusicPriority Priority)
{
	if (FActiveMusicLayer* Layer = ActiveLayers.Find(Priority))
	{
		Layer->bIsPaused = true;
		UpdateMusicSystem();
	}
}

void UDualNodeAudioSubsystem::ResumeMusicLayer(EDNAMusicPriority Priority)
{
	if (FActiveMusicLayer* Layer = ActiveLayers.Find(Priority))
	{
		Layer->bIsPaused = false;
		UpdateMusicSystem();
	}
}

void UDualNodeAudioSubsystem::SkipMusicTrack(EDNAMusicPriority Priority)
{
	if (FActiveMusicLayer* Layer = ActiveLayers.Find(Priority))
	{
		FDualNodePlaylist PlaylistDef;
		if (GetPlaylistDefinition(Layer->MusicTag, PlaylistDef))
		{
			const int32 NumTracks = PlaylistDef.Tracks.Num();
			if (NumTracks == 0) return;

			if (PlaylistDef.PlayMode == EDNAPlaylistMode::Shuffle)
			{
				Layer->ShuffleHistory.AddUnique(Layer->PlaylistIndex);
				
				if (Layer->ShuffleHistory.Num() >= NumTracks)
				{
					Layer->ShuffleHistory.Empty();
				}

				TArray<int32> AvailableIndices;
				for (int32 i = 0; i < NumTracks; i++)
				{
					if (!Layer->ShuffleHistory.Contains(i)) AvailableIndices.Add(i);
				}

				if (AvailableIndices.Num() > 0)
				{
					const int32 RandomIdx = FMath::RandRange(0, AvailableIndices.Num() - 1);
					Layer->PlaylistIndex = AvailableIndices[RandomIdx];
				}
			}
			else if (PlaylistDef.PlayMode == EDNAPlaylistMode::Random)
			{
				Layer->PlaylistIndex = FMath::RandRange(0, NumTracks - 1);
			}
			else 
			{
				Layer->PlaylistIndex++;
				if (Layer->PlaylistIndex >= NumTracks) Layer->PlaylistIndex = 0;
			}
			
			if (const UWorld* World = GetWorld()) Layer->ServerStartTime = World->GetTimeSeconds();
			UpdateMusicSystem();
		}
	}
}

void UDualNodeAudioSubsystem::SetSystemTimeOfDay(float NewTime)
{
	CurrentTimeOfDay = NewTime;
	UpdateMusicSystem();
}

void UDualNodeAudioSubsystem::HandleLevelChange()
{
	const UDualNodeAudioSettings* Settings = UDualNodeAudioSettings::Get();
	if (Settings && Settings->bResetStackOnLevelChange)
	{
		ActiveLayers.Reset();
		UpdateMusicSystem();
	}
}

bool UDualNodeAudioSubsystem::TryPlayBark(const UObject* WorldContext, FGameplayTag BarkTag, const FVector& Location)
{
	FDualNodeBarkDefinition Def;
	if (!GetBarkDefinition(BarkTag, Def)) return false;

	USoundBase* LoadedSound = LoadSoundSync(Def.Sound);
	if (!LoadedSound) return false;

	const UWorld* World = WorldContext ? WorldContext->GetWorld() : GetWorld();
	if (!World) return false;

	const double Now = World->GetTimeSeconds();

	if (Def.CooldownGroup.IsValid())
	{
		if (const double* LastTime = BarkCooldowns.Find(Def.CooldownGroup))
		{
			if (Now < *LastTime + Def.CooldownDuration) return false;
		}
	}

	if (Def.TriggerChance < 1.0f && FMath::FRand() > Def.TriggerChance) return false;

	if (Def.CooldownGroup.IsValid()) BarkCooldowns.Add(Def.CooldownGroup, Now);

	UGameplayStatics::PlaySoundAtLocation(WorldContext, LoadedSound, Location, FRotator::ZeroRotator, 1.0f, 1.0f, 0.0f, nullptr, Def.Concurrency);
	return true;
}

void UDualNodeAudioSubsystem::UpdateMusicSystem()
{
	// 1. PRIORITY CHECK
	FActiveMusicLayer BestLayer;
	BestLayer.bIsActive = false;

	// LINTER FIX: static_cast and removed unused 'BestPrio' variable
	for (uint8 i = static_cast<uint8>(EDNAMusicPriority::Critical); i >= static_cast<uint8>(EDNAMusicPriority::Ambient); --i)
	{
		const EDNAMusicPriority P = static_cast<EDNAMusicPriority>(i);
		if (ActiveLayers.Contains(P) && ActiveLayers[P].bIsActive)
		{
			// LINTER FIX: Variable moved to if-statement
			if (FDualNodeMusicDefinition CheckDef; GetMusicDefinition(ActiveLayers[P].MusicTag, CheckDef))
			{
				if (CheckDef.bUseTimeOfDay)
				{
					const bool bInRange = (CurrentTimeOfDay >= CheckDef.ActiveHourRange.X && CurrentTimeOfDay <= CheckDef.ActiveHourRange.Y);
					if (!bInRange) continue; 
				}
			}
			
			BestLayer = ActiveLayers[P];
			break; 
		}
	}

	if (BestLayer.bIsPaused)
	{
		if (AudioComp_A && AudioComp_A->IsPlaying()) AudioComp_A->SetPaused(true);
		if (AudioComp_B && AudioComp_B->IsPlaying()) AudioComp_B->SetPaused(true);
		return;
	}
	else
	{
		if (AudioComp_A && AudioComp_A->bIsPaused) AudioComp_A->SetPaused(false);
		if (AudioComp_B && AudioComp_B->bIsPaused) AudioComp_B->SetPaused(false);
	}

	if (BestLayer.MusicTag == CurrentPlayingTag && BestLayer.bIsActive) return;

	CurrentPlayingTag = BestLayer.MusicTag;

	UWorld* World = GetWorld();
	if (!World) return; 

	if (!AudioComp_A) { AudioComp_A = NewObject<UAudioComponent>(World); AudioComp_A->bAutoDestroy = false; AudioComp_A->RegisterComponentWithWorld(World); }
	if (!AudioComp_B) { AudioComp_B = NewObject<UAudioComponent>(World); AudioComp_B->bAutoDestroy = false; AudioComp_B->RegisterComponentWithWorld(World); }

	// 2. PLAYLIST RESOLVING
	FGameplayTag FinalTrackTag = BestLayer.MusicTag;

	// LINTER FIX: Variable moved to if-statement
	if (FDualNodePlaylist PlaylistDef; GetPlaylistDefinition(BestLayer.MusicTag, PlaylistDef))
	{
		if (PlaylistDef.Tracks.IsValidIndex(BestLayer.PlaylistIndex))
		{
			FinalTrackTag = PlaylistDef.Tracks[BestLayer.PlaylistIndex];
		}
	}

	FDualNodeMusicDefinition BaseDef;
	const bool bHasData = GetMusicDefinition(FinalTrackTag, BaseDef);
	
	if (bHasData)
	{
		BaseDef.Sound = LoadSoundSync(BaseDef.Sound);
	}

	const FDualNodeMusicDefinition Def = ApplyOverrides(BaseDef, BestLayer.ActiveSettings);

	// 3. CROSSFADE & PLAY
	UAudioComponent* FadeInComp = bIsPlayingA ? AudioComp_B : AudioComp_A;
	UAudioComponent* FadeOutComp = bIsPlayingA ? AudioComp_A : AudioComp_B;

	if (FadeOutComp->IsPlaying())
	{
		const float OutDur = bHasData ? Def.FadeInDuration : 1.0f; 
		FadeOutComp->FadeOut(OutDur, 0.0f);
	}

	if (bHasData && Def.Sound.Get())
	{
		FadeInComp->SetSound(Def.Sound.Get());
		FadeInComp->SoundClassOverride = Def.SoundClass;
		FadeInComp->PitchMultiplier = Def.PitchMultiplier;
		
		float StartTimeOffset = 0.0f;
		
		const double Now = World->GetTimeSeconds();
		const float Elapsed = static_cast<float>(Now - BestLayer.ServerStartTime);
		const float Duration = Def.Sound.Get()->Duration;
		
		if (Duration > 5.0f && Elapsed > 0.f) StartTimeOffset = FMath::Fmod(Elapsed, Duration);
		StartTimeOffset += Def.StartTime;

		FadeInComp->FadeIn(Def.FadeInDuration, Def.VolumeMultiplier, StartTimeOffset);
	}
	else
	{
		FadeInComp->Stop();
	}

	bIsPlayingA = !bIsPlayingA;
}