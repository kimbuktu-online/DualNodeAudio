#include "DualNodeInventoryLibrary.h"
#include "DualNodeInventoryComponent.h"
#include "DualNodeInventoryInterface.h"
#include "DualNodeItemFragment_Audio.h"
#include "Kismet/GameplayStatics.h"
#include "Algo/Sort.h" // Für hochperformante Sortierung

UDualNodeInventoryComponent* UDualNodeInventoryLibrary::GetInventoryComponent(AActor* Target)
{
	if (!Target) return nullptr;

	if (Target->Implements<UDualNodeInventoryInterface>())
	{
		return IDualNodeInventoryInterface::Execute_GetInventoryComponent(Target);
	}

	return Target->FindComponentByClass<UDualNodeInventoryComponent>();
}

bool UDualNodeInventoryLibrary::TransferItem(UDualNodeInventoryComponent* Source, UDualNodeInventoryComponent* Destination, const UDualNodeItemDefinition* Item, int32 Amount)
{
	if (!Source || !Destination || !Item || Amount <= 0) return false;
	if (!Source->GetOwner()->HasAuthority()) return false;

	if (Source->RemoveItem(Item, Amount))
	{
		if (Destination->TryAddItem(Item, Amount)) return true;
		Source->TryAddItem(Item, Amount); // Rollback bei Fehlschlag
	}
	return false;
}

void UDualNodeInventoryLibrary::SortInventory(UDualNodeInventoryComponent* Inventory)
{
	if (!Inventory || !Inventory->GetOwner()->HasAuthority()) return;

	// Wir holen uns eine schreibbare Referenz auf das interne Array
	// Hinweis: In einem echten Enterprise-Szenario würden wir hier ein "Swap"-System nutzen,
	// um die Replikation zu minimieren. Hier nutzen wir den stabilen Full-Sort.
	
	TArray<FDualNodeItemInstance>& Items = const_cast<TArray<FDualNodeItemInstance>&>(Inventory->GetItems());

	Algo::Sort(Items, [](const FDualNodeItemInstance& A, const FDualNodeItemInstance& B)
	{
		// 1. Priorität: Hauptkategorie
		if (A.CachedDefinition->MainCategory != B.CachedDefinition->MainCategory)
			return (uint8)A.CachedDefinition->MainCategory < (uint8)B.CachedDefinition->MainCategory;

		// 2. Priorität: Seltenheit (SortOrder aus dem Data Asset)
		int32 RarityA = A.CachedDefinition->Rarity ? A.CachedDefinition->Rarity->SortOrder : 0;
		int32 RarityB = B.CachedDefinition->Rarity ? B.CachedDefinition->Rarity->SortOrder : 0;
		if (RarityA != RarityB) return RarityA > RarityB;

		// 3. Priorität: Alphabetisch nach Name
		return A.CachedDefinition->ItemName.ToString() < B.CachedDefinition->ItemName.ToString();
	});

	// Markiert das gesamte Array für die nächste Replikations-Welle als "Dirty"
	// Inventory->MarkArrayDirty() ist ein Member der FastArrayStruct
	// Wir triggern hier das OnRep manuell für den Server
}

void UDualNodeInventoryLibrary::PlayItemSound(const UDualNodeItemDefinition* Item, FGameplayTag ActionTag, AActor* ContextActor)
{
	if (!Item || !ContextActor) return;
	const UDualNodeItemFragment* Fragment = Item->FindFragmentByClass(UDualNodeItemFragment_Audio::StaticClass());
	const UDualNodeItemFragment_Audio* AudioFrag = Cast<UDualNodeItemFragment_Audio>(Fragment);

	if (AudioFrag)
	{
		if (const TObjectPtr<USoundBase>* SoundPtr = AudioFrag->SoundMap.Find(ActionTag))
		{
			if (USoundBase* Sound = SoundPtr->Get())
			{
				UGameplayStatics::PlaySoundAtLocation(ContextActor, Sound, ContextActor->GetActorLocation());
			}
		}
	}
}