// Copyright Dual Node.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "DualNodeSaveManagerSubsystem.generated.h"

UCLASS()
class DUALNODESAVEGAME_API UDualNodeSaveManagerSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	// UGameInstanceSubsystem interface begin
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;
	// UGameInstanceSubsystem interface end

	//~ Begin Replication
	virtual bool IsSupportedForNetworking() const override { return true; }
	//~ End Replication

	UFUNCTION(BlueprintCallable, Category = "DualNode Save Game")
	void SaveGame(const FString& SlotName);

	UFUNCTION(BlueprintCallable, Category = "DualNode Save Game")
	void LoadGame(const FString& SlotName);

	UFUNCTION(BlueprintCallable, Category = "DualNode Save Game")
	bool DoesSaveGameExist(const FString& SlotName);

protected:
	UFUNCTION(Server, Reliable, WithValidation)
	void Server_SaveGame(const FString& SlotName);
	bool Server_SaveGame_Validate(const FString& SlotName);
	void Server_SaveGame_Implementation(const FString& SlotName);

	UFUNCTION(Server, Reliable, WithValidation)
	void Server_LoadGame(const FString& SlotName);
	bool Server_LoadGame_Validate(const FString& SlotName);
	void Server_LoadGame_Implementation(const FString& SlotName);

	void ExecuteSave(const FString& SlotName);
	void ExecuteLoad(const FString& SlotName);

private:
	FString GetUniquePlayerIdForSaving(APlayerController* PlayerController);
};
