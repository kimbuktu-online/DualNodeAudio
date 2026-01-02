#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GameplayTagContainer.h"
#include "DualNodeFoleyComponent.generated.h"

class UCharacterMovementComponent;
class UPhysicalMaterial;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class DUALNODEAUDIO_API UDualNodeFoleyComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UDualNodeFoleyComponent();

protected:
	virtual void BeginPlay() override;

	UFUNCTION()
	void OnLanded(const FHitResult& Hit);

public:
	// Tags Setup
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Foley")
	FGameplayTag WalkTag;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Foley")
	FGameplayTag SprintTag;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Foley")
	FGameplayTag LandTag;

	// Velocity Thresholds
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Foley")
	float MinVelocityForVolume = 100.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Foley")
	float MaxVelocityForVolume = 600.0f;

	// Trigger from AnimNotify
	UFUNCTION(BlueprintCallable, Category = "Foley")
	void TriggerFootstep();

private:
	UPROPERTY()
	TObjectPtr<UCharacterMovementComponent> MoveComp;

	UPhysicalMaterial* GetGroundMat() const;
};