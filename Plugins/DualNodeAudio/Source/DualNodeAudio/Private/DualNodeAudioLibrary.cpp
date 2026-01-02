#include "DualNodeAudioLibrary.h"
#include "DualNodeAudioSubsystem.h"
#include "DualNodeMusicManager.h"
#include "DualNodePlayerAudioComponent.h"
#include "DualNodeAudioSettings.h"
#include "EngineUtils.h"
#include "Kismet/GameplayStatics.h"
#include "DrawDebugHelpers.h"

UDualNodeAudioSubsystem* GetDNASubsystem(const UObject* WorldContext)
{
	if (!WorldContext) return nullptr;
	if (const UWorld* World = WorldContext->GetWorld())
	{
		if (const UGameInstance* GI = World->GetGameInstance())
		{
			return GI->GetSubsystem<UDualNodeAudioSubsystem>();
		}
	}
	return nullptr;
}

// Ensure static to avoid linker errors
static USoundBase* LoadSoundSync(const TSoftObjectPtr<USoundBase>& SoftSound)
{
	if (SoftSound.IsNull()) return nullptr;
	if (USoundBase* Loaded = SoftSound.Get()) return Loaded;
	return SoftSound.LoadSynchronous();
}

// --- SFX IMPLS ---

void UDualNodeAudioLibrary::PlaySoundByTag_2D(const UObject* WorldContextObject, FGameplayTag Tag, float VolumeMultiplier, float PitchMultiplier)
{
	if (UDualNodeAudioSubsystem* Sys = GetDNASubsystem(WorldContextObject))
	{
		FDualNodeSoundDefinition Def;
		if (Sys->GetSoundDefinition(Tag, Def))
		{
			USoundBase* SoundToPlay = LoadSoundSync(Def.Sound);
			
			if (SoundToPlay)
			{
				UGameplayStatics::PlaySound2D(WorldContextObject, SoundToPlay, Def.VolumeMultiplier * VolumeMultiplier, Def.PitchMultiplier * PitchMultiplier, 0.0f, Def.Concurrency, nullptr, true);
			}

			const UDualNodeAudioSettings* Settings = UDualNodeAudioSettings::Get();
			if (Settings && Settings->bWarnOnMissingTags && !SoundToPlay)
			{
				FMessageLog("PIE").Warning(FText::FromString("DNA Warning: Sound Tag found but Sound Object is NULL: " + Tag.ToString()));
			}
		}
	}
}

// LINTER FIX: const FVector&
void UDualNodeAudioLibrary::PlaySoundByTag_AtLocation(const UObject* WorldContextObject, FGameplayTag Tag, const FVector& Location, FRotator Rotation, float VolumeMultiplier, float PitchMultiplier)
{
	if (UDualNodeAudioSubsystem* Sys = GetDNASubsystem(WorldContextObject))
	{
		FDualNodeSoundDefinition Def;
		if (Sys->GetSoundDefinition(Tag, Def))
		{
			USoundBase* SoundToPlay = LoadSoundSync(Def.Sound);

			if (SoundToPlay)
			{
				UGameplayStatics::PlaySoundAtLocation(WorldContextObject, SoundToPlay, Location, Rotation, Def.VolumeMultiplier * VolumeMultiplier, Def.PitchMultiplier * PitchMultiplier, 0.0f, Def.Attenuation, Def.Concurrency);
				
				const UDualNodeAudioSettings* Settings = UDualNodeAudioSettings::Get();
				if (Settings && Settings->bDrawDebugSpheres)
				{
					DrawDebugSphere(WorldContextObject->GetWorld(), Location, 32.0f, 8, FColor::Green, false, 2.0f);
				}
			}
		}
	}
}

void UDualNodeAudioLibrary::PlaySoundByTag_WithPhysics(const UObject* WorldContextObject, FGameplayTag Tag, FHitResult Hit, float VolumeMultiplier, float PitchMultiplier)
{
	const UDualNodeAudioSettings* Settings = UDualNodeAudioSettings::Get();
	if (Settings && !Settings->bEnablePhysicsMaterialSupport)
	{
		PlaySoundByTag_AtLocation(WorldContextObject, Tag, Hit.Location, FRotator::ZeroRotator, VolumeMultiplier, PitchMultiplier);
		return;
	}

	if (const UDualNodeAudioSubsystem* Sys = GetDNASubsystem(WorldContextObject))
	{
		FDualNodeSoundDefinition Def;
		if (Sys->GetSoundDefinition(Tag, Def))
		{
			USoundBase* FinalSound = Sys->ResolveSoundFromPhysics(Def, Hit.PhysMaterial.Get());
			
			if (FinalSound)
			{
				UGameplayStatics::PlaySoundAtLocation(WorldContextObject, FinalSound, Hit.Location, FRotator::ZeroRotator, Def.VolumeMultiplier * VolumeMultiplier, Def.PitchMultiplier * PitchMultiplier, 0.0f, Def.Attenuation, Def.Concurrency);
			}
		}
	}
}

// LINTER FIX: const FVector&
void UDualNodeAudioLibrary::BroadcastSoundByTag_AtLocation(const UObject* WorldContextObject, FGameplayTag Tag, const FVector& Location, FRotator Rotation, bool bReliable)
{
	if (const UWorld* World = WorldContextObject->GetWorld())
	{
		for (TActorIterator<ADualNodeMusicManager> It(World); It; ++It)
		{
			if (bReliable) It->MulticastPlaySoundAtLocation_Reliable(Tag, Location, Rotation, 1.f, 1.f);
			else It->MulticastPlaySoundAtLocation(Tag, Location, Rotation, 1.f, 1.f);
			return;
		}
	}
}

