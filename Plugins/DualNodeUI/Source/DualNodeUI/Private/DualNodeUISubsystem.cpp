// Copyright dual node interactive. All Rights Reserved.

#include "DualNodeUISubsystem.h"
#include "DualNodeCore/Public/DualNodeCoreGameState.h"
#include "DualNodeUISettings.h"
#include "Blueprint/UserWidget.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/World.h"

void UDualNodeUISubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

	// Bind to the delegate that is called when a new world is created and initialized.
	FWorldDelegates::OnPostWorldInitialization.AddUObject(this, &UDualNodeUISubsystem::OnWorldInitialized);
}

void UDualNodeUISubsystem::OnWorldInitialized(UWorld* World, const UWorld::InitializationValues IVS)
{
    if (World && World->IsGameWorld())
    {
        // When a new world is ready, get its GameState.
        if (ADualNodeCoreGameState* GameState = Cast<ADualNodeCoreGameState>(World->GetGameState()))
        {
            // Unbind from the old GameState if it exists
            if (CurrentGameState.IsValid())
            {
                CurrentGameState->OnGamePhaseChanged.RemoveDynamic(this, &UDualNodeUISubsystem::OnGamePhaseChanged);
            }

            // Bind to the new GameState's delegate
            GameState->OnGamePhaseChanged.AddUniqueDynamic(this, &UDualNodeUISubsystem::OnGamePhaseChanged);
            CurrentGameState = GameState;

			// Immediately trigger a UI update with the current phase
			OnGamePhaseChanged(GameState->GetGamePhase());
        }
    }
}

void UDualNodeUISubsystem::Deinitialize()
{
	FWorldDelegates::OnPostWorldInitialization.RemoveAll(this);
	Super::Deinitialize();
}

void UDualNodeUISubsystem::ShowWidget(TSubclassOf<UUserWidget> WidgetClass)
{
	if (!WidgetClass)
	{
		return;
	}

	if (UUserWidget** FoundWidget = ActiveWidgets.Find(WidgetClass))
	{
		if (*FoundWidget && !(*FoundWidget)->IsInViewport())
		{
			(*FoundWidget)->AddToViewport();
		}
		return;
	}

	if (APlayerController* PC = UGameplayStatics::GetPlayerController(GetWorld(), 0))
	{
		UUserWidget* NewWidget = CreateWidget<UUserWidget>(PC, WidgetClass);
		if (NewWidget)
		{
			NewWidget->AddToViewport();
			ActiveWidgets.Add(WidgetClass, NewWidget);
		}
	}
}

void UDualNodeUISubsystem::HideWidget(UUserWidget* Widget)
{
	if (Widget && Widget->IsInViewport())
	{
		Widget->RemoveFromParent();
	}
}

void UDualNodeUISubsystem::ToggleDevConnectMenu()
{
	if (DevConnectWidget && DevConnectWidget->IsInViewport())
	{
		HideWidget(DevConnectWidget);
		DevConnectWidget = nullptr;
	}
	else
	{
		if (const UDualNodeUISettings* Settings = UDualNodeUISettings::Get())
		{
			if (TSubclassOf<UUserWidget> WidgetClass = Settings->DevConnectWidgetClass.LoadSynchronous())
			{
				if (APlayerController* PC = UGameplayStatics::GetPlayerController(GetWorld(), 0))
				{
					DevConnectWidget = CreateWidget<UUserWidget>(PC, WidgetClass);
					if(DevConnectWidget)
					{
						DevConnectWidget->AddToViewport(100); // High Z-Order to be on top
					}
				}
			}
		}
	}
}

void UDualNodeUISubsystem::OnGamePhaseChanged(EGamePhase NewPhase)
{
	// Hide all active widgets first
	for (auto& Elem : ActiveWidgets)
	{
		HideWidget(Elem.Value);
	}
	ActiveWidgets.Empty();

	UE_LOG(LogTemp, Log, TEXT("UI Subsystem received GamePhase change: %s"), *UEnum::GetValueAsString(NewPhase));

	const UDualNodeUISettings* Settings = UDualNodeUISettings::Get();
	if (!Settings) return;

	TSubclassOf<UUserWidget> WidgetClassToShow = nullptr;

	switch (NewPhase)
	{
	case EGamePhase::MainMenu:
		WidgetClassToShow = Settings->MainMenuWidgetClass.LoadSynchronous();
		break;
	case EGamePhase::Lobby:
		WidgetClassToShow = Settings->LobbyWidgetClass.LoadSynchronous();
		break;
	case EGamePhase::Gameplay:
		// The HUD is managed by the ADualNodeHUD class, not the subsystem.
		break;
	default:
		break;
	}

	if (WidgetClassToShow)
	{
		ShowWidget(WidgetClassToShow);
	}
}
