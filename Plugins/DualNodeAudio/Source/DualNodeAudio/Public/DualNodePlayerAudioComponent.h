#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "DualNodeAudioTypes.h"
#include "DualNodePlayerAudioComponent.generated.h"

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class DUALNODEAUDIO_API UDualNodePlayerAudioComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UDualNodePlayerAudioComponent();

	// RPC: Server befiehlt DIESEM Client, lokalen Musik-Slot zu setzen
	// FIX: Settings Parameter hinzugefügt
	UFUNCTION(Client, Reliable, BlueprintCallable, Category="Dual Node Audio|Network")
	void Client_SetMusicLayer(EDNAMusicPriority Priority, FGameplayTag MusicTag, FDualNodePlaybackSettings Settings);

	// RPC: Server befiehlt DIESEM Client, Slot zu löschen
	UFUNCTION(Client, Reliable, BlueprintCallable, Category="Dual Node Audio|Network")
	void Client_ClearMusicLayer(EDNAMusicPriority Priority);
};