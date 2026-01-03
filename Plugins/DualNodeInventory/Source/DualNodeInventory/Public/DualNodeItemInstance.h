#pragma once

#include "CoreMinimal.h"
#include "Net/Serialization/FastArraySerializer.h"
#include "DualNodeItemDefinition.h"
#include "DualNodeItemInstance.generated.h"

USTRUCT(BlueprintType)
struct FDualNodeItemInstance : public FFastArraySerializerItem
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<const UDualNodeItemDefinition> ItemDefinition = nullptr; // Node A Referenz [cite: 15]

	UPROPERTY(BlueprintReadOnly)
	int32 StackCount = 0; // [cite: 15]

	UPROPERTY(BlueprintReadOnly)
	int32 CurrentDurability = 0; // [cite: 15]

	UPROPERTY(BlueprintReadOnly)
	FGuid InstanceGuid; // Für Unique Items/Upgrades [cite: 5, 16]

	FDualNodeItemInstance() : InstanceGuid(FGuid::NewGuid()) {}
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