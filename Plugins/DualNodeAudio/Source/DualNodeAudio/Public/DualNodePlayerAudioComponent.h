#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "DualNodeAudioTypes.h"
#include "DualNodePlayerAudioComponent.generated.h"

/**
 * DNA Player Component.
 * Attach this to your PlayerController to receive targeted audio events (Quest updates, "You Died").
 */
UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class DUALNODEAUDIO_API UDualNodePlayerAudioComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UDualNodePlayerAudioComponent();

	// RPC: Server -> Specific Client (Reliable)
	UFUNCTION(Client, Reliable, BlueprintCallable, Category="Dual Node Audio|Network")
	void Client_SetMusicLayer(EDNAMusicPriority Priority, FGameplayTag MusicTag, FDualNodePlaybackSettings Settings);

	// RPC: Server -> Specific Client (Reliable)
	UFUNCTION(Client, Reliable, BlueprintCallable, Category="Dual Node Audio|Network")
	void Client_ClearMusicLayer(EDNAMusicPriority Priority);
};