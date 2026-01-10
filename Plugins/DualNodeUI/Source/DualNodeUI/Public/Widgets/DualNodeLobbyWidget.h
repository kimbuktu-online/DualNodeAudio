// Copyright dual node interactive. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "DualNodeLobbyWidget.generated.h"

/**
 * C++ base for the Lobby widget.
 * Manages the display of players, ready status, and countdown timer.
 */
UCLASS()
class DUALNODEUI_API UDualNodeLobbyWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;

	UFUNCTION(BlueprintCallable, Category = "Lobby")
	void OnReadyButtonClicked();

	UFUNCTION(BlueprintCallable, Category = "Lobby")
	void OnStartGameButtonClicked();

	UFUNCTION()
	void OnLobbyCountdownChanged(int32 RemainingTime);

	/**
	 * Called in Blueprint to update the visual list of players.
	 * You should get the GameState's PlayerArray and display their names and ready status.
	 */
	UFUNCTION(BlueprintImplementableEvent, Category = "Lobby")
	void OnPlayerListChanged();

	/**
	 * Called in Blueprint to update the countdown timer display.
	 */
	UFUNCTION(BlueprintImplementableEvent, Category = "Lobby")
	void OnCountdownUpdated(int32 RemainingTime);

	/**
	 * Called in Blueprint to show or hide the "Start Game" button for the host.
	 */
	UFUNCTION(BlueprintImplementableEvent, Category = "Lobby")
	void SetStartGameButtonVisibility(bool bVisible);
};
