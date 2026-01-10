// Copyright dual node interactive. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "DualNodeCore/Public/DualNodeCoreTypes.h" // For EGamePhase
#include "DualNodeUISubsystem.generated.h"

class UUserWidget;
class UWorld;
class ADualNodeCoreGameState;

/**
 * The central manager for all UI in a DualNode project.
 * This subsystem listens to gameplay events and shows/hides widgets accordingly.
 */
UCLASS()
class DUALNODEUI_API UDualNodeUISubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;

	UFUNCTION(BlueprintCallable, Category = "DualNodeUI")
	void ShowWidget(TSubclassOf<UUserWidget> WidgetClass);

	UFUNCTION(BlueprintCallable, Category = "DualNodeUI")
	void HideWidget(UUserWidget* Widget);

	UFUNCTION(BlueprintCallable, Category = "DualNodeUI")
	void ToggleDevConnectMenu();

protected:
	// Called when a new world is initialized
	void OnWorldInitialized(UWorld* World, const UWorld::InitializationValues IVS);

	//~================================================================================================================
	// Delegate Handlers
	//~================================================================================================================

	UFUNCTION()
	void OnGamePhaseChanged(EGamePhase NewPhase);

private:
	UPROPERTY()
	TMap<TSubclassOf<UUserWidget>, UUserWidget*> ActiveWidgets;

	UPROPERTY()
	TObjectPtr<UUserWidget> DevConnectWidget;

	// Keep a reference to the current GameState to unbind delegates later
	UPROPERTY()
	TWeakObjectPtr<ADualNodeCoreGameState> CurrentGameState;
};
