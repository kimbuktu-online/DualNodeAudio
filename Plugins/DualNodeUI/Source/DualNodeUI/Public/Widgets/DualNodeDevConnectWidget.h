// Copyright dual node interactive. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "DualNodeCore/Public/DualNodeCoreGameInstance.h" // For FServerInfo
#include "DualNodeDevConnectWidget.generated.h"

/**
 * The C++ base for the developer connection widget.
 * Provides functionality to find and join any active session.
 */
UCLASS()
class DUALNODEUI_API UDualNodeDevConnectWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;

	UFUNCTION(BlueprintCallable, Category = "DevConnect")
	void FindServers();

	UFUNCTION(BlueprintCallable, Category = "DevConnect")
	void JoinServer(FServerInfo ServerToJoin);

	UFUNCTION()
	void OnServersFound(const TArray<FServerInfo>& ServerList);

	/**
	 * This function must be implemented in the Blueprint widget.
	 * It is called when the server list has been updated.
	 */
	UFUNCTION(BlueprintImplementableEvent, Category = "DevConnect")
	void OnServerListUpdated(const TArray<FServerInfo>& Servers);

private:
	UPROPERTY()
	TObjectPtr<UDualNodeCoreGameInstance> CoreGameInstance;
};
