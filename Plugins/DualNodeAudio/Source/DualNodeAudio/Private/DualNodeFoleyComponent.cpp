#include "DualNodeFoleyComponent.h"
#include "DualNodeAudioSubsystem.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "PhysicalMaterials/PhysicalMaterial.h"
#include "DualNodeAudioSettings.h"

UDualNodeFoleyComponent::UDualNodeFoleyComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
	
	// Defaults
	WalkTag = FGameplayTag::RequestGameplayTag(FName("Audio.SFX.Footstep.Walk"));
	SprintTag = FGameplayTag::RequestGameplayTag(FName("Audio.SFX.Footstep.Sprint"));
	LandTag = FGameplayTag::RequestGameplayTag(FName("Audio.SFX.Impact.Land"));
}

void UDualNodeFoleyComponent::BeginPlay()
{
	Super::BeginPlay();

	if (ACharacter* Char = Cast<ACharacter>(GetOwner()))
	{
		MoveComp = Char->GetCharacterMovement();
		Char->LandedDelegate.AddDynamic(this, &UDualNodeFoleyComponent::OnLanded);
	}
}

void UDualNodeFoleyComponent::OnLanded(const FHitResult& Hit)
{
	if (LandTag.IsValid())
	{
		if (UGameInstance* GI = GetWorld()->GetGameInstance())
		{
			if (UDualNodeAudioSubsystem* Sys = GI->GetSubsystem<UDualNodeAudioSubsystem>())
			{
				FDualNodeSoundDefinition Def;
				if (Sys->GetSoundDefinition(LandTag, Def))
				{
					// Resolve Physics Asset (Async safe, returns SoftPtr)
					TSoftObjectPtr<USoundBase> SoundToPlay = Sys->ResolveSoundFromPhysics(Def, Hit.PhysMaterial.Get());
					
					// Velocity Impact Volume
					float ImpactVol = 1.0f;
					if (MoveComp)
					{
						float ZVel = FMath::Abs(MoveComp->Velocity.Z);
						ImpactVol = FMath::GetMappedRangeValueClamped(FVector2D(500.f, 2000.f), FVector2D(0.5f, 1.5f), ZVel);
					}

					// Use new Overload to play specific asset instead of Tag default
					Sys->PlaySoundAtLocation(this, SoundToPlay, Hit.Location, FRotator::ZeroRotator, ImpactVol, 1.0f, &Def);
				}
			}
		}
	}
}

void UDualNodeFoleyComponent::TriggerFootstep()
{
	if (!MoveComp) return;
	if (MoveComp->IsFalling()) return;

	// Velocity Check
	float Vel = MoveComp->Velocity.Size2D();
	if (Vel < 10.0f) return;

	FGameplayTag TagToUse = (Vel > 450.f && SprintTag.IsValid()) ? SprintTag : WalkTag;
	if (!TagToUse.IsValid()) return;

	float VolumeMult = FMath::GetMappedRangeValueClamped(
		FVector2D(MinVelocityForVolume, MaxVelocityForVolume), 
		FVector2D(0.5f, 1.2f), 
		Vel
	);

	// Raycast for PhysMat
	FHitResult Hit;
	FVector Start = GetOwner()->GetActorLocation();
	FVector End = Start - FVector(0, 0, 150.0f);
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(GetOwner());
	Params.bReturnPhysicalMaterial = true;

	const UDualNodeAudioSettings* Settings = UDualNodeAudioSettings::Get();
	ECollisionChannel Channel = Settings ? Settings->PhysicsTraceChannel.GetValue() : ECC_Visibility;

	if (GetWorld()->LineTraceSingleByChannel(Hit, Start, End, Channel, Params))
	{
		if (UGameInstance* GI = GetWorld()->GetGameInstance())
		{
			if (UDualNodeAudioSubsystem* Sys = GI->GetSubsystem<UDualNodeAudioSubsystem>())
			{
				FDualNodeSoundDefinition Def;
				if (Sys->GetSoundDefinition(TagToUse, Def))
				{
					// Resolve Physics Asset (SoftPtr)
					TSoftObjectPtr<USoundBase> FinalSound = Sys->ResolveSoundFromPhysics(Def, Hit.PhysMaterial.Get());
					
					// Play specific resolved sound
					Sys->PlaySoundAtLocation(this, FinalSound, Hit.Location, FRotator::ZeroRotator, VolumeMult, 1.0f, &Def);
				}
			}
		}
	}
}