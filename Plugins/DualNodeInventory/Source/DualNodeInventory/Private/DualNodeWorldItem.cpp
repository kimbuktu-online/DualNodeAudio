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

void ADualNodeWorldItem::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);

	// Editor-Vorschau: Lädt die Definition synchron, um das Mesh anzuzeigen
	if (ItemToGive.IsValid())
	{
		UAssetManager& AssetManager = UAssetManager::Get();
		
		if (UDualNodeItemDefinition* Def = Cast<UDualNodeItemDefinition>(AssetManager.GetPrimaryAssetObject(ItemToGive)))
		{
			UpdateVisualsFromDefinition(Def);
		}
		else
		{
			// FIX: Nutze AssetManager, um den Pfad für FPrimaryAssetId zu erhalten
			FSoftObjectPath AssetPath = AssetManager.GetPrimaryAssetPath(ItemToGive);
			UDualNodeItemDefinition* LoadedDef = Cast<UDualNodeItemDefinition>(AssetPath.TryLoad());
			
			if (LoadedDef)
			{
				UpdateVisualsFromDefinition(LoadedDef);
			}
		}
	}
}

void ADualNodeWorldItem::InitializeItem(const UDualNodeItemDefinition* InDefinition, int32 InAmount)
{
	if (!InDefinition) return;

	ItemToGive = InDefinition->GetPrimaryAssetId();
	Amount = InAmount;
	
	UpdateVisualsFromDefinition(InDefinition);
}

void ADualNodeWorldItem::UpdateVisualsFromDefinition(const UDualNodeItemDefinition* Definition)
{
	if (!Definition || !MeshComponent) return;

	const UDualNodeItemFragment* Frag = Definition->FindFragmentByClass(UDualNodeItemFragment_Mesh::StaticClass());
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
		// Asset auflösen
		const UDualNodeItemDefinition* ItemDef = Cast<UDualNodeItemDefinition>(UAssetManager::Get().GetPrimaryAssetObject(ItemToGive));
		if (!ItemDef) return false;

		if (Inv->TryAddItem(ItemDef, Amount))
		{
			// Optional: Audio Pickup-Event triggern
			UDualNodeInventoryLibrary::PlayItemSound(ItemDef, FGameplayTag::RequestGameplayTag(TEXT("Event.Inventory.Pickup")), Interactor);
			
			Destroy();
			return true;
		}
	}
	return false;
}