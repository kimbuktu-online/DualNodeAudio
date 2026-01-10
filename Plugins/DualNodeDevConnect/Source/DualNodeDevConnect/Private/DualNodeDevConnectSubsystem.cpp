// Copyright dual node interactive. All Rights Reserved.

#include "DualNodeDevConnectSubsystem.h"
#include "DualNodeDevConnectSettings.h"
#include "DualNodeCore/Public/DualNodeCore.h" // For registration
#include "GameFramework/GameModeBase.h"
#include "GameFramework/SpectatorPawn.h"
#include "GameFramework/PlayerState.h"

void UDualNodeDevConnectSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

	// Register this subsystem with the Core module so the GameMode can find it.
	FDualNodeCoreModule::RegisterDevConnectInterface(this);
	UE_LOG(LogTemp, Log, TEXT("DualNodeDevConnectSubsystem initialized and registered with DualNodeCore."));
}

void UDualNodeDevConnectSubsystem::Deinitialize()
{
	// Unregister to prevent dangling pointers.
	FDualNodeCoreModule::RegisterDevConnectInterface(nullptr);
	UE_LOG(LogTemp, Log, TEXT("DualNodeDevConnectSubsystem deinitialized and unregistered."));

	Super::Deinitialize();
}

bool UDualNodeDevConnectSubsystem::IsDeveloper(const FUniqueNetIdRepl& UniqueId) const
{
	if (!UniqueId.IsValid() || !UniqueId.GetUniqueNetId().IsValid())
	{
		return false;
	}

	const FString PlayerSteamID = UniqueId.GetUniqueNetId()->ToString();
	
	const UDualNodeDevConnectSettings* Settings = UDualNodeDevConnectSettings::Get();
	if (Settings)
	{
		for (const FString& DevID : Settings->DeveloperSteamIDs)
		{
			if (PlayerSteamID.Equals(DevID, ESearchCase::IgnoreCase))
			{
				return true;
			}
		}
	}

	return false;
}

bool UDualNodeDevConnectSubsystem::OnDeveloperPostLogin(APlayerController* NewPlayer)
{
	if (!NewPlayer)
	{
		return false;
	}

	AGameModeBase* GameMode = GetWorld()->GetAuthGameMode();
	if (GameMode)
	{
		// Set the player to spectate.
		NewPlayer->PlayerState->SetIsSpectator(true);
		NewPlayer->ChangeState(NAME_Spectating);
		
		// Spawn a spectator pawn for the developer at a suitable start point.
		AActor* StartSpot = GameMode->FindPlayerStart(NewPlayer);
		ASpectatorPawn* SpectatorPawn = GetWorld()->SpawnActor<ASpectatorPawn>(GameMode->SpectatorClass, StartSpot->GetActorTransform());
		
		if (SpectatorPawn)
		{
			NewPlayer->Possess(SpectatorPawn);
			return true; // We handled the login.
		}
	}

	return false;
}
