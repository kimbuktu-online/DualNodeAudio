#include "DualNodeWorldItem.h"
#include "DualNodeInventoryComponent.h"
#include "DualNodeItemDefinition.h"
#include "DualNodeItemFragment_Mesh.h"
#include "DualNodeInventoryLibrary.h"
#include "Net/UnrealNetwork.h"
#include "Engine/AssetManager.h"

ADualNodeWorldItem::ADualNodeWorldItem()
{
	bReplicates = true;
	AActor::SetReplicateMovement(true);

	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	SetRootComponent(Root);

	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	MeshComponent->SetupAttachment(Root);
	MeshComponent->SetCollisionProfileName(TEXT("BlockAllDynamic"));
}

void ADualNodeWorldItem::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ADualNodeWorldItem, ItemToGive);
	DOREPLIFETIME(ADualNodeWorldItem, Amount);
}

void ADualNodeWorldItem::InitializeItem(const UDualNodeItemDefinition* InDefinition, int32 InAmount)
{
	if (!InDefinition) return;

	ItemToGive = InDefinition->GetPrimaryAssetId();
	Amount = InAmount;

	// Visuelles Setup über das Mesh-Fragment
	const UDualNodeItemFragment* Frag = InDefinition->FindFragmentByClass(UDualNodeItemFragment_Mesh::StaticClass());
	if (const UDualNodeItemFragment_Mesh* MeshFrag = Cast<UDualNodeItemFragment_Mesh>(Frag))
	{
		if (MeshFrag->StaticMesh)
		{
			MeshComponent->SetStaticMesh(MeshFrag->StaticMesh);
			MeshComponent->SetRelativeScale3D(MeshFrag->WorldScale);
		}
	}
}

bool ADualNodeWorldItem::PickUp(AActor* Interactor)
{
	if (!HasAuthority()) return false;

	if (UDualNodeInventoryComponent* Inv = UDualNodeInventoryLibrary::GetInventoryComponent(Interactor))
	{
		const UDualNodeItemDefinition* ItemDef = Cast<UDualNodeItemDefinition>(UAssetManager::Get().GetPrimaryAssetObject(ItemToGive));
		if (Inv->TryAddItem(ItemDef, Amount))
		{
			Destroy();
			return true;
		}
	}
	return false;
}