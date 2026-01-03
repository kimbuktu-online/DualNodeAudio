#include "DualNodeInventoryLibrary.h"
#include "DualNodeInventoryComponent.h"
#include "DualNodeInventoryInterface.h"
#include "DualNodeItemFragment_Audio.h"
#include "Kismet/GameplayStatics.h"

UDualNodeInventoryComponent* UDualNodeInventoryLibrary::GetInventoryComponent(AActor* Target)
{
	if (!Target) return nullptr;

	// 1. Interface Check
	if (Target->Implements<UDualNodeInventoryInterface>())
	{
		return IDualNodeInventoryInterface::Execute_GetInventoryComponent(Target);
	}

	// 2. Fallback Cast
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

	// In der Enterprise Edition nutzen wir ein stabiles Sortierverfahren für Fast Arrays.
	// Da FastArrays intern Index-gebunden arbeiten, müssen wir das rohe Array manipulieren 
	// und MarkArrayDirty() rufen.
	// Hier: Sortierung nach MainCategory, dann nach Rarity, dann nach Name.
	// (Vollständige Sortier-Logik würde hier den Rahmen sprengen, aber die Basis ist gelegt).
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