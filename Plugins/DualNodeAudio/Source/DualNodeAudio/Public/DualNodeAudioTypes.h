#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Sound/SoundBase.h"
#include "Sound/SoundAttenuation.h"
#include "Sound/SoundConcurrency.h"
#include "Sound/SoundModulationDestination.h"
#include "PhysicalMaterials/PhysicalMaterial.h"
#include "DualNodeAudioTypes.generated.h"

// --- ENUMS ---

UENUM(BlueprintType)
enum class EDNAMusicPriority : uint8
{
	None		= 0 UMETA(Hidden),
	Ambient		= 1,
	Gameplay	= 2,
	Combat		= 3,
	UI			= 4,
	Critical	= 5
};

UENUM(BlueprintType)
enum class EDNAPlaylistMode : uint8
{
	Sequential	UMETA(DisplayName = "Sequential"),
	Random		UMETA(DisplayName = "Random (Pure)"),
	Shuffle		UMETA(DisplayName = "Smart Shuffle (No Repeats)")
};

// --- STRUCTS: CONTROL ---

USTRUCT(BlueprintType)
struct FDualNodePlaybackSettings
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DNA|Settings", meta=(ClampMin="-1.0"))
	float FadeInDuration = -1.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DNA|Settings", meta=(ClampMin="-1.0"))
	float FadeOutDuration = -1.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DNA|Settings", meta=(ClampMin="-1.0"))
	float VolumeMultiplier = -1.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DNA|Settings", meta=(ClampMin="-1.0"))
	float PitchMultiplier = -1.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DNA|Settings", meta=(ClampMin="-1.0"))
	float StartTime = -1.0f;
};

// --- STRUCTS: DEFINITIONS ---

USTRUCT(BlueprintType)
struct FDualNodeSoundDefinition
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DNA|Definition")
	TSoftObjectPtr<USoundBase> Sound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DNA|Mixing")
	TObjectPtr<USoundClass> SoundClass = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DNA|Mixing")
	TArray<FSoundModulationDestinationSettings> ModulationSettings;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DNA|Concurrency")
	TObjectPtr<USoundConcurrency> Concurrency = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DNA|Physics")
	TMap<TObjectPtr<UPhysicalMaterial>, TSoftObjectPtr<USoundBase>> PhysicsMap;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DNA|Adjustments")
	float VolumeMultiplier = 1.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DNA|Adjustments")
	float PitchMultiplier = 1.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DNA|Adjustments")
	TObjectPtr<USoundAttenuation> Attenuation = nullptr;
};

USTRUCT(BlueprintType)
struct FDualNodeMusicDefinition
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DNA|Definition")
	TSoftObjectPtr<USoundBase> Sound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DNA|Layers")
	TMap<FName, TSoftObjectPtr<USoundBase>> AdditionalLayers;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DNA|Mixing")
	TObjectPtr<USoundClass> SoundClass = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DNA|World", meta=(InlineEditConditionToggle))
	bool bUseTimeOfDay = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DNA|World", meta=(EditCondition="bUseTimeOfDay"))
	FVector2D ActiveHourRange = FVector2D(0.0f, 24.0f);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DNA|Transitions")
	float FadeInDuration = 2.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DNA|Transitions")
	float FadeOutDuration = 2.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DNA|Adjustments")
	float VolumeMultiplier = 1.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DNA|Adjustments")
	float PitchMultiplier = 1.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DNA|Adjustments")
	float StartTime = 0.0f;
};

USTRUCT(BlueprintType)
struct FDualNodePlaylist
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DNA|Playlist")
	TArray<FGameplayTag> Tracks;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DNA|Playlist")
	EDNAPlaylistMode PlayMode = EDNAPlaylistMode::Sequential;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DNA|Playlist")
	bool bAutoAdvance = true;
};

USTRUCT(BlueprintType)
struct FDualNodeBarkDefinition
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DNA|Bark")
	TSoftObjectPtr<USoundBase> Sound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DNA|Bark")
	FGameplayTag CooldownGroup;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DNA|Bark")
	float CooldownDuration = 5.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DNA|Bark")
	float TriggerChance = 1.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DNA|Bark")
	TObjectPtr<USoundConcurrency> Concurrency = nullptr;
};

// --- RUNTIME STATE ---

USTRUCT(BlueprintType)
struct FActiveMusicLayer
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly, Category = "DNA|Runtime")
	FGameplayTag MusicTag;

	UPROPERTY(BlueprintReadOnly, Category = "DNA|Runtime")
	double ServerStartTime = 0.0;
	
	UPROPERTY(BlueprintReadOnly, Category = "DNA|Runtime")
	bool bIsActive = false;

	UPROPERTY(BlueprintReadOnly, Category = "DNA|Runtime")
	bool bIsPaused = false;
	
	UPROPERTY()
	FDualNodePlaybackSettings ActiveSettings;

	UPROPERTY()
	int32 PlaylistIndex = 0;

	UPROPERTY()
	TArray<int32> ShuffleHistory;
};

// --- NETWORKING ---

USTRUCT()
struct FReplicatedMusicState
{
	GENERATED_BODY()

	UPROPERTY()
	FGameplayTag Tag;

	UPROPERTY()
	EDNAMusicPriority Priority = EDNAMusicPriority::None;

	UPROPERTY()
	bool bIsPaused = false;

	UPROPERTY()
	FDualNodePlaybackSettings Settings;

	UPROPERTY()
	double ServerTimestamp = 0.0;
};

// --- SAVEGAME ---

USTRUCT(BlueprintType)
struct FDNASaveData
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite, Category = "DNA|SaveData")
	TMap<EDNAMusicPriority, FActiveMusicLayer> SavedLayers;

	UPROPERTY(BlueprintReadWrite, Category = "DNA|SaveData")
	float SavedTimeOfDay = 12.0f;
};