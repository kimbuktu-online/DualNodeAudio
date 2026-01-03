#pragma once

#include "CoreMinimal.h"
#include "DualNodeItemFragment.h"
#include "GameplayTagContainer.h"
#include "Sound/SoundBase.h"
#include "DualNodeItemFragment_Audio.generated.h"

UCLASS()
class DUALNODEINVENTORY_API UDualNodeItemFragment_Audio : public UDualNodeItemFragment
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, Category="Audio")
	TMap<FGameplayTag, TObjectPtr<USoundBase>> SoundMap;
};