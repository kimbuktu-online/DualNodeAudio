#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "DualNodeInventoryTypes.generated.h"

/** Bestimmt, wie das Inventar intern validiert und im UI dargestellt wird */
UENUM(BlueprintType)
enum class EDualNodeInventoryType : uint8
{
	Classic UMETA(DisplayName = "Classic (Slot-based)"),
	Spatial UMETA(DisplayName = "Spatial (Grid-based)")
};

/** Bestimmt die Art des Verfalls für Items */
UENUM(BlueprintType)
enum class EDualNodeDurabilityType : uint8
{
	None,
	UseBased   UMETA(DisplayName = "Abnutzung bei Benutzung (z.B. Werkzeug)"),
	TimeBased  UMETA(DisplayName = "Zeitbasierter Verfall (z.B. Lebensmittel)")
};

UENUM(BlueprintType)
enum class EDualNodeMainCategory : uint8
{
	None,
	Resources,
	Equipment,
	Consumables,
	QuestItems
};


UENUM(BlueprintType)
enum class EDualNodeResourceSubCategory : uint8
{
	None,
	Wood,
	Ore,
	Fiber
};

/** Datensatz für ein einzelnes Item im Save-Game */
USTRUCT(BlueprintType)
struct FDualNodeItemSaveData
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite)
	FPrimaryAssetId ItemId;

	UPROPERTY(BlueprintReadWrite)
	int32 StackCount = 0;

	UPROPERTY(BlueprintReadWrite)
	FGuid InstanceGuid;
};

/** Vollständiger Inventar-Snapshot für Save-Games */
USTRUCT(BlueprintType)
struct FDualNodeInventorySaveData
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite)
	TArray<FDualNodeItemSaveData> SavedItems;
};

UCLASS(BlueprintType)
class DUALNODEINVENTORY_API UDualNodeRarityDefinition : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Display")
	FText RarityName;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Display")
	FColor RarityColor = FColor::White;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Display")
	TObjectPtr<UTexture2D> RarityPattern;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Logic")
	int32 SortOrder = 0;
};