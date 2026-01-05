#pragma once

#include "CoreMinimal.h"
#include "DualNodeItemFragment.h"
#include "DualNodeItemFragment_Spatial.generated.h"

UCLASS(meta=(DisplayName="Fragment: Spatial Dimensions"))
class DUALNODEINVENTORY_API UDualNodeItemFragment_Spatial : public UDualNodeItemFragment
{
	GENERATED_BODY()

public:
	/** Breite und Höhe des Items im Grid (z.B. 1x1, 2x3) */
	UPROPERTY(EditDefaultsOnly, Category = "Spatial")
	FIntPoint Dimensions = FIntPoint(1, 1);

	/** Darf das Item im Grid rotiert werden? */
	UPROPERTY(EditDefaultsOnly, Category = "Spatial")
	bool bCanRotate = true;
};