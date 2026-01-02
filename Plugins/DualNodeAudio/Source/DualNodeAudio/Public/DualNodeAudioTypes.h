#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Sound/SoundBase.h"
#include "Sound/SoundAttenuation.h"
#include "Sound/SoundConcurrency.h"
#include "Sound/SoundModulationDestination.h" // Für Audio Modulation
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

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Override", meta=(ClampMin="-1.0"))
	float FadeInDuration = -1.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Override", meta=(ClampMin="-1.0"))
	float FadeOutDuration = -1.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Override", meta=(ClampMin="-1.0"))
	float VolumeMultiplier = -1.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Override", meta=(ClampMin="-1.0"))
	float PitchMultiplier = -1.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Override", meta=(ClampMin="-1.0"))
	float StartTime = -1.0f;
};

// --- STRUCTS: DEFINITIONS (Soft Pointers Update) ---

USTRUCT(BlueprintType)
struct FDualNodeSoundDefinition
{
	GENERATED_BODY()

	// MEMORY FIX: Soft Object Pointer
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Audio")
	TSoftObjectPtr<USoundBase> Sound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mixing")
	TObjectPtr<USoundClass> SoundClass = nullptr;

	// MIXING UPDATE: Audio Modulation Class
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mixing")
	TArray<FSoundModulationDestinationSettings> ModulationSettings;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Concurrency")
	TObjectPtr<USoundConcurrency> Concurrency = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Physics")
	TMap<TObjectPtr<UPhysicalMaterial>, TSoftObjectPtr<USoundBase>> PhysicsMap;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Adjustments")
	float VolumeMultiplier = 1.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Adjustments")
	float PitchMultiplier = 1.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Adjustments")
	TObjectPtr<USoundAttenuation> Attenuation = nullptr;
};

USTRUCT(BlueprintType)
struct FDualNodeMusicDefinition
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Audio")
	TSoftObjectPtr<USoundBase> Sound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Layers")
	TMap<FName, TSoftObjectPtr<USoundBase>> AdditionalLayers;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mixing")
	TObjectPtr<USoundClass> SoundClass = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "World", meta=(InlineEditConditionToggle))
	bool bUseTimeOfDay = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "World", meta=(EditCondition="bUseTimeOfDay"))
	FVector2D ActiveHourRange = FVector2D(0.0f, 24.0f);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Transitions")
	float FadeInDuration = 2.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Transitions")
	float FadeOutDuration = 2.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Adjustments")
	float VolumeMultiplier = 1.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Adjustments")
	float PitchMultiplier = 1.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Adjustments")
	float StartTime = 0.0f;
};

USTRUCT(BlueprintType)
struct FDualNodePlaylist
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FGameplayTag> Tracks;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EDNAPlaylistMode PlayMode = EDNAPlaylistMode::Sequential;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bAutoAdvance = true;
};

USTRUCT(BlueprintType)
struct FDualNodeBarkDefinition
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSoftObjectPtr<USoundBase> Sound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FGameplayTag CooldownGroup;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float CooldownDuration = 5.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float TriggerChance = 1.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<USoundConcurrency> Concurrency = nullptr;
};

// --- RUNTIME STATE (Active Layers) ---

USTRUCT(BlueprintType)
struct FActiveMusicLayer
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly)
	FGameplayTag MusicTag;

	UPROPERTY(BlueprintReadOnly)
	double ServerStartTime = 0.0;
	
	UPROPERTY(BlueprintReadOnly)
	bool bIsActive = false;

	UPROPERTY(BlueprintReadOnly)
	bool bIsPaused = false;
	
	UPROPERTY()
	FDualNodePlaybackSettings ActiveSettings;

	// Playlist State
	UPROPERTY()
	int32 PlaylistIndex = 0;

	// SMART SHUFFLE: History (Indexe, die schon gespielt wurden)
	UPROPERTY()
	TArray<int32> ShuffleHistory;
};

// --- SAVEGAME STRUCT (Persistence) ---

USTRUCT(BlueprintType)
struct FDNASaveData
{
	GENERATED_BODY()

	// Speichert alle aktiven Layer (inkl. Playlist Status & Shuffle History)
	UPROPERTY(BlueprintReadWrite)
	TMap<EDNAMusicPriority, FActiveMusicLayer> SavedLayers;

	// Speichert Time of Day
	UPROPERTY(BlueprintReadWrite)
	float SavedTimeOfDay = 12.0f;
};