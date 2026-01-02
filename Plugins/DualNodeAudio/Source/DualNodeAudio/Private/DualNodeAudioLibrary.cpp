#include "DualNodeAudioLibrary.h"
#include "DualNodeAudioSubsystem.h"
#include "DualNodeMusicManager.h"
#include "DualNodePlayerAudioComponent.h"

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

// --- MIXING IMPLS (V13.0) ---

void UDualNodeAudioLibrary::SetVolumeByClassTag(const UObject* WorldContextObject, FGameplayTag ClassTag, float Volume, float FadeTime)
{
	if (UDualNodeAudioSubsystem* Sys = GetDNASubsystem(WorldContextObject))
	{
		Sys->SetVolumeByClassTag(ClassTag, Volume, FadeTime);
	}
}

void UDualNodeAudioLibrary::MuteClassTag(const UObject* WorldContextObject, FGameplayTag ClassTag, bool bMuted, float FadeTime)
{
	if (UDualNodeAudioSubsystem* Sys = GetDNASubsystem(WorldContextObject))
	{
		Sys->MuteClassTag(ClassTag, bMuted, FadeTime);
	}
}


// --- NEW FEATURES ---

void UDualNodeAudioLibrary::PreloadSoundGroup(const UObject* WorldContextObject, FGameplayTag RootTag)
{
	if (UDualNodeAudioSubsystem* Sys = GetDNASubsystem(WorldContextObject))
	{
		Sys->PreloadSoundGroup(RootTag);
	}
}

void UDualNodeAudioLibrary::DebugAudioState(const UObject* WorldContextObject)
{
	if (UDualNodeAudioSubsystem* Sys = GetDNASubsystem(WorldContextObject))
	{
		// Printing simple debug info
		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow, TEXT("--- DNA AUDIO STACK ---"));
			// Hier könntest du Sys->GetStateSnapshot() aufrufen und printen
		}
	}
}

// --- STANDARD IMPLS (Standard, Unchanged) ---

void UDualNodeAudioLibrary::PlaySoundByTag_2D(const UObject* WorldContextObject, FGameplayTag Tag, float VolumeMultiplier, float PitchMultiplier)
{
	if (UDualNodeAudioSubsystem* Sys = GetDNASubsystem(WorldContextObject))
	{
		Sys->PlaySound2D(WorldContextObject, Tag, VolumeMultiplier, PitchMultiplier);
	}
}

void UDualNodeAudioLibrary::PlaySoundByTag_AtLocation(const UObject* WorldContextObject, FGameplayTag Tag, const FVector& Location, FRotator Rotation, float VolumeMultiplier, float PitchMultiplier)
{
	if (UDualNodeAudioSubsystem* Sys = GetDNASubsystem(WorldContextObject))
	{
		Sys->PlaySoundAtLocation(WorldContextObject, Tag, Location, Rotation, VolumeMultiplier, PitchMultiplier);
	}
}

void UDualNodeAudioLibrary::PlaySoundByTag_WithPhysics(const UObject* WorldContextObject, FGameplayTag Tag, FHitResult Hit, float VolumeMultiplier, float PitchMultiplier)
{
	if (UDualNodeAudioSubsystem* Sys = GetDNASubsystem(WorldContextObject))
	{
		// Hier müsste ResolveSoundFromPhysics Logic rein, oder neue Subsystem Funktion.
		// Fallback:
		Sys->PlaySoundAtLocation(WorldContextObject, Tag, Hit.Location, FRotator::ZeroRotator, VolumeMultiplier, PitchMultiplier);
	}
}

void UDualNodeAudioLibrary::BroadcastSoundByTag_AtLocation(const UObject* WorldContextObject, FGameplayTag Tag, const FVector& Location, FRotator Rotation, bool bReliable)
{
	if (UDualNodeAudioSubsystem* Sys = GetDNASubsystem(WorldContextObject))
	{
		if (ADualNodeMusicManager* Manager = Sys->GetOrSpawnMusicManager(WorldContextObject))
		{
			if (bReliable) Manager->MulticastPlaySoundAtLocation_Reliable(Tag, Location, Rotation, 1.f, 1.f);
			else Manager->MulticastPlaySoundAtLocation(Tag, Location, Rotation, 1.f, 1.f);
		}
	}
}

UAudioComponent* UDualNodeAudioLibrary::SpawnSoundByTag_Attached(const UObject* WorldContextObject, FGameplayTag Tag, USceneComponent* AttachToComponent, FName SocketName, bool bAutoDestroy)
{
	if (UDualNodeAudioSubsystem* Sys = GetDNASubsystem(WorldContextObject))
	{
		return Sys->SpawnSoundAttached(WorldContextObject, Tag, AttachToComponent, SocketName, bAutoDestroy);
	}
	return nullptr;
}

bool UDualNodeAudioLibrary::PlayBarkByTag(const UObject* WorldContextObject, FGameplayTag BarkTag, const FVector& Location)
{
	if (UDualNodeAudioSubsystem* Sys = GetDNASubsystem(WorldContextObject))
	{
		return Sys->TryPlayBark(WorldContextObject, BarkTag, Location);
	}
	return false;
}

// --- MUSIC ---

void UDualNodeAudioLibrary::SetGlobalMusic(const UObject* WorldContextObject, EDNAMusicPriority Priority, FGameplayTag MusicTag, FDualNodePlaybackSettings Settings)
{
	if (UDualNodeAudioSubsystem* Sys = GetDNASubsystem(WorldContextObject))
	{
		if (ADualNodeMusicManager* Manager = Sys->GetOrSpawnMusicManager(WorldContextObject))
		{
			Manager->Server_SetGlobalMusic(Priority, MusicTag, Settings);
		}
	}
}

void UDualNodeAudioLibrary::ClearGlobalMusic(const UObject* WorldContextObject, EDNAMusicPriority Priority)
{
	if (UDualNodeAudioSubsystem* Sys = GetDNASubsystem(WorldContextObject))
	{
		if (ADualNodeMusicManager* Manager = Sys->GetOrSpawnMusicManager(WorldContextObject))
		{
			Manager->Server_ClearGlobalMusic(Priority);
		}
	}
}

void UDualNodeAudioLibrary::PauseGlobalMusic(const UObject* WorldContextObject, EDNAMusicPriority Priority)
{
	if (UDualNodeAudioSubsystem* Sys = GetDNASubsystem(WorldContextObject))
	{
		if (ADualNodeMusicManager* Manager = Sys->GetOrSpawnMusicManager(WorldContextObject))
		{
			Manager->Server_PauseMusic(Priority);
		}
	}
}

void UDualNodeAudioLibrary::ResumeGlobalMusic(const UObject* WorldContextObject, EDNAMusicPriority Priority)
{
	if (UDualNodeAudioSubsystem* Sys = GetDNASubsystem(WorldContextObject))
	{
		if (ADualNodeMusicManager* Manager = Sys->GetOrSpawnMusicManager(WorldContextObject))
		{
			Manager->Server_ResumeMusic(Priority);
		}
	}
}

void UDualNodeAudioLibrary::SkipGlobalMusicTrack(const UObject* WorldContextObject, EDNAMusicPriority Priority)
{
	// Skip Logic ist noch nicht voll repliziert in V12, 
	// aber wir können hier Placeholder lassen oder über Manager implementieren.
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

void UDualNodeAudioLibrary::SkipLocalMusicTrack(const UObject* WorldContextObject, EDNAMusicPriority Priority)
{
	if (UDualNodeAudioSubsystem* Sys = GetDNASubsystem(WorldContextObject))
	{
		Sys->SkipMusicTrack(Priority);
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