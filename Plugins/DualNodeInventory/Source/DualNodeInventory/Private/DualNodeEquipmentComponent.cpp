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

	// 1. Alten Slot räumen (löst auch Event aus)
	UnequipSlot(EquipFrag->SlotTag);

	USkeletalMeshComponent* ParentMesh = GetParentMesh();
	if (!ParentMesh) return;

	// 2. Item in der Map speichern
	EquippedItems.Add(EquipFrag->SlotTag, ItemDef);

	// 3. FALL A: Skinned Mesh (Rüstung/Kleidung)
	if (EquipFrag->EquipmentMesh != nullptr)
	{
		USkeletalMeshComponent* NewComp = NewObject<USkeletalMeshComponent>(GetOwner());
		NewComp->SetSkeletalMesh(EquipFrag->EquipmentMesh.Get());
		NewComp->SetupAttachment(ParentMesh);
		NewComp->SetLeaderPoseComponent(ParentMesh);
		NewComp->RegisterComponent();
		EquippedSkeletalMeshes.Add(EquipFrag->SlotTag, NewComp);
	}
	// 4. FALL B: Statisches Mesh (Waffen/Werkzeuge am Socket)
	else if (EquipFrag->StaticMesh != nullptr)
	{
		UStaticMeshComponent* NewWeapon = NewObject<UStaticMeshComponent>(GetOwner());
		NewWeapon->SetStaticMesh(EquipFrag->StaticMesh.Get());
		NewWeapon->SetupAttachment(ParentMesh, EquipFrag->AttachmentSocket);
		NewWeapon->RegisterComponent();
		EquippedStaticMeshes.Add(EquipFrag->SlotTag, NewWeapon);
	}

	// 5. Event auslösen
	OnEquipmentChanged.Broadcast(EquipFrag->SlotTag, ItemDef);
}

void UDualNodeEquipmentComponent::UnequipSlot(FGameplayTag SlotTag)
{
	// Item aus der Map entfernen
	TObjectPtr<const UDualNodeItemDefinition> OldItem = nullptr;
	EquippedItems.RemoveAndCopyValue(SlotTag, OldItem);

	// Skeletal Meshes entfernen
	if (TObjectPtr<USkeletalMeshComponent>* FoundCompPtr = EquippedSkeletalMeshes.Find(SlotTag))
	{
		if (USkeletalMeshComponent* Comp = FoundCompPtr->Get())
		{
			Comp->DestroyComponent();
		}
		EquippedSkeletalMeshes.Remove(SlotTag);
	}

	// Static Meshes entfernen
	if (TObjectPtr<UStaticMeshComponent>* FoundStaticCompPtr = EquippedStaticMeshes.Find(SlotTag))
	{
		if (UStaticMeshComponent* Comp = FoundStaticCompPtr->Get())
		{
			Comp->DestroyComponent();
		}
		EquippedStaticMeshes.Remove(SlotTag);
	}

	// Event nur auslösen, wenn tatsächlich ein Item entfernt wurde
	if (OldItem)
	{
		OnEquipmentChanged.Broadcast(SlotTag, nullptr);
	}
}

const UDualNodeItemDefinition* UDualNodeEquipmentComponent::GetEquippedItemForSlot(FGameplayTag SlotTag) const
{
	if (const TObjectPtr<const UDualNodeItemDefinition>* FoundItem = EquippedItems.Find(SlotTag))
	{
		return FoundItem->Get();
	}
	return nullptr;
}

USkeletalMeshComponent* UDualNodeEquipmentComponent::GetParentMesh() const
{
	if (ACharacter* Char = Cast<ACharacter>(GetOwner()))
	{
		return Char->GetMesh();
	}
	return GetOwner()->FindComponentByClass<USkeletalMeshComponent>();
}