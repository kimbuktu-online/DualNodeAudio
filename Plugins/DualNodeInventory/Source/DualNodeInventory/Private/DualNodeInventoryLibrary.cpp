#include "DualNodeInventoryLibrary.h"
#include "DualNodeInventoryComponent.h"
#include "DualNodeInventoryInterface.h"
#include "DualNodeItemFragment_Audio.h"
#include "Kismet/GameplayStatics.h"
#include "Algo/Sort.h"

UDualNodeInventoryComponent* UDualNodeInventoryLibrary::GetInventoryComponent(AActor* Target)
{
	if (!Target) return nullptr;
	if (Target->Implements<UDualNodeInventoryInterface>()) return IDualNodeInventoryInterface::Execute_GetInventoryComponent(Target);
	return Target->FindComponentByClass<UDualNodeInventoryComponent>();
}

bool UDualNodeInventoryLibrary::TransferItem(UDualNodeInventoryComponent* Source, UDualNodeInventoryComponent* Destination, const UDualNodeItemDefinition* Item, int32 Amount)
{
	if (!Source || !Destination || !Item || Amount <= 0) return false;
	if (!Source->GetOwner()->HasAuthority()) return false;

	if (Source->RemoveItem(Item, Amount))
	{
		if (Destination->TryAddItem(Item, Amount)) return true;
		Source->TryAddItem(Item, Amount); // Rollback
	}
	return false;
}

void UDualNodeInventoryLibrary::SortInventory(UDualNodeInventoryComponent* Inventory)
{
	if (!Inventory || !Inventory->GetOwner()->HasAuthority()) return;

	// In der Enterprise Edition nutzen wir Algo::Sort für maximale C++ Performance
	TArray<FDualNodeItemInstance>& Items = const_cast<TArray<FDualNodeItemInstance>&>(Inventory->GetItems());

	Algo::Sort(Items, [](const FDualNodeItemInstance& A, const FDualNodeItemInstance& B)
	{
		if (!A.CachedDefinition || !B.CachedDefinition) return false;

		// 1. Nach Kategorie
		if (A.CachedDefinition->MainCategory != B.CachedDefinition->MainCategory)
			return (uint8)A.CachedDefinition->MainCategory < (uint8)B.CachedDefinition->MainCategory;

		// 2. Nach Seltenheit
		int32 RarityA = A.CachedDefinition->Rarity ? A.CachedDefinition->Rarity->SortOrder : 0;
		int32 RarityB = B.CachedDefinition->Rarity ? B.CachedDefinition->Rarity->SortOrder : 0;
		if (RarityA != RarityB) return RarityA > RarityB;

		// 3. Alphabetisch
		return A.CachedDefinition->ItemName.ToString() < B.CachedDefinition->ItemName.ToString();
	});

	// Markiert das gesamte Array als Dirty für die Replikation
	// Hier rufen wir das interne OnRep manuell auf
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
			if (USoundBase* Sound = SoundPtr->Get()) UGameplayStatics::PlaySoundAtLocation(ContextActor, Sound, ContextActor->GetActorLocation());
		}
	}
}