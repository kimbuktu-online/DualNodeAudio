#pragma once

#include "CoreMinimal.h"
#include "DualNodeItemFragment.h"
#include "DualNodeItemFragment_Mesh.generated.h"

class UStaticMesh;
class USkeletalMesh;

UCLASS()
class DUALNODEINVENTORY_API UDualNodeItemFragment_Mesh : public UDualNodeItemFragment
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, Category = "Visuals")
	TObjectPtr<UStaticMesh> StaticMesh;

	UPROPERTY(EditDefaultsOnly, Category = "Visuals")
	TObjectPtr<USkeletalMesh> SkeletalMesh;

	/** Skalierung in der Welt (z.B. für winzige Ringe oder riesige Erze) */
	UPROPERTY(EditDefaultsOnly, Category = "Visuals")
	FVector WorldScale = FVector(1.0f);
};