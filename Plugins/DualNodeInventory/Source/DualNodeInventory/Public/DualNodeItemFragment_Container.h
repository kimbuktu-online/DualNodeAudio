#pragma once

#include "CoreMinimal.h"
#include "DualNodeItemFragment.h"
#include "DualNodeItemFragment_Container.generated.h"

/**
 * Fragment, das einem Item die Eigenschaft gibt, ein Behälter mit zusätzlichen Slots zu sein.
 */
UCLASS()
class DUALNODEINVENTORY_API UDualNodeItemFragment_Container : public UDualNodeItemFragment
{
	GENERATED_BODY()

public:
	// Die Anzahl der Slots, die dieser Behälter bereitstellt.
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Container", meta = (UIMin = "0", ClampMin = "0"))
	int32 SlotCount = 0;
};