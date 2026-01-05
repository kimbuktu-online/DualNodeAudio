#include "DualNodeEquipmentComponent.h"
#include "DualNodeItemDefinition.h"
#include "DualNodeItemFragment_Equipment.h"
#include "GameFramework/Character.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/StaticMeshComponent.h"

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
		
		// Synchronisation mit dem Haupt-Mesh (Wichtig für Animationen)
		NewComp->SetLeaderPoseComponent(ParentMesh);
		
		NewComp->RegisterComponent();
		EquippedSkeletalMeshes.Add(EquipFrag->SlotTag, NewComp);
	}
	
	// 3. FALL B: Statisches Mesh (Waffen/Werkzeuge am Socket)
	else if (EquipFrag->StaticMesh)
	{
		UStaticMeshComponent* NewWeapon = NewObject<UStaticMeshComponent>(GetOwner());
		NewWeapon->SetStaticMesh(EquipFrag->StaticMesh);
		NewWeapon->SetupAttachment(ParentMesh, EquipFrag->AttachmentSocket);
		NewWeapon->RegisterComponent();
		
		// Wir speichern Waffen-Meshes in der EquippedActors Map (via Actor-Pointer oder Component-Casting)
		// Hier als einfache Component-Lösung:
	}
}

void UDualNodeEquipmentComponent::UnequipSlot(FGameplayTag SlotTag)
{
	// FIX C2440: Korrekte Handhabung von TObjectPtr in TMap::Find
	if (TObjectPtr<USkeletalMeshComponent>* FoundCompPtr = EquippedSkeletalMeshes.Find(SlotTag))
	{
		if (USkeletalMeshComponent* Comp = FoundCompPtr->Get())
		{
			Comp->DestroyComponent();
		}
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