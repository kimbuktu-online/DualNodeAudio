#include "DualNodeInventoryLibrary.h"
#include "DualNodeInventoryComponent.h"
#include "DualNodeInventoryInterface.h"
#include "DualNodeItemFragment_Audio.h"
#include "DualNodeItemFragment_UseAction.h"
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
		Source->TryAddItem(Item, Amount);
	}
	return false;
}

void UDualNodeInventoryLibrary::SortInventory(UDualNodeInventoryComponent* Inventory)
{
	if (!Inventory || !Inventory->GetOwner()->HasAuthority()) return;

	TArray<FDualNodeItemInstance>& Items = const_cast<TArray<FDualNodeItemInstance>&>(Inventory->GetItems());

	Algo::StableSort(Items, [](const FDualNodeItemInstance& A, const FDualNodeItemInstance& B)
	{
		if (!A.CachedDefinition || !B.CachedDefinition) return false;
		if (A.CachedDefinition->MainCategory != B.CachedDefinition->MainCategory)
			return (uint8)A.CachedDefinition->MainCategory < (uint8)B.CachedDefinition->MainCategory;
		return A.CachedDefinition->ItemName.ToString() < B.CachedDefinition->ItemName.ToString();
	});

	Inventory->OnRep_Inventory(); 
}

bool UDualNodeInventoryLibrary::UseItem(AActor* User, const UDualNodeItemDefinition* ItemDef)
{
	if (!User || !ItemDef) return false;
	UDualNodeInventoryComponent* Inv = GetInventoryComponent(User);
	if (!Inv) return false;

	const UDualNodeItemFragment* Fragment = ItemDef->FindFragmentByClass(UDualNodeItemFragment_UseAction::StaticClass());
	if (const UDualNodeItemFragment_UseAction* UseFrag = Cast<UDualNodeItemFragment_UseAction>(Fragment))
	{
		for (const TObjectPtr<UDualNodeItemAction>& ActionPtr : UseFrag->Actions)
		{
			if (ActionPtr) ActionPtr->ExecuteAction(User, Inv);
		}

		if (UseFrag->bConsumeOnUse) Inv->RemoveItem(ItemDef, 1);
		return true;
	}
	return false;
}

void UDualNodeInventoryLibrary::PlayItemSound(const UDualNodeItemDefinition* Item, FGameplayTag ActionTag, AActor* ContextActor)
{
	if (!Item || !ContextActor) return;
	const UDualNodeItemFragment* Fragment = Item->FindFragmentByClass(UDualNodeItemFragment_Audio::StaticClass());
	if (const UDualNodeItemFragment_Audio* AudioFrag = Cast<UDualNodeItemFragment_Audio>(Fragment))
	{
		if (const TObjectPtr<USoundBase>* SoundPtr = AudioFrag->SoundMap.Find(ActionTag))
		{
			if (USoundBase* Sound = SoundPtr->Get()) UGameplayStatics::PlaySoundAtLocation(ContextActor, Sound, ContextActor->GetActorLocation());
		}
	}
}