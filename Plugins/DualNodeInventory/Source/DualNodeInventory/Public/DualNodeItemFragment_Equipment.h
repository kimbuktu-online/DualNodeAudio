#pragma once

#include "CoreMinimal.h"
#include "DualNodeItemFragment.h"
#include "GameplayTagContainer.h"
#include "Animation/AnimInstance.h"
#include "DualNodeItemFragment_Equipment.generated.h"

class USkeletalMesh;
class UStaticMesh;

UCLASS(meta=(DisplayName="Fragment: Equipment Visuals"))
class DUALNODEINVENTORY_API UDualNodeItemFragment_Equipment : public UDualNodeItemFragment
{
	GENERATED_BODY()

public:
	/** Skinned Mesh für Kleidung/Rüstung (nutzt Leader-Pose) */
	UPROPERTY(EditDefaultsOnly, Category = "Visuals")
	TObjectPtr<USkeletalMesh> EquipmentMesh;

	/** Statisches Mesh für Waffen/Werkzeuge (nutzt Socket-Attachment) */
	UPROPERTY(EditDefaultsOnly, Category = "Visuals")
	TObjectPtr<UStaticMesh> StaticMesh;

	/** In welchen Slot gehört dieses Item? (z.B. Equipment.Slot.Head) */
	UPROPERTY(EditDefaultsOnly, Category = "Logic")
	FGameplayTag SlotTag;

	/** Socket-Name für Waffen-Attachments */
	UPROPERTY(EditDefaultsOnly, Category = "Logic", meta=(EditCondition="StaticMesh != nullptr"))
	FName AttachmentSocket = NAME_None;

	/** Optionales Anim-Overlay für spezifische Item-Haltungen (z.B. Zweihänder) */
	UPROPERTY(EditDefaultsOnly, Category = "Visuals")
	TSubclassOf<UAnimInstance> OverlayAnimInstance;
};