#include "DualNodeInteractionComponent.h"
#include "DualNodeInteractionInterface.h"
#include "DualNodeInventorySettings.h"
#include "GameFramework/Pawn.h"
#include "GameFramework/PlayerController.h"

UDualNodeInteractionComponent::UDualNodeInteractionComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	PrimaryComponentTick.TickInterval = 0.1f; // Performance: Trace nur 10x pro Sekunde
}

void UDualNodeInteractionComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	TraceForInteractables();
}

void UDualNodeInteractionComponent::TraceForInteractables()
{
	AActor* Owner = GetOwner();
	if (!Owner) return;

	APlayerController* PC = nullptr;
	if (APawn* PawnOwner = Cast<APawn>(Owner))
	{
		PC = Cast<APlayerController>(PawnOwner->GetController());
	}
	
	if (!PC) return;

	// DNA 2.2: Settings abrufen
	const UDualNodeInventorySettings* Settings = UDualNodeInventorySettings::Get();
	float TraceRange = Settings ? Settings->DefaultInteractionRange : InteractionRange;
	ECollisionChannel TraceChannel = Settings ? Settings->InteractionTraceChannel.GetValue() : InteractionChannel.GetValue();

	FVector CameraLocation;
	FRotator CameraRotation;
	PC->GetPlayerViewPoint(CameraLocation, CameraRotation);

	const FVector TraceStart = CameraLocation;
	const FVector TraceEnd = TraceStart + (CameraRotation.Vector() * TraceRange);

	FHitResult HitResult;
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(Owner);

	AActor* NewFocus = nullptr;

	if (GetWorld()->LineTraceSingleByChannel(HitResult, TraceStart, TraceEnd, TraceChannel, Params))
	{
		AActor* HitActor = HitResult.GetActor();
		if (HitActor && HitActor->Implements<UDualNodeInteractionInterface>())
		{
			if (IDualNodeInteractionInterface::Execute_CanInteract(HitActor, Owner))
			{
				NewFocus = HitActor;
			}
		}
	}

	if (NewFocus != CurrentFocusedActor)
	{
		CurrentFocusedActor = NewFocus;
		OnFocusedActorChanged.Broadcast(CurrentFocusedActor);
	}
}

void UDualNodeInteractionComponent::PerformInteraction()
{
	if (CurrentFocusedActor && CurrentFocusedActor->Implements<UDualNodeInteractionInterface>())
	{
		IDualNodeInteractionInterface::Execute_Interact(CurrentFocusedActor, GetOwner());
	}
}