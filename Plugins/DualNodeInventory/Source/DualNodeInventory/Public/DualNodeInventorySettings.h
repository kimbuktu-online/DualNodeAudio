#pragma once

#include "CoreMinimal.h"
#include "Engine/DeveloperSettings.h"
#include "GameplayTagContainer.h"
#include "DualNodeInventoryTypes.h" // Für Rarity Definition
#include "DualNodeInventorySettings.generated.h"

UCLASS(Config=Game, DefaultConfig, meta=(DisplayName="Dual Node Inventory"))
class DUALNODEINVENTORY_API UDualNodeInventorySettings : public UDeveloperSettings
{
	GENERATED_BODY()

public:
	UDualNodeInventorySettings();

	/** Standard-Stackgröße für neue Items, falls im Data Asset nichts anderes steht */
	UPROPERTY(Config, EditAnywhere, Category="General")
	int32 GlobalDefaultMaxStackSize = 99;

	/** Aktiviert On-Screen Debugging für Inventar-Operationen */
	UPROPERTY(Config, EditAnywhere, Category="Debugging")
	bool bDrawDebugStatsOnScreen = false;

	UPROPERTY(Config, EditAnywhere, Category="Debugging")
	FColor DebugTextColor = FColor::Cyan;

	/** Mapping von Seltenheits-Tags zu deren Definitionen (Zentraler Designer-Hub) */
	UPROPERTY(Config, EditAnywhere, Category="Identity")
	TMap<FGameplayTag, TSoftObjectPtr<class UDualNodeRarityDefinition>> RarityRegistry;

	UPROPERTY(Config, EditAnywhere, Category="Audio")
	FGameplayTag DefaultPickupTag;
};