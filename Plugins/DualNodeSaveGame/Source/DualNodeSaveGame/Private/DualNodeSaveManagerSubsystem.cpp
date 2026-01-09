// Copyright Dual Node.

#include "DualNodeSaveManagerSubsystem.h"
#include "DualNodeSaveGameSettings.h"
#include "DualNodeSaveGameData.h"
#include "DualNodeSavableActor.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/PlayerState.h"
#include "OnlineSubsystem.h"
#include "Interfaces/OnlineIdentityInterface.h"
#include "SteamUser/SteamUser.h" // Corrected include path

void UDualNodeSaveManagerSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
	UE_LOG(LogTemp, Log, TEXT("DualNodeSaveManagerSubsystem Initialized"));
}

void UDualNodeSaveManagerSubsystem::Deinitialize()
{
	Super::Deinitialize();
}

void UDualNodeSaveManagerSubsystem::SaveGame(const FString& SlotName)
{
	if (GetWorld()->GetNetMode() == NM_Client)
	{
		Server_SaveGame(SlotName);
	}
	else
	{
		ExecuteSave(SlotName);
	}
}

void UDualNodeSaveManagerSubsystem::LoadGame(const FString& SlotName)
{
	if (GetWorld()->GetNetMode() == NM_Client)
	{
		Server_LoadGame(SlotName);
	}
	else
	{
		ExecuteLoad(SlotName);
	}
}

bool UDualNodeSaveManagerSubsystem::Server_SaveGame_Validate(const FString& SlotName) { return true; }
void UDualNodeSaveManagerSubsystem::Server_SaveGame_Implementation(const FString& SlotName)
{
	ExecuteSave(SlotName);
}

bool UDualNodeSaveManagerSubsystem::Server_LoadGame_Validate(const FString& SlotName) { return true; }
void UDualNodeSaveManagerSubsystem::Server_LoadGame_Implementation(const FString& SlotName)
{
	ExecuteLoad(SlotName);
}

void UDualNodeSaveManagerSubsystem::ExecuteSave(const FString& SlotName)
{
	APlayerController* PC = GetGameInstance()->GetFirstLocalPlayerController();
	if (!PC) return;

	FString UniquePlayerId = GetUniquePlayerIdForSaving(PC);
	if (UniquePlayerId.IsEmpty())
	{
		UE_LOG(LogTemp, Error, TEXT("Could not get a unique player ID for saving."));
		return;
	}

	const FString FullSlotName = UniquePlayerId + "_" + SlotName;

	UDualNodeSaveGame* SaveGameObject = Cast<UDualNodeSaveGame>(UGameplayStatics::CreateSaveGameObject(UDualNodeSaveGame::StaticClass()));
	if (!SaveGameObject) return;

	SaveGameObject->SlotName = FullSlotName;
	SaveGameObject->SaveDate = FDateTime::UtcNow();

	TArray<AActor*> SavableActors;
	UGameplayStatics::GetAllActorsWithInterface(GetWorld(), UDualNodeSavableActor::StaticClass(), SavableActors);

	for (AActor* Actor : SavableActors)
	{
		FActorSaveData ActorSaveData = IDualNodeSavableActor::Execute_GetActorSaveData(Actor);
		ActorSaveData.ActorName = Actor->GetFName();
		ActorSaveData.ActorClass = Actor->GetClass();
		ActorSaveData.ActorTransform = Actor->GetTransform();
		SaveGameObject->SavedActors.Add(ActorSaveData);
	}

	UGameplayStatics::AsyncSaveGameToSlot(SaveGameObject, FullSlotName, 0);
}

void UDualNodeSaveManagerSubsystem::ExecuteLoad(const FString& SlotName)
{
	APlayerController* PC = GetGameInstance()->GetFirstLocalPlayerController();
	if (!PC) return;

	FString UniquePlayerId = GetUniquePlayerIdForSaving(PC);
	if (UniquePlayerId.IsEmpty())
	{
		UE_LOG(LogTemp, Error, TEXT("Could not get a unique player ID for loading."));
		return;
	}

	const FString FullSlotName = UniquePlayerId + "_" + SlotName;

	if (!DoesSaveGameExist(FullSlotName))
	{
		UE_LOG(LogTemp, Warning, TEXT("Save game slot '%s' not found."), *FullSlotName);
		return;
	}

	FAsyncLoadGameFromSlotDelegate LoadDelegate;
	LoadDelegate.BindLambda([this](const FString& SlotName, const int32 UserIndex, USaveGame* LoadedGame)
	{
		UDualNodeSaveGame* LoadGameObject = Cast<UDualNodeSaveGame>(LoadedGame);
		if (!LoadGameObject) return;

		TMap<FName, FActorSaveData> ActorSaveDataMap;
		for (const FActorSaveData& ActorData : LoadGameObject->SavedActors)
		{
			ActorSaveDataMap.Add(ActorData.ActorName, ActorData);
		}

		TArray<AActor*> SavableActors;
		UGameplayStatics::GetAllActorsWithInterface(GetWorld(), UDualNodeSavableActor::StaticClass(), SavableActors);

		for (AActor* Actor : SavableActors)
		{
			if (ActorSaveDataMap.Contains(Actor->GetFName()))
			{
				const FActorSaveData& ActorData = ActorSaveDataMap[Actor->GetFName()];
				Actor->SetActorTransform(ActorData.ActorTransform);
				IDualNodeSavableActor::Execute_LoadActorSaveData(Actor, ActorData);
			}
		}
	});

	UGameplayStatics::AsyncLoadGameFromSlot(FullSlotName, 0, LoadDelegate);
}

bool UDualNodeSaveManagerSubsystem::DoesSaveGameExist(const FString& SlotName)
{
	APlayerController* PC = GetGameInstance()->GetFirstLocalPlayerController();
	if (!PC) return false;

	FString UniquePlayerId = GetUniquePlayerIdForSaving(PC);
	if (UniquePlayerId.IsEmpty()) return false;

	return UGameplayStatics::DoesSaveGameExist(UniquePlayerId + "_" + SlotName, 0);
}

FString UDualNodeSaveManagerSubsystem::GetUniquePlayerIdForSaving(APlayerController* PlayerController)
{
    if (!IsValid(PlayerController))
    {
        return FString();
    }

    const UDualNodeSaveGameSettings* Settings = GetDefault<UDualNodeSaveGameSettings>();

    switch (Settings->SaveSubsystemStrategy)
    {
        case EDualNodeSaveSubsystemStrategy::SteamCorePro:
        {
            if (FModuleManager::Get().IsModuleLoaded("SteamCorePro"))
            {
				// Correct way to get the SteamID as a string
				return USteamProUser::GetSteamIDAsString();
            }
            UE_LOG(LogTemp, Warning, TEXT("SteamCore-Pro selected but could not retrieve SteamID!"));
            return FString();
        }

        case EDualNodeSaveSubsystemStrategy::Default:
        default:
        {
			if (!IsValid(PlayerController->PlayerState))
			{
				return FString();
			}

            IOnlineSubsystem* OnlineSub = IOnlineSubsystem::Get();
            if (OnlineSub)
            {
                IOnlineIdentityPtr Identity = OnlineSub->GetIdentityInterface();
                if (Identity.IsValid())
                {
                    TSharedPtr<const FUniqueNetId> UniqueId = Identity->GetUniquePlayerId(PlayerController->PlayerState->GetUniqueID());
                    if (UniqueId.IsValid())
                    {
                        return UniqueId->ToString();
                    }
                }
            }
            return PlayerController->PlayerState->GetPlayerName() + "_local";
        }
    }
}
