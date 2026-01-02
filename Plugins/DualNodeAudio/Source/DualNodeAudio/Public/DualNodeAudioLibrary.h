#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "DualNodeAudioTypes.h"
#include "Components/AudioComponent.h"
#include "DualNodeAudioLibrary.generated.h"

class APlayerController;

/**
 * The main Blueprint API for Dual Node Audio (DNA).
 * Provides static functions to control music, play SFX, and handle barks.
 */
UCLASS()
class DUALNODEAUDIO_API UDualNodeAudioLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:

	// ==========================================================================================
	// SFX & ONE-SHOTS
	// ==========================================================================================

	/**
	 * Plays a sound locally (2D, no position) based on a Gameplay Tag.
	 * Ideal for UI sounds or local feedback that doesn't need to be heard by others.
	 * * [NETWORK] LOCAL ONLY. Does not replicate.
	 * [USAGE] Use for UI clicks, inventory sounds, or local notifications.
	 * * @param Tag The SFX Tag defined in the Registry (e.g., Audio.UI.Click).
	 */
	UFUNCTION(BlueprintCallable, Category = "Dual Node Audio|Local", meta = (WorldContext = "WorldContextObject"))
	static void PlaySoundByTag_2D(const UObject* WorldContextObject, FGameplayTag Tag, float VolumeMultiplier = 1.0f, float PitchMultiplier = 1.0f);

	/**
	 * Plays a sound locally at a specific location in the world.
	 * * [NETWORK] LOCAL ONLY. Does not replicate.
	 * [USAGE] Use for visual effects spawned locally (e.g. footstep particles) or events where network bandwidth is a concern.
	 * * @param Tag The SFX Tag from the Registry.
	 * @param Location World location to play the sound.
	 */
	UFUNCTION(BlueprintCallable, Category = "Dual Node Audio|Local", meta = (WorldContext = "WorldContextObject"))
	static void PlaySoundByTag_AtLocation(const UObject* WorldContextObject, FGameplayTag Tag, const FVector& Location, FRotator Rotation = FRotator::ZeroRotator, float VolumeMultiplier = 1.0f, float PitchMultiplier = 1.0f);

	/**
	 * Performs a line trace downwards to detect the physical material and plays the corresponding sound variation.
	 * * [NETWORK] LOCAL ONLY. Does not replicate.
	 * [USAGE] Best used in AnimNotifies for footsteps or impact FX. 
	 * Note: Physics tracing is expensive; ensure 'Enable Physics Material Support' is on in Project Settings.
	 * * @param Hit The HitResult containing the PhysMaterial, or use Location if falling back to default.
	 */
	UFUNCTION(BlueprintCallable, Category = "Dual Node Audio|Physics", meta = (WorldContext = "WorldContextObject"))
	static void PlaySoundByTag_WithPhysics(const UObject* WorldContextObject, FGameplayTag Tag, FHitResult Hit, float VolumeMultiplier = 1.0f, float PitchMultiplier = 1.0f);

	/**
	 * Plays a sound at a location for ALL players on the server.
	 * * [NETWORK] SERVER -> MULTICAST. Must be called on the Server (Authority).
	 * [USAGE] Use for gameplay-relevant events (Explosions, Level Up, Quest Complete).
	 * * @param bReliable If TRUE, guarantees delivery (Critical). If FALSE, may be dropped on packet loss (FX/Spam).
	 */
	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category = "Dual Node Audio|Network", meta = (WorldContext = "WorldContextObject"))
	static void BroadcastSoundByTag_AtLocation(const UObject* WorldContextObject, FGameplayTag Tag, const FVector& Location, FRotator Rotation = FRotator::ZeroRotator, bool bReliable = false);

	/**
	 * Spawns an AudioComponent attached to a SceneComponent (e.g., a moving vehicle or character).
	 * * [NETWORK] LOCAL ONLY. The component exists only on this client.
	 * * @param AttachToComponent The component to follow.
	 * @param SocketName Optional socket name.
	 * @return The spawned Audio Component (can be used to stop/fade later).
	 */
	UFUNCTION(BlueprintCallable, Category = "Dual Node Audio|Components", meta = (WorldContext = "WorldContextObject"))
	static UAudioComponent* SpawnSoundByTag_Attached(const UObject* WorldContextObject, FGameplayTag Tag, USceneComponent* AttachToComponent, FName SocketName = NAME_None, bool bAutoDestroy = true);

	// ==========================================================================================
	// AI & BARKS
	// ==========================================================================================

	/**
	 * Attempts to play an AI Bark (Voice/Shout).
	 * Automatically handles Cooldown Groups and Random Trigger Chances defined in the Registry.
	 * * [NETWORK] LOCAL ONLY. If called on Server, only Server hears it.
	 * [USAGE] Call this inside local events or via Multicast/RepNotify from your AI Character.
	 * * @return True if the bark was played, False if blocked by cooldown or chance.
	 */
	UFUNCTION(BlueprintCallable, Category = "Dual Node Audio|AI", meta = (WorldContext = "WorldContextObject"))
	static bool PlayBarkByTag(const UObject* WorldContextObject, FGameplayTag BarkTag, const FVector& Location);


	// ==========================================================================================
	// GLOBAL MUSIC (SERVER AUTHORITY)
	// ==========================================================================================

	/**
	 * Sets the Global Music for ALL players. Replaces any track with lower priority.
	 * * [NETWORK] SERVER -> MULTICAST (Reliable). Must be called on Server (Authority).
	 * [SYNC] Includes Server Timestamp for sample-accurate playback on clients.
	 * * @param Priority The priority slot (e.g. Combat overrides Exploration).
	 * @param MusicTag The Music or Playlist Tag to play.
	 * @param Settings Optional runtime overrides (fade time, pitch, etc).
	 */
	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category = "Dual Node Audio|Music", meta = (WorldContext = "WorldContextObject"))
	static void SetGlobalMusic(const UObject* WorldContextObject, EDNAMusicPriority Priority, FGameplayTag MusicTag, FDualNodePlaybackSettings Settings);

	/**
	 * Clears (Stops) the music on the specified priority slot for ALL players.
	 * The system automatically fades back to the next highest active priority layer.
	 * * [NETWORK] SERVER -> MULTICAST (Reliable).
	 */
	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category = "Dual Node Audio|Music", meta = (WorldContext = "WorldContextObject"))
	static void ClearGlobalMusic(const UObject* WorldContextObject, EDNAMusicPriority Priority);

	/**
	 * Pauses the active music track for ALL players.
	 * Useful for opening Pause Menus or cinematics where silence is needed but resume is expected.
	 * * [NETWORK] SERVER -> MULTICAST (Reliable).
	 */
	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category = "Dual Node Audio|Music", meta = (WorldContext = "WorldContextObject"))
	static void PauseGlobalMusic(const UObject* WorldContextObject, EDNAMusicPriority Priority);

	/**
	 * Resumes the paused music track for ALL players.
	 * * [NETWORK] SERVER -> MULTICAST (Reliable).
	 */
	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category = "Dual Node Audio|Music", meta = (WorldContext = "WorldContextObject"))
	static void ResumeGlobalMusic(const UObject* WorldContextObject, EDNAMusicPriority Priority);

	/**
	 * Forces a Playlist to skip to the next track for ALL players.
	 * If the current track is not a playlist, nothing happens.
	 * * [NETWORK] SERVER -> MULTICAST (Reliable).
	 * [LOGIC] Supports Sequential, Random, and Smart Shuffle modes.
	 */
	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category = "Dual Node Audio|Music", meta = (WorldContext = "WorldContextObject"))
	static void SkipGlobalMusicTrack(const UObject* WorldContextObject, EDNAMusicPriority Priority);

	// ==========================================================================================
	// LOCAL MUSIC (CLIENT ONLY)
	// ==========================================================================================

	/**
	 * Sets music ONLY for the local player (Client).
	 * Useful for Main Menus, Inventory Screens, or Client-side events.
	 * * [NOTE] Local Music overrides Global Music if Priority is Equal or Higher.
	 * [NETWORK] LOCAL ONLY.
	 */
	UFUNCTION(BlueprintCallable, Category = "Dual Node Audio|Music", meta = (WorldContext = "WorldContextObject"))
	static void SetLocalMusic(const UObject* WorldContextObject, EDNAMusicPriority Priority, FGameplayTag MusicTag, FDualNodePlaybackSettings Settings);

	/**
	 * Clears local music for this client.
	 * * [NETWORK] LOCAL ONLY.
	 */
	UFUNCTION(BlueprintCallable, Category = "Dual Node Audio|Music", meta = (WorldContext = "WorldContextObject"))
	static void ClearLocalMusic(const UObject* WorldContextObject, EDNAMusicPriority Priority);

	/**
	 * Skips the next track on a local playlist.
	 * * [NETWORK] LOCAL ONLY.
	 */
	UFUNCTION(BlueprintCallable, Category = "Dual Node Audio|Music", meta = (WorldContext = "WorldContextObject"))
	static void SkipLocalMusicTrack(const UObject* WorldContextObject, EDNAMusicPriority Priority);

	// ==========================================================================================
	// TARGETED MUSIC (SERVER -> CLIENT)
	// ==========================================================================================

	/**
	 * Forces music playback on a SPECIFIC Client.
	 * * [NETWORK] SERVER -> CLIENT RPC (Reliable).
	 * [USAGE] Use for personal events like "You Died", "Level Up", or "Mission Failed".
	 * * @param TargetPlayer The PlayerController of the client who should hear the music.
	 */
	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category = "Dual Node Audio|Music")
	static void ClientMusicPush(APlayerController* TargetPlayer, EDNAMusicPriority Priority, FGameplayTag MusicTag, FDualNodePlaybackSettings Settings);
	
	/**
	 * Clears music on a SPECIFIC Client.
	 * * [NETWORK] SERVER -> CLIENT RPC (Reliable).
	 */
	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category = "Dual Node Audio|Music")
	static void ClientMusicClear(APlayerController* TargetPlayer, EDNAMusicPriority Priority);

	// ==========================================================================================
	// WORLD & ENVIRONMENT
	// ==========================================================================================

	/**
	 * Updates the internal Time-of-Day for the Audio System.
	 * Tracks with restricted hours will automatically fade in/out based on this value.
	 * * [NETWORK] NOT REPLICATED. You must call this locally on clients (e.g. from your Day/Night Cycle actor).
	 * * @param Hour The current time in 24h format (0.0 to 24.0).
	 */
	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category = "Dual Node Audio|World", meta = (WorldContext = "WorldContextObject"))
	static void SetSystemTimeOfDay(const UObject* WorldContextObject, float Hour);
};