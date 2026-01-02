#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Volume.h"
#include "DualNodeAudioTypes.h"
#include "DualNodeAudioVolume.generated.h"

/**
 * DNA 10.0: Ambient Volume.
 * Startet Musik/Ambience automatisch beim Betreten und stoppt beim Verlassen.
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

	// --- Overlap Events ---
	UFUNCTION()
	void OnOverlapBegin(AActor* OverlappedActor, AActor* OtherActor);

	UFUNCTION()
	void OnOverlapEnd(AActor* OverlappedActor, AActor* OtherActor);

public:
	// Welchen Musik-Tag soll dieses Volume triggern?
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dual Node Audio")
	FGameplayTag AmbientMusicTag;

	// Auf welchem Prioritäts-Slot?
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dual Node Audio")
	EDNAMusicPriority Priority = EDNAMusicPriority::Ambient;

	// Overrides für Fade-In/Out
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dual Node Audio")
	FDualNodePlaybackSettings PlaybackSettings;

	// Nur für lokalen Spieler? (True = Client Logic, False = Server Logic)
	// Normalerweise ist Ambience lokal (Client), außer es ist ein geteiltes Event.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dual Node Audio")
	bool bClientOnly = true;
};