UAudioComponent* UDualNodeAudioLibrary::SpawnSoundByTag_Attached(const UObject* WorldContextObject, FGameplayTag Tag, USceneComponent* AttachToComponent, FName SocketName, bool bAutoDestroy)
{
	if (UDualNodeAudioSubsystem* Sys = GetDNASubsystem(WorldContextObject))
	{
		FDualNodeSoundDefinition Def;
		if (Sys->GetSoundDefinition(Tag, Def))
		{
			USoundBase* SoundToPlay = LoadSoundSync(Def.Sound);

			if (SoundToPlay)
			{
				return UGameplayStatics::SpawnSoundAttached(SoundToPlay, AttachToComponent, SocketName, FVector::ZeroVector, FRotator::ZeroRotator, EAttachLocation::KeepRelativeOffset, bAutoDestroy, Def.VolumeMultiplier, Def.PitchMultiplier, 0.0f, Def.Attenuation, Def.Concurrency);
			}
		}
	}
	return nullptr;
}

// --- BARKS ---

// LINTER FIX: const FVector&
bool UDualNodeAudioLibrary::PlayBarkByTag(const UObject* WorldContextObject, FGameplayTag BarkTag, const FVector& Location)
{
	if (UDualNodeAudioSubsystem* Sys = GetDNASubsystem(WorldContextObject))
	{
		return Sys->TryPlayBark(WorldContextObject, BarkTag, Location);
	}
	return false;
}

// --- MUSIC IMPLS (Standard, Unchanged) ---

void UDualNodeAudioLibrary::SetGlobalMusic(const UObject* WorldContextObject, EDNAMusicPriority Priority, FGameplayTag MusicTag, FDualNodePlaybackSettings Settings)
{
	if (const UWorld* World = WorldContextObject->GetWorld())
	{
		for (TActorIterator<ADualNodeMusicManager> It(World); It; ++It)
		{
			It->Server_SetGlobalMusic(Priority, MusicTag, Settings);
			return;
		}
	}
}

void UDualNodeAudioLibrary::ClearGlobalMusic(const UObject* WorldContextObject, EDNAMusicPriority Priority)
{
	if (const UWorld* World = WorldContextObject->GetWorld())
	{
		for (TActorIterator<ADualNodeMusicManager> It(World); It; ++It)
		{
			It->Server_ClearGlobalMusic(Priority);
			return;
		}
	}
}

void UDualNodeAudioLibrary::PauseGlobalMusic(const UObject* WorldContextObject, EDNAMusicPriority Priority)
{
	if (const UWorld* World = WorldContextObject->GetWorld())
	{
		for (TActorIterator<ADualNodeMusicManager> It(World); It; ++It)
		{
			It->Multicast_PauseMusicLayer(Priority);
			return;
		}
	}
}

void UDualNodeAudioLibrary::ResumeGlobalMusic(const UObject* WorldContextObject, EDNAMusicPriority Priority)
{
	if (const UWorld* World = WorldContextObject->GetWorld())
	{
		for (TActorIterator<ADualNodeMusicManager> It(World); It; ++It)
		{
			It->Multicast_ResumeMusicLayer(Priority);
			return;
		}
	}
}

void UDualNodeAudioLibrary::SetLocalMusic(const UObject* WorldContextObject, EDNAMusicPriority Priority, FGameplayTag MusicTag, FDualNodePlaybackSettings Settings)
{
	if (UDualNodeAudioSubsystem* Sys = GetDNASubsystem(WorldContextObject))
	{
		Sys->SetMusicLayer(Priority, MusicTag, Settings, 0.0);
	}
}

void UDualNodeAudioLibrary::ClearLocalMusic(const UObject* WorldContextObject, EDNAMusicPriority Priority)
{
	if (UDualNodeAudioSubsystem* Sys = GetDNASubsystem(WorldContextObject))
	{
		Sys->ClearMusicLayer(Priority);
	}
}

void UDualNodeAudioLibrary::ClientMusicPush(APlayerController* TargetPlayer, EDNAMusicPriority Priority, FGameplayTag MusicTag, FDualNodePlaybackSettings Settings)
{
	if (!TargetPlayer) return;
	if (UDualNodePlayerAudioComponent* Comp = TargetPlayer->FindComponentByClass<UDualNodePlayerAudioComponent>())
	{
		Comp->Client_SetMusicLayer(Priority, MusicTag, Settings);
	}
}

void UDualNodeAudioLibrary::ClientMusicClear(APlayerController* TargetPlayer, EDNAMusicPriority Priority)
{
	if (!TargetPlayer) return;
	if (UDualNodePlayerAudioComponent* Comp = TargetPlayer->FindComponentByClass<UDualNodePlayerAudioComponent>())
	{
		Comp->Client_ClearMusicLayer(Priority);
	}
}

void UDualNodeAudioLibrary::SetSystemTimeOfDay(const UObject* WorldContextObject, float Hour)
{
	if (UDualNodeAudioSubsystem* Sys = GetDNASubsystem(WorldContextObject))
	{
		Sys->SetSystemTimeOfDay(Hour);
	}
}

void UDualNodeAudioLibrary::SkipGlobalMusicTrack(const UObject* WorldContextObject, EDNAMusicPriority Priority)
{
	if (const UWorld* World = WorldContextObject->GetWorld())
	{
		for (TActorIterator<ADualNodeMusicManager> It(World); It; ++It)
		{
			It->Server_SkipMusicTrack(Priority);
			return;
		}
	}
}

void UDualNodeAudioLibrary::SkipLocalMusicTrack(const UObject* WorldContextObject, EDNAMusicPriority Priority)
{
	if (UDualNodeAudioSubsystem* Sys = GetDNASubsystem(WorldContextObject))
	{
		Sys->SkipMusicTrack(Priority);
	}
}