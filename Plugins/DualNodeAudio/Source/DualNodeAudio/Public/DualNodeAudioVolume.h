#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Volume.h"
#include "DualNodeAudioTypes.h"
#include "DualNodeAudioVolume.generated.h"

/**
 * DNA Ambient Volume.
 * Automatically triggers music/ambience on overlap. Supports Server & Client logic.
 */
UCLASS()
class DUALNODEAUDIO_API ADualNodeAudioVolume : public AVolume
{
	GENERATED_BODY()

public:
	ADualNodeAudioVolume();

protected:
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	UFUNCTION()
	void OnOverlapBegin(AActor* OverlappedActor, AActor* OtherActor);

	UFUNCTION()
	void OnOverlapEnd(AActor* OverlappedActor, AActor* OtherActor);

public:
	// The Music Tag to play (defined in Registry)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dual Node Audio")
	FGameplayTag AmbientMusicTag;

	// Priority slot for this volume
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dual Node Audio")
	EDNAMusicPriority Priority = EDNAMusicPriority::Ambient;

	// Override default fade/volume settings
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dual Node Audio")
	FDualNodePlaybackSettings PlaybackSettings;

	// If TRUE: Only affects local player (no network traffic). Good for pure ambience.
	// If FALSE: Server syncs this event to everyone (e.g. Boss Room entry).
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dual Node Audio")
	bool bClientOnly = true;
};