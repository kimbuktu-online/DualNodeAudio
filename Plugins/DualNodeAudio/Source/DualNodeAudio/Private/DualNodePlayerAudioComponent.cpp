#include "DualNodePlayerAudioComponent.h"
#include "DualNodeAudioSubsystem.h"

UDualNodePlayerAudioComponent::UDualNodePlayerAudioComponent()
{
	SetIsReplicatedByDefault(true); 
}

UDualNodeAudioSubsystem* GetLocalDNASubsystem(const UObject* WorldContext)
{
	if (UWorld* World = WorldContext->GetWorld())
	{
		if (UGameInstance* GI = World->GetGameInstance())
		{
			return GI->GetSubsystem<UDualNodeAudioSubsystem>();
		}
	}
	return nullptr;
}

void UDualNodePlayerAudioComponent::Client_SetMusicLayer_Implementation(EDNAMusicPriority Priority, FGameplayTag MusicTag, FDualNodePlaybackSettings Settings)
{
	if (UDualNodeAudioSubsystem* Sys = GetLocalDNASubsystem(this))
	{
		Sys->SetMusicLayer(Priority, MusicTag, Settings, 0.0);
	}
}

void UDualNodePlayerAudioComponent::Client_ClearMusicLayer_Implementation(EDNAMusicPriority Priority)
{
	if (UDualNodeAudioSubsystem* Sys = GetLocalDNASubsystem(this))
	{
		Sys->ClearMusicLayer(Priority);
	}
}