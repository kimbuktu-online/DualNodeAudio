// Copyright dual node interactive. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "DualNodeUIBlueprintLibrary.generated.h"

class APlayerState;

/**
 * A library of UI-related helper functions for Blueprints.
 */
UCLASS()
class DUALNODEUI_API UDualNodeUIBlueprintLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	/**
	 * Gets the Steam avatar for a given PlayerState.
	 * This is an asynchronous operation; the texture will be returned when it's loaded.
	 * @param PlayerState The PlayerState of the player whose avatar you want.
	 * @return A Texture2D object for the avatar, or nullptr if not available.
	 */
	UFUNCTION(BlueprintCallable, Category = "DualNodeUI|Steam")
	static UTexture2D* GetSteamAvatar(APlayerState* PlayerState);
};
