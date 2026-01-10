// Copyright dual node interactive. All Rights
// Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "DualNodeMainMenuWidget.generated.h"

/**
 * C++ base for the Main Menu widget.
 * Provides Blueprint-callable functions to host, find, and join games.
 */
UCLASS()
class DUALNODEUI_API UDualNodeMainMenuWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	UFUNCTION(BlueprintCallable, Category = "Main Menu")
	void HostGame(int32 MaxPlayers, const FString& ServerName);

	UFUNCTION(BlueprintCallable, Category = "Main Menu")
	void CreateParty();
};
