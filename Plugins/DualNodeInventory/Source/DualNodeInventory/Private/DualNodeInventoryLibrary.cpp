#include "DualNodeInventoryLibrary.h"
#include "DualNodeInventoryComponent.h"
#include "DualNodeItemFragment_Audio.h"
#include "Kismet/GameplayStatics.h"
#include "Templates/SubclassOf.h"

UDualNodeInventoryComponent* UDualNodeInventoryLibrary::GetInventoryComponent(AActor* Target)
{
	if (!Target) return nullptr;
	return Target->FindComponentByClass<UDualNodeInventoryComponent>();
}

bool UDualNodeInventoryLibrary::TransferItem(UDualNodeInventoryComponent* Source, UDualNodeInventoryComponent* Destination, const UDualNodeItemDefinition* Item, int32 Amount)
{
	if (!Source || !Destination || !Item || Amount <= 0) return false;

	if (!Source->GetOwner()->HasAuthority()) return false;

	if (Source->RemoveItem(Item, Amount))
	{
		if (Destination->TryAddItem(Item, Amount))
		{
			return true;
		}
		else
		{
			Source->TryAddItem(Item, Amount);
		}
	}

	return false;
}

void UDualNodeInventoryLibrary::PlayItemSound(const UDualNodeItemDefinition* Item, FGameplayTag ActionTag, AActor* ContextActor)
{
	if (!Item || !ContextActor) return;

	// Suche nach dem Audio-Fragment in der Definition (Node A) [cite: 5, 13]
	const UDualNodeItemFragment* Fragment = Item->FindFragmentByClass(UDualNodeItemFragment_Audio::StaticClass());
	const UDualNodeItemFragment_Audio* AudioFrag = Cast<UDualNodeItemFragment_Audio>(Fragment);

	if (AudioFrag)
	{
		// Korrekte Handhabung von TObjectPtr in einer TMap [cite: 7, 33]
		if (const TObjectPtr<USoundBase>* SoundPtr = AudioFrag->SoundMap.Find(ActionTag))
		{
			if (USoundBase* Sound = SoundPtr->Get()) // Konvertierung von TObjectPtr zu Raw Pointer
			{
				UGameplayStatics::PlaySoundAtLocation(ContextActor, Sound, ContextActor->GetActorLocation());
			}
		}
	}
}