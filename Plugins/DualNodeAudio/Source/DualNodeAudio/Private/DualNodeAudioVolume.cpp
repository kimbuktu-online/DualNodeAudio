#include "DualNodeAudioVolume.h"
#include "DualNodeAudioSubsystem.h"
#include "Components/BrushComponent.h"
#include "DualNodeMusicManager.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/Pawn.h"
#include "Engine/GameInstance.h"

ADualNodeAudioVolume::ADualNodeAudioVolume()
{
	GetBrushComponent()->SetCollisionProfileName(TEXT("Trigger"));
	bClientOnly = true;
}

void ADualNodeAudioVolume::BeginPlay()
{
	Super::BeginPlay();
	OnActorBeginOverlap.AddDynamic(this, &ADualNodeAudioVolume::OnOverlapBegin);
	OnActorEndOverlap.AddDynamic(this, &ADualNodeAudioVolume::OnOverlapEnd);
}

void ADualNodeAudioVolume::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	OnActorBeginOverlap.RemoveAll(this);
	OnActorEndOverlap.RemoveAll(this);
	Super::EndPlay(EndPlayReason);
}

void ADualNodeAudioVolume::OnOverlapBegin(AActor* OverlappedActor, AActor* OtherActor)
{
	if (!AmbientMusicTag.IsValid()) return;

	APawn* Pawn = Cast<APawn>(OtherActor);
	if (!Pawn || !Pawn->IsLocallyControlled()) return;

	// CLIENT LOGIC (Lokal)
	if (bClientOnly)
	{
		if (UGameInstance* GI = GetGameInstance())
		{
			if (UDualNodeAudioSubsystem* Sys = GI->GetSubsystem<UDualNodeAudioSubsystem>())
			{
				Sys->SetMusicLayer(Priority, AmbientMusicTag, PlaybackSettings, 0.0);
			}
		}
	}
	// SERVER LOGIC (Global)
	else if (HasAuthority())
	{
		if (UGameInstance* GI = GetGameInstance())
		{
			if (UDualNodeAudioSubsystem* Sys = GI->GetSubsystem<UDualNodeAudioSubsystem>())
			{
				if (ADualNodeMusicManager* Manager = Sys->GetOrSpawnMusicManager(this))
				{
					Manager->Server_SetGlobalMusic(Priority, AmbientMusicTag, PlaybackSettings);
				}
			}
		}
	}
}

void ADualNodeAudioVolume::OnOverlapEnd(AActor* OverlappedActor, AActor* OtherActor)
{
	if (!AmbientMusicTag.IsValid()) return;

	APawn* Pawn = Cast<APawn>(OtherActor);
	if (!Pawn || !Pawn->IsLocallyControlled()) return;

	if (bClientOnly)
	{
		if (UGameInstance* GI = GetGameInstance())
		{
			if (UDualNodeAudioSubsystem* Sys = GI->GetSubsystem<UDualNodeAudioSubsystem>())
			{
				Sys->ClearMusicLayer(Priority);
			}
		}
	}
	else if (HasAuthority())
	{
		if (UGameInstance* GI = GetGameInstance())
		{
			if (UDualNodeAudioSubsystem* Sys = GI->GetSubsystem<UDualNodeAudioSubsystem>())
			{
				if (ADualNodeMusicManager* Manager = Sys->GetOrSpawnMusicManager(this))
				{
					Manager->Server_ClearGlobalMusic(Priority);
				}
			}
		}
	}
}