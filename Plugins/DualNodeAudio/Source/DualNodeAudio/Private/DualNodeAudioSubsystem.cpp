#include "DualNodeAudioSubsystem.h"
#include "DualNodeAudioSettings.h"
#include "DualNodeMusicManager.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/World.h"
#include "Engine/AssetManager.h"
#include "EngineUtils.h"

// --- HELPER ---
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

// --- LIFECYCLE ---

void UDualNodeAudioSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
	RebuildRegistryCache();
	PushGlobalMix();
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

// --- PHYSICS RESOLVER (FIX) ---

TSoftObjectPtr<USoundBase> UDualNodeAudioSubsystem::ResolveSoundFromPhysics(const FDualNodeSoundDefinition& BaseDef, const UPhysicalMaterial* PhysMat) const
{
	// Wenn ein Physics Material da ist und in der Map existiert -> nimm das.
	if (PhysMat && BaseDef.PhysicsMap.Contains(PhysMat))
	{
		return BaseDef.PhysicsMap[PhysMat];
	}
	// Sonst Default Sound
	return BaseDef.Sound;
}

// --- MIXING API ---

void UDualNodeAudioSubsystem::PushGlobalMix()
{
	if (const UDualNodeAudioSettings* Settings = UDualNodeAudioSettings::Get())
	{
		if (USoundMix* Mix = Settings->GlobalSoundMix.LoadSynchronous())
		{
			UGameplayStatics::PushSoundMixModifier(GetWorld(), Mix);
		}
	}
}

void UDualNodeAudioSubsystem::SetVolumeByClassTag(FGameplayTag ClassTag, float Volume, float FadeTime)
{
	const UDualNodeAudioSettings* Settings = UDualNodeAudioSettings::Get();
	if (!Settings) return;

	const TSoftObjectPtr<USoundClass>* FoundClassPtr = Settings->TagToSoundClassDefaults.Find(ClassTag);
	
	if (!FoundClassPtr)
	{
		FGameplayTag Parent = ClassTag.RequestDirectParent();
		while (Parent.IsValid() && !FoundClassPtr)
		{
			FoundClassPtr = Settings->TagToSoundClassDefaults.Find(Parent);
			Parent = Parent.RequestDirectParent();
		}
	}

	if (FoundClassPtr)
	{
		if (USoundClass* SC = FoundClassPtr->LoadSynchronous())
		{
			if (USoundMix* GlobalMix = Settings->GlobalSoundMix.LoadSynchronous())
			{
				UGameplayStatics::SetSoundMixClassOverride(GetWorld(), GlobalMix, SC, Volume, 1.0f, FadeTime, true);
			}
		}
	}
}

void UDualNodeAudioSubsystem::MuteClassTag(FGameplayTag ClassTag, bool bMuted, float FadeTime)
{
	SetVolumeByClassTag(ClassTag, bMuted ? 0.0f : 1.0f, FadeTime);
}

// --- ASYNC LOADING ---

void UDualNodeAudioSubsystem::ExecuteWhenSoundLoaded(const TSoftObjectPtr<USoundBase>& SoftSound, FOnSoundLoaded OnLoaded)
{
	if (SoftSound.IsNull())
	{
		OnLoaded.ExecuteIfBound(nullptr);
		return;
	}

	if (USoundBase* Loaded = SoftSound.Get())
	{
		OnLoaded.ExecuteIfBound(Loaded);
		return;
	}

	TSoftObjectPtr<USoundBase> TempPtr = SoftSound;
	StreamableManager.RequestAsyncLoad(SoftSound.ToSoftObjectPath(), 
		[TempPtr, OnLoaded]()
		{
			if (USoundBase* Asset = TempPtr.Get())
			{
				OnLoaded.ExecuteIfBound(Asset);
			}
			else
			{
				OnLoaded.ExecuteIfBound(nullptr);
			}
		}
	);
}

