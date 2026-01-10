// Copyright dual node interactive. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "DualNodeCore/Public/DualNodeCoreInterfaces.h" // Include the interface we are implementing
#include "DualNodeDevConnectSubsystem.generated.h"

/**
 * This subsystem manages developer access and provides the DevConnect functionality.
 * It automatically registers itself with the DualNodeCore module.
 */
UCLASS()
class DUALNODEDEVCONNECT_API UDualNodeDevConnectSubsystem : public UGameInstanceSubsystem, public IDualNodeDevConnectInterface
{
	GENERATED_BODY()

public:
	//~================================================================================================================
	// Subsystem Lifecycle
	//~================================================================================================================
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;

	//~================================================================================================================
	// IDualNodeDevConnectInterface implementation
	//~================================================================================================================
	virtual bool IsDeveloper(const FUniqueNetIdRepl& UniqueId) const override;
	virtual bool OnDeveloperPostLogin(APlayerController* NewPlayer) override;
};
