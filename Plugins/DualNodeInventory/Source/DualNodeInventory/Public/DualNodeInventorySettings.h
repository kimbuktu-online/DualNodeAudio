#pragma once

#include "CoreMinimal.h"
#include "Engine/DeveloperSettings.h"
#include "GameplayTagContainer.h"
#include "DualNodeInventoryTypes.h"
#include "Engine/EngineTypes.h"
#include "DualNodeInventorySettings.generated.h"

/**
 * DNA 2.2 - Globale Projekt-Einstellungen für das Inventar-System.
 */
UCLASS(Config=Game, DefaultConfig, meta=(DisplayName="Dual Node Inventory"))
class DUALNODEINVENTORY_API UDualNodeInventorySettings : public UDeveloperSettings
{
	GENERATED_BODY()

public:
	UDualNodeInventorySettings();

	static const UDualNodeInventorySettings* Get() { return GetDefault<UDualNodeInventorySettings>(); }

	// --- ALLGEMEIN ---
	UPROPERTY(Config, EditAnywhere, Category="General")
	int32 GlobalDefaultMaxStackSize = 99;

	UPROPERTY(Config, EditAnywhere, Category="General")
	int32 DefaultMaxSlotCount = 20;

	UPROPERTY(Config, EditAnywhere, Category="General")
	int32 DefaultHUDSlotCount = 5;

	// --- INTERAKTION ---
	UPROPERTY(Config, EditAnywhere, Category="Interaction")
	float DefaultInteractionRange = 250.0f;

	UPROPERTY(Config, EditAnywhere, Category="Interaction")
	TEnumAsByte<ECollisionChannel> InteractionTraceChannel = ECC_Visibility;

	// --- DROP LOGIK ---
	/** Wie weit vor dem Spieler soll ein Item gespawnt werden? */
	UPROPERTY(Config, EditAnywhere, Category="World Operations")
	float ItemDropForwardOffset = 120.0f;

	// --- VALIDIERUNG ---
	UPROPERTY(Config, EditAnywhere, Category="Validation")
	float GlobalDefaultMaxWeight = 100.0f;

	// --- IDENTITY ---
	UPROPERTY(Config, EditAnywhere, Category="Identity")
	TMap<FGameplayTag, TSoftObjectPtr<class UDualNodeRarityDefinition>> RarityRegistry;

	// --- AUDIO ---
	UPROPERTY(Config, EditAnywhere, Category="Audio")
	FGameplayTag DefaultPickupTag;

	// --- DEBUGGING ---
	UPROPERTY(Config, EditAnywhere, Category="Debugging")
	bool bDrawDebugStatsOnScreen = false;

	UPROPERTY(Config, EditAnywhere, Category="Debugging")
	FColor DebugTextColor = FColor::Cyan;

	UPROPERTY(Config, EditAnywhere, Category="Debugging")
	bool bLogInventoryOperations = true;
};