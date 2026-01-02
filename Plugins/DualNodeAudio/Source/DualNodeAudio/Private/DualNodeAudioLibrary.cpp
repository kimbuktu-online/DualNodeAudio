// Copyright YourCompany. All Rights Reserved.

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

// --- MIXING ---

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

// --- OPTIMIZATION ---

void UDualNodeAudioLibrary::PreloadSoundGroup(const UObject* WorldContextObject, FGameplayTag RootTag)
{
	if (UDualNodeAudioSubsystem* Sys = GetDNASubsystem(WorldContextObject))
	{
		Sys->PreloadSoundGroup(RootTag);
	}
}

void UDualNodeAudioLibrary::DebugAudioState(const UObject* WorldContextObject)
{
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow, TEXT("--- DNA AUDIO STACK ---"));
	}
}

// --- SFX IMPLS ---

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
	// FIX: Zugriff auf Get() ist jetzt validiert durch den neuen Header
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
			// FIX C2440: TSoftObjectPtr in lokalen Pointer auflösen
			TSoftObjectPtr<USoundBase> SoftSound = Sys->ResolveSoundFromPhysics(Def, Hit.PhysMaterial.Get());
			USoundBase* FinalSound = LoadSoundSync(SoftSound);
			
			if (FinalSound)
			{
				UGameplayStatics::PlaySoundAtLocation(
					WorldContextObject, 
					FinalSound, 
					Hit.Location, 
					FRotator::ZeroRotator, 
					Def.VolumeMultiplier * VolumeMultiplier, 
					Def.PitchMultiplier * PitchMultiplier, 
					0.0f, 
					Def.Attenuation, 
					Def.Concurrency
				);
			}
		}
	}
}

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

// --- MUSIC IMPLS ---

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
			It->Server_PauseMusic(Priority);
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
			It->Server_ResumeMusic(Priority);
			return;
		}
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