#pragma once

#include "CoreMinimal.h"
#include "Net/Serialization/FastArraySerializer.h"
#include "GameplayTagContainer.h"
#include "DualNodeItemDefinition.h"
#include "DualNodeItemInstance.generated.h"

USTRUCT(BlueprintType)
struct FDualNodeItemInstance : public FFastArraySerializerItem
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly, Category = "Item")
	FPrimaryAssetId ItemId;

	UPROPERTY(BlueprintReadOnly, Category = "Item")
	int32 StackCount = 0;

	UPROPERTY(BlueprintReadOnly, Category = "Item")
	FGuid InstanceGuid;

	UPROPERTY(BlueprintReadWrite, Category = "Item")
	FGameplayTagContainer DynamicTags;

	// --- V2.0 DYNAMIC DATA ---

	/** Aktuelle Haltbarkeit (für Use-Based Items wie Werkzeuge) */
	UPROPERTY(BlueprintReadWrite, Category = "Item|Durability")
	float CurrentDurability = 0.0f;

	/** Zeitpunkt des vollständigen Verfalls (Server-Time für Lebensmittel) */
	UPROPERTY(BlueprintReadWrite, Category = "Item|Durability")
	double ExpirationTimestamp = 0.0;

	/** Ankerpunkt im Grid (X = Spalte, Y = Zeile) */
	UPROPERTY(BlueprintReadWrite, Category = "Item|Spatial")
	FIntPoint GridLocation = FIntPoint(-1, -1);

	/** Markiert, ob das Item im Grid um 90 Grad gedreht wurde */
	UPROPERTY(BlueprintReadWrite, Category = "Item|Spatial")
	bool bIsRotated = false;

	// --- CACHING ---

	UPROPERTY(NotReplicated, BlueprintReadOnly, Category = "Item")
	TObjectPtr<UDualNodeItemDefinition> CachedDefinition = nullptr;

	FDualNodeItemInstance() : InstanceGuid(FGuid::NewGuid()) {}

	bool ResolveDefinition();
};

USTRUCT(BlueprintType)
struct FDualNodeInventoryArray : public FFastArraySerializer
{
	GENERATED_BODY()

	UPROPERTY()
	TArray<FDualNodeItemInstance> Items;

	UPROPERTY(NotReplicated)
	TObjectPtr<class UDualNodeInventoryComponent> OwnerComponent;

	bool NetDeltaSerialize(FNetDeltaSerializeInfo& DeltaParms)
	{
		return FFastArraySerializer::FastArrayDeltaSerialize<FDualNodeItemInstance, FDualNodeInventoryArray>(Items, DeltaParms, *this);
	}
};

template<>
struct TStructOpsTypeTraits<FDualNodeInventoryArray> : public TStructOpsTypeTraitsBase2<FDualNodeInventoryArray>
{
	enum { WithNetDeltaSerializer = true };
};