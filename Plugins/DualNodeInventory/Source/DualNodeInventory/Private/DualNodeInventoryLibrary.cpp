#include "DualNodeInventoryLibrary.h"
#include "DualNodeInventoryComponent.h"
#include "DualNodeInventoryInterface.h"
#include "DualNodeItemFragment_Audio.h"
#include "DualNodeItemFragment_UseAction.h"
#include "DualNodeWorldItem.h" // Zwingend erforderlich für DropItem
#include "Kismet/GameplayStatics.h"
#include "Algo/Sort.h"

UDualNodeInventoryComponent* UDualNodeInventoryLibrary::GetInventoryComponent(AActor* Target)
{
	if (!Target) return nullptr;
	if (Target->Implements<UDualNodeInventoryInterface>()) return IDualNodeInventoryInterface::Execute_GetInventoryComponent(Target);
	return Target->FindComponentByClass<UDualNodeInventoryComponent>();
}

bool UDualNodeInventoryLibrary::ResolveItemDefinition(FDualNodeItemInstance& ItemInstance)
{
	return ItemInstance.ResolveDefinition();
}

bool UDualNodeInventoryLibrary::TransferItem(UDualNodeInventoryComponent* Source, UDualNodeInventoryComponent* Destination, const UDualNodeItemDefinition* Item, int32 Amount)
{
	if (!Source || !Destination || !Item || Amount <= 0 || !Source->GetOwner()->HasAuthority()) return false;
	if (Source->RemoveItem(Item, Amount))
	{
		if (Destination->TryAddItem(Item, Amount)) return true;
		Source->TryAddItem(Item, Amount); // Rollback
	}
	return false;
}

bool UDualNodeInventoryLibrary::SplitStack(UDualNodeInventoryComponent* Inventory, const UDualNodeItemDefinition* Item, int32 Amount)
{
	if (!Inventory || !Item || Amount <= 0 || !Inventory->GetOwner()->HasAuthority()) return false;
	if (Inventory->GetTotalAmountOfItem(Item) < Amount) return false;

	if (Inventory->RemoveItem(Item, Amount))
	{
		return Inventory->TryAddItem(Item, Amount);
	}
	return false;
}

bool UDualNodeInventoryLibrary::DropItem(AActor* Dropper, const UDualNodeItemDefinition* Item, int32 Amount)
{
	if (!Dropper || !Item || Amount <= 0 || !Dropper->HasAuthority()) return false;

	UDualNodeInventoryComponent* Inv = GetInventoryComponent(Dropper);
	if (Inv && Inv->RemoveItem(Item, Amount))
	{
		// Spawn-Position vor dem Dropper
		FVector SpawnLoc = Dropper->GetActorLocation() + (Dropper->GetActorForwardVector() * 100.0f);
		ADualNodeWorldItem* WorldItem = Dropper->GetWorld()->SpawnActor<ADualNodeWorldItem>(ADualNodeWorldItem::StaticClass(), SpawnLoc, FRotator::ZeroRotator);
		
		if (WorldItem)
		{
			WorldItem->InitializeItem(Item, Amount);
			return true;
		}
	}
	return false;
}

void UDualNodeInventoryLibrary::SortInventory(UDualNodeInventoryComponent* Inventory)
{
	if (!Inventory || !Inventory->GetOwner()->HasAuthority()) return;
	TArray<FDualNodeItemInstance>& Items = const_cast<TArray<FDualNodeItemInstance>&>(Inventory->GetItems());
	Algo::StableSort(Items, [](const FDualNodeItemInstance& A, const FDualNodeItemInstance& B) {
		if (!A.CachedDefinition || !B.CachedDefinition) return false;
		if (A.CachedDefinition->MainCategory != B.CachedDefinition->MainCategory) return (uint8)A.CachedDefinition->MainCategory < (uint8)B.CachedDefinition->MainCategory;
		return A.CachedDefinition->ItemName.ToString() < B.CachedDefinition->ItemName.ToString();
	});
	Inventory->OnRep_Inventory();
}

bool UDualNodeInventoryLibrary::UseItem(AActor* User, const UDualNodeItemDefinition* ItemDef)
{
	if (!User || !ItemDef) return false;
	UDualNodeInventoryComponent* Inv = GetInventoryComponent(User);
	if (!Inv) return false;
	const UDualNodeItemFragment* Frag = ItemDef->FindFragmentByClass(UDualNodeItemFragment_UseAction::StaticClass());
	if (const UDualNodeItemFragment_UseAction* UseFrag = Cast<UDualNodeItemFragment_UseAction>(Frag))
	{
		for (const TObjectPtr<UDualNodeItemAction>& Action : UseFrag->Actions) if (Action) Action->ExecuteAction(User, Inv);
		if (UseFrag->bConsumeOnUse) Inv->RemoveItem(ItemDef, 1);
		return true;
	}
	return false;
}

void UDualNodeInventoryLibrary::PlayItemSound(const UDualNodeItemDefinition* Item, FGameplayTag ActionTag, AActor* ContextActor)
{
	if (!Item || !ContextActor) return;
	const UDualNodeItemFragment* Frag = Item->FindFragmentByClass(UDualNodeItemFragment_Audio::StaticClass());
	if (const UDualNodeItemFragment_Audio* AudioFrag = Cast<UDualNodeItemFragment_Audio>(Frag))
	{
		if (const TObjectPtr<USoundBase>* SoundPtr = AudioFrag->SoundMap.Find(ActionTag))
			if (USoundBase* Sound = SoundPtr->Get()) UGameplayStatics::PlaySoundAtLocation(ContextActor, Sound, ContextActor->GetActorLocation());
	}
}