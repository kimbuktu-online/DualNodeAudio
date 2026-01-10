// Copyright dual node interactive. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DeveloperSettings.h"
#include "DualNodeUISettings.generated.h"

class UUserWidget;

/**
 * Configure the default UI widgets for the DualNode framework.
 * Assign your project-specific widget Blueprints here.
 */
UCLASS(Config = Game, defaultconfig, meta = (DisplayName = "DualNode UI"))
class DUALNODEUI_API UDualNodeUISettings : public UDeveloperSettings
{
	GENERATED_BODY()

public:
	// The widget class to use for the main menu.
	UPROPERTY(Config, EditAnywhere, BlueprintReadOnly, Category = "Widgets")
	TSoftClassPtr<UUserWidget> MainMenuWidgetClass;

	// The widget class to use for the game lobby.
	UPROPERTY(Config, EditAnywhere, BlueprintReadOnly, Category = "Widgets")
	TSoftClassPtr<UUserWidget> LobbyWidgetClass;

	// The widget class for the developer connection menu.
	UPROPERTY(Config, EditAnywhere, BlueprintReadOnly, Category = "Widgets")
	TSoftClassPtr<UUserWidget> DevConnectWidgetClass;

	// The widget class for the main in-game HUD.
	UPROPERTY(Config, EditAnywhere, BlueprintReadOnly, Category = "Widgets")
	TSoftClassPtr<UUserWidget> HUDWidgetClass;

	static const UDualNodeUISettings* Get();
};
