#include "DualNodeMusicManager.h"
#include "DualNodeAudioSubsystem.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/GameStateBase.h"

ADualNodeMusicManager::ADualNodeMusicManager()
{
	bReplicates = true;
	bAlwaysRelevant = true; 
}

// --- SFX ---
void ADualNodeMusicManager::MulticastPlaySoundAtLocation_Implementation(FGameplayTag SoundTag, FVector Location, FRotator Rotation, float VolumeMult, float PitchMult)
{
	PlayLocalSound(SoundTag, Location, Rotation, VolumeMult, PitchMult);
}

void ADualNodeMusicManager::MulticastPlaySoundAtLocation_Reliable_Implementation(FGameplayTag SoundTag, FVector Location, FRotator Rotation, float VolumeMult, float PitchMult)
{
	PlayLocalSound(SoundTag, Location, Rotation, VolumeMult, PitchMult);
}

void ADualNodeMusicManager::PlayLocalSound(FGameplayTag Tag, FVector Location, FRotator Rotation, float Volume, float Pitch)
{
	if (UGameInstance* GI = GetGameInstance())
	{
		if (UDualNodeAudioSubsystem* Subsystem = GI->GetSubsystem<UDualNodeAudioSubsystem>())
		{
			FDualNodeSoundDefinition Def;
			if (Subsystem->GetSoundDefinition(Tag, Def))
			{
				// FIX: Soft Pointer laden
				USoundBase* LoadedSound = nullptr;
				if (Def.Sound.Get()) LoadedSound = Def.Sound.Get();
				else LoadedSound = Def.Sound.LoadSynchronous();

				if (LoadedSound)
				{
					UGameplayStatics::PlaySoundAtLocation(this, LoadedSound, Location, Rotation, Def.VolumeMultiplier * Volume, Def.PitchMultiplier * Pitch, 0.0f, Def.Attenuation, Def.Concurrency);
				}
			}
		}
	}
}

// --- MUSIC ---

void ADualNodeMusicManager::Server_SetGlobalMusic(EDNAMusicPriority Priority, FGameplayTag Tag, FDualNodePlaybackSettings Settings)
{
	double Timestamp = GetWorld()->GetTimeSeconds();
	if (AGameStateBase* GS = GetWorld()->GetGameState())
	{
		Timestamp = GS->GetServerWorldTimeSeconds();
	}
	Multicast_SetMusicLayer(Priority, Tag, Settings, Timestamp);
}

void ADualNodeMusicManager::Server_ClearGlobalMusic(EDNAMusicPriority Priority)
{
	Multicast_ClearMusicLayer(Priority);
}

void ADualNodeMusicManager::Multicast_SetMusicLayer_Implementation(EDNAMusicPriority Priority, FGameplayTag MusicTag, FDualNodePlaybackSettings Settings, double ServerTimestamp)
{
	if (UGameInstance* GI = GetGameInstance())
	{
		if (UDualNodeAudioSubsystem* Sys = GI->GetSubsystem<UDualNodeAudioSubsystem>())
		{
			Sys->SetMusicLayer(Priority, MusicTag, Settings, ServerTimestamp);
		}
	}
}

void ADualNodeMusicManager::Multicast_ClearMusicLayer_Implementation(EDNAMusicPriority Priority)
{
	if (UGameInstance* GI = GetGameInstance())
	{
		if (UDualNodeAudioSubsystem* Sys = GI->GetSubsystem<UDualNodeAudioSubsystem>())
		{
			Sys->ClearMusicLayer(Priority);
		}
	}
}

void ADualNodeMusicManager::Multicast_PauseMusicLayer_Implementation(EDNAMusicPriority Priority)
{
	if (UGameInstance* GI = GetGameInstance())
	{
		if (UDualNodeAudioSubsystem* Sys = GI->GetSubsystem<UDualNodeAudioSubsystem>())
		{
			Sys->PauseMusicLayer(Priority);
		}
	}
}

void ADualNodeMusicManager::Multicast_ResumeMusicLayer_Implementation(EDNAMusicPriority Priority)
{
	if (UGameInstance* GI = GetGameInstance())
	{
		if (UDualNodeAudioSubsystem* Sys = GI->GetSubsystem<UDualNodeAudioSubsystem>())
		{
			Sys->ResumeMusicLayer(Priority);
		}
	}
}

void ADualNodeMusicManager::Server_SkipMusicTrack(EDNAMusicPriority Priority)
{
	Multicast_SkipMusicTrack(Priority);
}

void ADualNodeMusicManager::Multicast_SkipMusicTrack_Implementation(EDNAMusicPriority Priority)
{
	if (UGameInstance* GI = GetGameInstance())
	{
		if (UDualNodeAudioSubsystem* Sys = GI->GetSubsystem<UDualNodeAudioSubsystem>())
		{
			Sys->SkipMusicTrack(Priority);
		}
	}
}