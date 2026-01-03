#include "DualNodePlayerAudioComponent.h"
#include "DualNodeAudioSubsystem.h"
#include "Engine/World.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/GameInstance.h"

UDualNodePlayerAudioComponent::UDualNodePlayerAudioComponent()
{
	SetIsReplicatedByDefault(true); 
}

void UDualNodePlayerAudioComponent::Client_SetMusicLayer_Implementation(EDNAMusicPriority Priority, FGameplayTag MusicTag, FDualNodePlaybackSettings Settings)
{
	if (UWorld* World = GetWorld())
	{
		if (UGameInstance* GI = World->GetGameInstance())
		{
			if (UDualNodeAudioSubsystem* Sys = GI->GetSubsystem<UDualNodeAudioSubsystem>())
			{
				Sys->SetMusicLayer(Priority, MusicTag, Settings, 0.0);
			}
		}
	}
}

void UDualNodePlayerAudioComponent::Client_ClearMusicLayer_Implementation(EDNAMusicPriority Priority)
{
	if (UWorld* World = GetWorld())
	{
		if (UGameInstance* GI = World->GetGameInstance())
		{
			if (UDualNodeAudioSubsystem* Sys = GI->GetSubsystem<UDualNodeAudioSubsystem>())
			{
				Sys->ClearMusicLayer(Priority);
			}
		}
	}
}