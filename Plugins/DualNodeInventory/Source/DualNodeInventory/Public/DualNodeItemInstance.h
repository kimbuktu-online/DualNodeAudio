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
	FPrimaryAssetId ItemId;

	UPROPERTY(BlueprintReadOnly)
	int32 StackCount = 0;

	UPROPERTY(BlueprintReadOnly)
	FGuid InstanceGuid;

	UPROPERTY(NotReplicated, BlueprintReadOnly)
	TObjectPtr<const UDualNodeItemDefinition> CachedDefinition = nullptr;

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