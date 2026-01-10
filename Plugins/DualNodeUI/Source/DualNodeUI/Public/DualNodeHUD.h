// Copyright dual node interactive. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "DualNodeHUD.generated.h"

/**
 * Base HUD class for DualNode projects.
 * Responsible for creating and holding the main HUD widget.
 */
UCLASS()
class DUALNODEUI_API ADualNodeHUD : public AHUD
{
	GENERATED_BODY()

protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly, Category = "DualNodeUI")
	TSubclassOf<class UUserWidget> HUDWidgetClass;

	UPROPERTY()
	TObjectPtr<UUserWidget> HUDWidget;
};
