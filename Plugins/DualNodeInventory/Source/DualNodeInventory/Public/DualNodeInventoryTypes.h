#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "DualNodeInventoryTypes.generated.h"

/** Hauptkategorien gemäß Enterprise-Anforderung */
UENUM(BlueprintType)
enum class EDualNodeMainCategory : uint8
{
	None,
	Resources,
	Equipment,
	Consumables,
	QuestItems
};

/** Beispiel für Sub-Kategorien (Resources) */
UENUM(BlueprintType)
enum class EDualNodeResourceSubCategory : uint8
{
	None,
	Wood,
	Ore,
	Fiber
};

/** * Rarity-Definition: Erlaubt datengetriebene Seltenheitsstufen.
 * Beinhaltet visuelle Parameter für das UI.
 */
UCLASS(BlueprintType)
class DUALNODEINVENTORY_API UDualNodeRarityDefinition : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Display")
	FText RarityName;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Display")
	FColor RarityColor = FColor::White;

	/** Optionales Pattern-Overlay für edlere Items (z.B. Epic/Legendary) */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Display")
	TObjectPtr<UTexture2D> RarityPattern;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Logic")
	int32 SortOrder = 0;
};