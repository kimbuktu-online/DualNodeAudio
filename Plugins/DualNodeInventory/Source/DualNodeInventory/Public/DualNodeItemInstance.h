#pragma once

#include "CoreMinimal.h"
#include "Net/Serialization/FastArraySerializer.h"
#include "DualNodeItemDefinition.h"
#include "DualNodeItemInstance.generated.h"

/** * Node B: Die dynamischen Instanzdaten eines Items.
 * Optimiert für Netzwerk-Replikation und Save-Games[cite: 14, 15, 39].
 */
USTRUCT(BlueprintType)
struct FDualNodeItemInstance : public FFastArraySerializerItem
{
	GENERATED_BODY()

	/** Die ID für den Asset Manager (Stabil für Save-Games) [cite: 39] */
	UPROPERTY(BlueprintReadOnly)
	FPrimaryAssetId ItemId;

	UPROPERTY(BlueprintReadOnly)
	int32 StackCount = 0;

	/** Einzigartige ID für dieses spezifische Item (Mods, Upgrades) [cite: 16] */
	UPROPERTY(BlueprintReadOnly)
	FGuid InstanceGuid;

	/** Transienter Pointer für schnellen Zugriff zur Laufzeit (Wird nicht repliziert/gespeichert) */
	UPROPERTY(NotReplicated, BlueprintReadOnly)
	TObjectPtr<const UDualNodeItemDefinition> CachedDefinition = nullptr;

	FDualNodeItemInstance() : InstanceGuid(FGuid::NewGuid()) {}

	// Hilfsmethode zum Auflösen der Definition via Asset Manager
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