#include "DualNodeMusicManager.h"
#include "DualNodeAudioSubsystem.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/GameStateBase.h"
#include "Net/UnrealNetwork.h"
#include "Engine/GameInstance.h"
#include "Engine/World.h"

ADualNodeMusicManager::ADualNodeMusicManager()
{
	bReplicates = true;
	bAlwaysRelevant = true;
	// Initialisiere leere States für Prio 0 bis 5
	ActiveMusicStates.SetNum(6);
}

void ADualNodeMusicManager::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ADualNodeMusicManager, ActiveMusicStates);
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
			Subsystem->PlaySoundAtLocation(this, Tag, Location, Rotation, Volume, Pitch);
		}
	}
}

// --- MUSIC REPLICATION ---

void ADualNodeMusicManager::Server_SetGlobalMusic(EDNAMusicPriority Priority, FGameplayTag Tag, FDualNodePlaybackSettings Settings)
{
	int32 Idx = static_cast<int32>(Priority);
	if (!ActiveMusicStates.IsValidIndex(Idx)) return;

	double Timestamp = GetWorld()->GetTimeSeconds();
	if (AGameStateBase* GS = GetWorld()->GetGameState())
	{
		Timestamp = GS->GetServerWorldTimeSeconds();
	}

	// Update State
	ActiveMusicStates[Idx].Tag = Tag;
	ActiveMusicStates[Idx].Priority = Priority;
	ActiveMusicStates[Idx].Settings = Settings;
	ActiveMusicStates[Idx].ServerTimestamp = Timestamp;
	ActiveMusicStates[Idx].bIsPaused = false;

	// Server Update Immediately (RepNotify wird auf Server nicht automatisch gerufen für den Setter)
	OnRep_MusicStates();
}

void ADualNodeMusicManager::Server_ClearGlobalMusic(EDNAMusicPriority Priority)
{
	int32 Idx = static_cast<int32>(Priority);
	if (!ActiveMusicStates.IsValidIndex(Idx)) return;

	ActiveMusicStates[Idx].Tag = FGameplayTag::EmptyTag;
	ActiveMusicStates[Idx].bIsPaused = false;

	OnRep_MusicStates();
}

void ADualNodeMusicManager::Server_PauseMusic(EDNAMusicPriority Priority)
{
	int32 Idx = static_cast<int32>(Priority);
	if (ActiveMusicStates.IsValidIndex(Idx))
	{
		ActiveMusicStates[Idx].bIsPaused = true;
		OnRep_MusicStates();
	}
}

void ADualNodeMusicManager::Server_ResumeMusic(EDNAMusicPriority Priority)
{
	int32 Idx = static_cast<int32>(Priority);
	if (ActiveMusicStates.IsValidIndex(Idx))
	{
		ActiveMusicStates[Idx].bIsPaused = false;
		OnRep_MusicStates();
	}
}

// FIX: Implementation for Skip
void ADualNodeMusicManager::Server_SkipMusicTrack(EDNAMusicPriority Priority)
{
	// Skip ist ein Event, kein persistenter State im Replicated Array (außer wir würden PlaylistIndex replizieren).
	// Fürs erste triggern wir es als RPC Event.
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

void ADualNodeMusicManager::OnRep_MusicStates()
{
	if (UGameInstance* GI = GetGameInstance())
	{
		if (UDualNodeAudioSubsystem* Sys = GI->GetSubsystem<UDualNodeAudioSubsystem>())
		{
			for (const FReplicatedMusicState& State : ActiveMusicStates)
			{
				if (State.Priority == EDNAMusicPriority::None) continue;

				if (State.Tag.IsValid())
				{
					Sys->SetMusicLayer(State.Priority, State.Tag, State.Settings, State.ServerTimestamp);
					if (State.bIsPaused)
					{
						Sys->PauseMusicLayer(State.Priority);
					}
					else
					{
						Sys->ResumeMusicLayer(State.Priority);
					}
				}
				else
				{
					Sys->ClearMusicLayer(State.Priority);
				}
			}
		}
	}
}