void UDualNodeAudioSubsystem::PreloadSoundGroup(FGameplayTag RootTag)
{
	TArray<FSoftObjectPath> AssetsToLoad;

	for (const auto& Pair : MergedRegistryCache)
	{
		if (Pair.Key.MatchesTag(RootTag)) AssetsToLoad.Add(Pair.Value.Sound.ToSoftObjectPath());
	}
	for (const auto& Pair : MergedMusicCache)
	{
		if (Pair.Key.MatchesTag(RootTag)) AssetsToLoad.Add(Pair.Value.Sound.ToSoftObjectPath());
	}

	if (AssetsToLoad.Num() > 0)
	{
		StreamableManager.RequestAsyncLoad(AssetsToLoad);
		UE_LOG(LogAudio, Log, TEXT("DNA: Preloading %d assets for group %s"), AssetsToLoad.Num(), *RootTag.ToString());
	}
}

// --- PLAY IMPLEMENTATIONS ---

void UDualNodeAudioSubsystem::PlaySound2D(const UObject* WorldContext, FGameplayTag Tag, float Vol, float Pitch)
{
	FDualNodeSoundDefinition Def;
	if (GetSoundDefinition(Tag, Def))
	{
		// Hier nutzen wir direkt die neue interne Play-Funktion
		PlaySoundAtLocation(WorldContext, Def.Sound, FVector::ZeroVector, FRotator::ZeroRotator, Vol, Pitch, &Def);
	}
}

void UDualNodeAudioSubsystem::PlaySoundAtLocation(const UObject* WorldContext, FGameplayTag Tag, FVector Location, FRotator Rotation, float Vol, float Pitch)
{
	FDualNodeSoundDefinition Def;
	if (GetSoundDefinition(Tag, Def))
	{
		PlaySoundAtLocation(WorldContext, Def.Sound, Location, Rotation, Vol, Pitch, &Def);
	}
}

// Neue Overload Implementation
void UDualNodeAudioSubsystem::PlaySoundAtLocation(const UObject* WorldContext, TSoftObjectPtr<USoundBase> SoundToPlay, FVector Location, FRotator Rotation, float Vol, float Pitch, const FDualNodeSoundDefinition* OptionalDefSettings)
{
	// Kopiere Settings wenn vorhanden, sonst Defaults
	float FinalVol = Vol;
	float FinalPitch = Pitch;
	USoundConcurrency* Concurrency = nullptr;
	USoundAttenuation* Attenuation = nullptr;

	if (OptionalDefSettings)
	{
		FinalVol *= OptionalDefSettings->VolumeMultiplier;
		FinalPitch *= OptionalDefSettings->PitchMultiplier;
		Concurrency = OptionalDefSettings->Concurrency;
		Attenuation = OptionalDefSettings->Attenuation;
	}

	ExecuteWhenSoundLoaded(SoundToPlay, FOnSoundLoaded::CreateLambda(
		[WorldContext, Location, Rotation, FinalVol, FinalPitch, Concurrency, Attenuation](USoundBase* Sound)
		{
			if (Sound)
			{
				UGameplayStatics::PlaySoundAtLocation(WorldContext, Sound, Location, Rotation, FinalVol, FinalPitch, 0.0f, Attenuation, Concurrency);
			}
		}));
}

UAudioComponent* UDualNodeAudioSubsystem::SpawnSoundAttached(const UObject* WorldContext, FGameplayTag Tag, USceneComponent* Parent, FName Socket, bool bAutoDestroy)
{
	FDualNodeSoundDefinition Def;
	if (!GetSoundDefinition(Tag, Def)) return nullptr;

	UAudioComponent* AudioComp = UGameplayStatics::SpawnSoundAttached(nullptr, Parent, Socket, FVector::ZeroVector, FRotator::ZeroRotator, EAttachLocation::KeepRelativeOffset, bAutoDestroy);
	
	if (AudioComp)
	{
		ExecuteWhenSoundLoaded(Def.Sound, FOnSoundLoaded::CreateLambda(
			[AudioComp, Def](USoundBase* Sound)
			{
				if (IsValid(AudioComp) && Sound)
				{
					AudioComp->SetSound(Sound);
					AudioComp->SetVolumeMultiplier(Def.VolumeMultiplier);
					AudioComp->SetPitchMultiplier(Def.PitchMultiplier);
					AudioComp->AttenuationSettings = Def.Attenuation;
					AudioComp->ConcurrencySet.Add(Def.Concurrency);
					AudioComp->Play();
				}
				else if (IsValid(AudioComp))
				{
					AudioComp->DestroyComponent();
				}
			}));
	}
	return AudioComp;
}

