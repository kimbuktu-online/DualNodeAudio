// Copyright dual node interactive. All Rights Reserved.

#include "DualNodeHUD.h"
#include "Blueprint/UserWidget.h"

void ADualNodeHUD::BeginPlay()
{
	Super::BeginPlay();

	if (HUDWidgetClass)
	{
		if (APlayerController* PC = GetOwningPlayerController())
		{
			HUDWidget = CreateWidget<UUserWidget>(PC, HUDWidgetClass);
			if (HUDWidget)
			{
				HUDWidget->AddToViewport();
			}
		}
	}
}
