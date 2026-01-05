#include "DualNodeEquipmentComponent.h"
#include "DualNodeItemDefinition.h"
#include "DualNodeItemFragment_Equipment.h"
#include "GameFramework/Character.h"
#include "Components/SkeletalMeshComponent.h"

UDualNodeEquipmentComponent::UDualNodeEquipmentComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UDualNodeEquipmentComponent::EquipItem(const UDualNodeItemDefinition* ItemDef)
{
	if (!ItemDef) return;

	const UDualNodeItemFragment* Frag = ItemDef->FindFragmentByClass(UDualNodeItemFragment_Equipment::StaticClass());
	const UDualNodeItemFragment_Equipment* EquipFrag = Cast<UDualNodeItemFragment_Equipment>(Frag);
	
	if (!EquipFrag || !EquipFrag->SlotTag.IsValid()) return;

	// 1. Alten Slot räumen
	UnequipSlot(EquipFrag->SlotTag);

	USkeletalMeshComponent* ParentMesh = GetParentMesh();
	if (!ParentMesh) return;

	// 2. FALL A: Skinned Mesh (Rüstung/Kleidung)
	if (EquipFrag->EquipmentMesh)
	{
		USkeletalMeshComponent* NewComp = NewObject<USkeletalMeshComponent>(GetOwner());
		NewComp->SetSkeletalMesh(EquipFrag->EquipmentMesh);
		NewComp->SetupAttachment(ParentMesh);
		
		// WICHTIG: Synchronisation mit dem Haupt-Mesh
		NewComp->SetLeaderPoseComponent(ParentMesh);
		
		NewComp->RegisterComponent();
		EquippedSkeletalMeshes.Add(EquipFrag->SlotTag, NewComp);
	}
	
	// 3. FALL B: Statisches Mesh (Waffen/Werkzeuge am Socket)
	else if (EquipFrag->StaticMesh)
	{
		// Hier könnten wir einen Actor spawnen oder eine StaticMeshComponent nutzen
		UStaticMeshComponent* WeaponComp = NewObject<UStaticMeshComponent>(GetOwner());
		WeaponComp->SetStaticMesh(EquipFrag->StaticMesh);
		WeaponComp->SetupAttachment(ParentMesh, EquipFrag->AttachmentSocket);
		WeaponComp->RegisterComponent();
		
		// In einem echten System würden wir dies in der TMap tracken
	}
}

void UDualNodeEquipmentComponent::UnequipSlot(FGameplayTag SlotTag)
{
	if (USkeletalMeshComponent** FoundComp = EquippedSkeletalMeshes.Find(SlotTag))
	{
		(*FoundComp)->DestroyComponent();
		EquippedSkeletalMeshes.Remove(SlotTag);
	}
}

USkeletalMeshComponent* UDualNodeEquipmentComponent::GetParentMesh() const
{
	if (ACharacter* Char = Cast<ACharacter>(GetOwner()))
	{
		return Char->GetMesh();
	}
	return GetOwner()->FindComponentByClass<USkeletalMeshComponent>();
}