// --- AUTO SPAWN MANAGER ---

ADualNodeMusicManager* UDualNodeAudioSubsystem::GetOrSpawnMusicManager(const UObject* WorldContext)
{
	UWorld* World = WorldContext ? WorldContext->GetWorld() : GetWorld();
	if (!World) return nullptr;

	for (TActorIterator<ADualNodeMusicManager> It(World); It; ++It)
	{
		return *It;
	}

	if (World->GetNetMode() < NM_Client)
	{
		FActorSpawnParameters Params;
		Params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		ADualNodeMusicManager* NewManager = World->SpawnActor<ADualNodeMusicManager>(Params);
		return NewManager;
	}

	return nullptr;
}

// --- REGISTRY GETTERS ---
bool UDualNodeAudioSubsystem::GetSoundDefinition(const FGameplayTag& Tag, FDualNodeSoundDefinition& OutDef) const
{
	if (const FDualNodeSoundDefinition* Found = MergedRegistryCache.Find(Tag)) { OutDef = *Found; return true; } return false;
}
bool UDualNodeAudioSubsystem::GetMusicDefinition(const FGameplayTag& Tag, FDualNodeMusicDefinition& OutDef) const
{
	if (const FDualNodeMusicDefinition* Found = MergedMusicCache.Find(Tag)) { OutDef = *Found; return true; } return false;
}
bool UDualNodeAudioSubsystem::GetPlaylistDefinition(const FGameplayTag& Tag, FDualNodePlaylist& OutDef) const
{
	if (const FDualNodePlaylist* Found = MergedPlaylistCache.Find(Tag)) { OutDef = *Found; return true; } return false;
}
bool UDualNodeAudioSubsystem::GetBarkDefinition(const FGameplayTag& Tag, FDualNodeBarkDefinition& OutDef) const
{
	if (const FDualNodeBarkDefinition* Found = MergedBarkCache.Find(Tag)) { OutDef = *Found; return true; } return false;
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

// --- MUSIC LOGIC ---

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
	ActiveLayers.Remove(Priority);
	UpdateMusicSystem();
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
				if (Layer->ShuffleHistory.Num() >= NumTracks) Layer->ShuffleHistory.Empty();

				TArray<int32> AvailableIndices;
				for (int32 i = 0; i < NumTracks; i++) { if (!Layer->ShuffleHistory.Contains(i)) AvailableIndices.Add(i); }

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

void UDualNodeAudioSubsystem::UpdateMusicSystem()
{
	FActiveMusicLayer BestLayer;
	BestLayer.bIsActive = false;

	for (uint8 i = static_cast<uint8>(EDNAMusicPriority::Critical); i >= static_cast<uint8>(EDNAMusicPriority::Ambient); --i)
	{
		const EDNAMusicPriority P = static_cast<EDNAMusicPriority>(i);
		if (ActiveLayers.Contains(P) && ActiveLayers[P].bIsActive)
		{
			if (FDualNodeMusicDefinition CheckDef; GetMusicDefinition(ActiveLayers[P].MusicTag, CheckDef))
			{
				if (CheckDef.bUseTimeOfDay)
				{
					if (CurrentTimeOfDay < CheckDef.ActiveHourRange.X || CurrentTimeOfDay > CheckDef.ActiveHourRange.Y) continue;
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

	if (!AudioComp_A) { AudioComp_A = NewObject<UAudioComponent>(World); AudioComp_A->RegisterComponentWithWorld(World); }
	if (!AudioComp_B) { AudioComp_B = NewObject<UAudioComponent>(World); AudioComp_B->RegisterComponentWithWorld(World); }

	FGameplayTag FinalTrackTag = BestLayer.MusicTag;
	if (FDualNodePlaylist PlaylistDef; GetPlaylistDefinition(BestLayer.MusicTag, PlaylistDef))
	{
		if (PlaylistDef.Tracks.IsValidIndex(BestLayer.PlaylistIndex))
		{
			FinalTrackTag = PlaylistDef.Tracks[BestLayer.PlaylistIndex];
		}
	}

	FDualNodeMusicDefinition BaseDef;
	const bool bHasData = GetMusicDefinition(FinalTrackTag, BaseDef);
	const FDualNodeMusicDefinition Def = ApplyOverrides(BaseDef, BestLayer.ActiveSettings);

	if (bHasData)
	{
		ExecuteWhenSoundLoaded(Def.Sound, FOnSoundLoaded::CreateLambda(
			[this, Def, BestLayer, World](USoundBase* LoadedSound)
			{
				if (CurrentPlayingTag != BestLayer.MusicTag) return; 

				UAudioComponent* FadeInComp = bIsPlayingA ? AudioComp_B : AudioComp_A;
				UAudioComponent* FadeOutComp = bIsPlayingA ? AudioComp_A : AudioComp_B;

				if (FadeOutComp->IsPlaying()) FadeOutComp->FadeOut(Def.FadeInDuration, 0.0f);

				if (LoadedSound)
				{
					FadeInComp->SetSound(LoadedSound);
					FadeInComp->SoundClassOverride = Def.SoundClass;
					FadeInComp->SetPitchMultiplier(Def.PitchMultiplier);
					
					float StartTimeOffset = 0.0f;
					const double Now = World->GetTimeSeconds();
					const float Elapsed = static_cast<float>(Now - BestLayer.ServerStartTime);
					const float Dur = LoadedSound->Duration;
					
					if (Dur > 5.0f && Elapsed > 0.f) StartTimeOffset = FMath::Fmod(Elapsed, Dur);
					StartTimeOffset += Def.StartTime;

					FadeInComp->FadeIn(Def.FadeInDuration, Def.VolumeMultiplier, StartTimeOffset);
				}
				bIsPlayingA = !bIsPlayingA;
			}
		));
	}
	else
	{
		if (AudioComp_A) AudioComp_A->Stop();
		if (AudioComp_B) AudioComp_B->Stop();
	}
}

// --- BARKS ---

void UDualNodeAudioSubsystem::PruneBarkCooldowns(double CurrentTime)
{
	for (auto It = BarkCooldowns.CreateIterator(); It; ++It)
	{
		if (CurrentTime > It.Value() + 300.0) It.RemoveCurrent();
	}
}

bool UDualNodeAudioSubsystem::TryPlayBark(const UObject* WorldContext, FGameplayTag BarkTag, const FVector& Location)
{
	FDualNodeBarkDefinition Def;
	if (!GetBarkDefinition(BarkTag, Def)) return false;

	const UWorld* World = WorldContext ? WorldContext->GetWorld() : GetWorld();
	if (!World) return false;

	const double Now = World->GetTimeSeconds();

	if (FMath::RandRange(0, 100) == 0) PruneBarkCooldowns(Now);

	if (Def.CooldownGroup.IsValid())
	{
		if (const double* LastTime = BarkCooldowns.Find(Def.CooldownGroup))
		{
			if (Now < *LastTime + Def.CooldownDuration) return false;
		}
	}

	if (Def.TriggerChance < 1.0f && FMath::FRand() > Def.TriggerChance) return false;

	if (Def.CooldownGroup.IsValid()) BarkCooldowns.Add(Def.CooldownGroup, Now);

	PlaySoundAtLocation(WorldContext, BarkTag, Location, FRotator::ZeroRotator, 1.0f, 1.0f);
	return true;
}