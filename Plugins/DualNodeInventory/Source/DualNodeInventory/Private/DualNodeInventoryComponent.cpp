#include "DualNodeInventoryComponent.h"
#include "Net/UnrealNetwork.h"
#include "DualNodeItemDefinition.h"
#include "DualNodeInventoryLibrary.h"

UDualNodeInventoryComponent::UDualNodeInventoryComponent()
{
	SetIsReplicatedByDefault(true);
	InventoryArray.OwnerComponent = this;
}

void UDualNodeInventoryComponent::BeginPlay()
{
	Super::BeginPlay();
	if (GetOwner()->HasAuthority() && InventoryArray.Items.Num() == 0)
	{
		for (int32 i = 0; i < MaxSlotCount; i++)
		{
			InventoryArray.Items.Add(FDualNodeItemInstance());
		}
		InventoryArray.MarkArrayDirty();
	}
}

void UDualNodeInventoryComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(UDualNodeInventoryComponent, InventoryArray);
}

bool UDualNodeInventoryComponent::CanAddItem(const UDualNodeItemDefinition* ItemDef, int32 Amount, FText& OutFailureReason) const
{
	if (!ItemDef || Amount <= 0) return false;
	for (const TObjectPtr<UDualNodeInventoryValidator>& ValidatorPtr : Validators)
	{
		if (ValidatorPtr && !ValidatorPtr->CanAddItem(this, ItemDef, Amount, OutFailureReason)) return false;
	}
	if (ItemDef->bCanStack && FindStackableSlot(ItemDef) != INDEX_NONE) return true;
	return FindFirstEmptySlot() != INDEX_NONE;
}

bool UDualNodeInventoryComponent::TryAddItem(const UDualNodeItemDefinition* ItemDef, int32 Amount)
{
	if (!GetOwner()->HasAuthority() || !ItemDef) return false;
	FText Dummy;
	if (!CanAddItem(ItemDef, Amount, Dummy)) return false;

	int32 Remaining = Amount;
	FPrimaryAssetId Id = ItemDef->GetPrimaryAssetId();

	if (ItemDef->bCanStack)
	{
		for (FDualNodeItemInstance& Slot : InventoryArray.Items)
		{
			if (Slot.ItemId == Id && Slot.StackCount < ItemDef->MaxStackSize)
			{
				int32 ToAdd = FMath::Min(Remaining, ItemDef->MaxStackSize - Slot.StackCount);
				Slot.StackCount += ToAdd;
				Slot.ResolveDefinition();
				Remaining -= ToAdd;
				InventoryArray.MarkItemDirty(Slot);
				if (Remaining <= 0) break;
			}
		}
	}

	while (Remaining > 0)
	{
		int32 EmptyIdx = FindFirstEmptySlot();
		if (EmptyIdx == INDEX_NONE) break;

		FDualNodeItemInstance& Slot = InventoryArray.Items[EmptyIdx];
		Slot.ItemId = Id;
		Slot.CachedDefinition = const_cast<UDualNodeItemDefinition*>(ItemDef);
		Slot.StackCount = FMath::Min(Remaining, ItemDef->MaxStackSize);
		Slot.InstanceGuid = FGuid::NewGuid();
		Remaining -= Slot.StackCount;
		InventoryArray.MarkItemDirty(Slot);
	}
	OnRep_Inventory();
	return true;
}

bool UDualNodeInventoryComponent::RemoveItem(const UDualNodeItemDefinition* ItemDef, int32 Amount)
{
	if (!GetOwner()->HasAuthority() || !ItemDef) return false;
	FPrimaryAssetId Id = ItemDef->GetPrimaryAssetId();
	int32 Remaining = Amount;
	for (int32 i = InventoryArray.Items.Num() - 1; i >= 0; --i)
	{
		if (InventoryArray.Items[i].ItemId == Id)
		{
			int32 ToRemove = FMath::Min(Remaining, InventoryArray.Items[i].StackCount);
			InventoryArray.Items[i].StackCount -= ToRemove;
			Remaining -= ToRemove;
			if (InventoryArray.Items[i].StackCount <= 0) InventoryArray.Items[i] = FDualNodeItemInstance();
			InventoryArray.MarkItemDirty(InventoryArray.Items[i]);
			if (Remaining <= 0) break;
		}
	}
	OnRep_Inventory();
	return true;
}

void UDualNodeInventoryComponent::Server_SwapSlots_Implementation(int32 From, int32 To)
{
	if (InventoryArray.Items.IsValidIndex(From) && InventoryArray.Items.IsValidIndex(To))
	{
		InventoryArray.Items.Swap(From, To);
		InventoryArray.MarkArrayDirty();
		OnRep_Inventory();
	}
}

void UDualNodeInventoryComponent::Server_DropFromSlot_Implementation(int32 SlotIndex, int32 Amount)
{
	if (!InventoryArray.Items.IsValidIndex(SlotIndex)) return;
	
	FDualNodeItemInstance& Slot = InventoryArray.Items[SlotIndex];
	if (!Slot.CachedDefinition || Amount <= 0 || Slot.StackCount <= 0) return;

	int32 DropAmount = FMath::Min(Amount, Slot.StackCount);
	
	// FIX: Wir nutzen SpawnItemInWorld (nur Spawn!), anstatt DropItem (Spawn + globaler Abzug)
	FVector SpawnLoc = GetOwner()->GetActorLocation() + (GetOwner()->GetActorForwardVector() * 100.0f);
	
	if (UDualNodeInventoryLibrary::SpawnItemInWorld(GetOwner(), Slot.CachedDefinition, DropAmount, SpawnLoc))
	{
		// Jetzt ziehen wir NUR von diesem spezifischen Slot ab
		Slot.StackCount -= DropAmount;
		
		if (Slot.StackCount <= 0) 
		{
			Slot = FDualNodeItemInstance();
		}

		InventoryArray.MarkItemDirty(Slot);
		OnRep_Inventory();
	}
}

void UDualNodeInventoryComponent::Server_UseItemInSlot_Implementation(int32 SlotIndex)
{
	if (!InventoryArray.Items.IsValidIndex(SlotIndex)) return;
	
	FDualNodeItemInstance& Slot = InventoryArray.Items[SlotIndex];
	if (Slot.CachedDefinition)
	{
		// Wir geben den SlotIndex mit, damit das System weiß, woher es kommt
		UDualNodeInventoryLibrary::UseItem(GetOwner(), Slot.CachedDefinition, SlotIndex);
		
		// Falls das Item verbraucht wurde (durch UseItem -> RemoveItem), 
		// müssen wir sicherstellen, dass wir synchron sind. 
		// OnRep_Inventory() sorgt für den Refresh der ViewModels.
		OnRep_Inventory();
	}
}

void UDualNodeInventoryComponent::Server_TransferQuantity_Implementation(int32 FromIndex, int32 ToIndex, int32 Quantity, UDualNodeInventoryComponent* TargetInventory)
{
	if (!InventoryArray.Items.IsValidIndex(FromIndex)) return;
	
	UDualNodeInventoryComponent* DestInv = TargetInventory ? TargetInventory : this;
	if (!DestInv->InventoryArray.Items.IsValidIndex(ToIndex)) return;

	FDualNodeItemInstance& SourceSlot = InventoryArray.Items[FromIndex];
	FDualNodeItemInstance& DestSlot = DestInv->InventoryArray.Items[ToIndex];

	if (!SourceSlot.CachedDefinition) return;

	int32 MoveAmount = FMath::Min(Quantity, SourceSlot.StackCount);

	// Logik: Entweder in leeren Slot schieben oder auf gleichen Typ stacken
	if (!DestSlot.ItemId.IsValid() || (DestSlot.ItemId == SourceSlot.ItemId && DestSlot.StackCount < SourceSlot.CachedDefinition->MaxStackSize))
	{
		int32 CanFit = DestSlot.ItemId.IsValid() ? SourceSlot.CachedDefinition->MaxStackSize - DestSlot.StackCount : MoveAmount;
		int32 ActualMove = FMath::Min(MoveAmount, CanFit);

		if (ActualMove <= 0) return;

		DestSlot.ItemId = SourceSlot.ItemId;
		DestSlot.CachedDefinition = SourceSlot.CachedDefinition;
		DestSlot.StackCount += ActualMove;
		if (!DestSlot.InstanceGuid.IsValid()) DestSlot.InstanceGuid = FGuid::NewGuid();

		SourceSlot.StackCount -= ActualMove;
		if (SourceSlot.StackCount <= 0) SourceSlot = FDualNodeItemInstance();

		InventoryArray.MarkItemDirty(SourceSlot);
		DestInv->InventoryArray.MarkItemDirty(DestSlot);
		
		OnRep_Inventory();
		if (DestInv != this) DestInv->OnRep_Inventory();
	}
}

int32 UDualNodeInventoryComponent::GetTotalAmountOfItem(const UDualNodeItemDefinition* ItemDef) const
{
	return ItemDef ? GetTotalAmountOfItemById(ItemDef->GetPrimaryAssetId()) : 0;
}

int32 UDualNodeInventoryComponent::GetTotalAmountOfItemById(FPrimaryAssetId ItemId) const
{
	int32 Total = 0;
	for (const auto& Item : InventoryArray.Items) if (Item.ItemId == ItemId) Total += Item.StackCount;
	return Total;
}

float UDualNodeInventoryComponent::GetTotalWeight() const
{
	float Weight = 0.0f;
	for (const auto& Item : InventoryArray.Items) if (Item.CachedDefinition) Weight += (Item.CachedDefinition->ItemWeight * Item.StackCount);
	return Weight;
}

FDualNodeInventorySaveData UDualNodeInventoryComponent::GetInventorySnapshot() const
{
	FDualNodeInventorySaveData Snapshot;
	for (const auto& Item : InventoryArray.Items)
	{
		FDualNodeItemSaveData Data;
		Data.ItemId = Item.ItemId; Data.StackCount = Item.StackCount; Data.InstanceGuid = Item.InstanceGuid;
		Snapshot.SavedItems.Add(Data);
	}
	return Snapshot;
}

void UDualNodeInventoryComponent::LoadInventoryFromSnapshot(const FDualNodeInventorySaveData& Snapshot)
{
	if (!GetOwner()->HasAuthority()) return;
	InventoryArray.Items.Empty();
	for (const auto& S : Snapshot.SavedItems)
	{
		FDualNodeItemInstance NewItem;
		NewItem.ItemId = S.ItemId; NewItem.StackCount = S.StackCount; NewItem.InstanceGuid = S.InstanceGuid;
		NewItem.ResolveDefinition();
		InventoryArray.Items.Add(NewItem);
	}
	InventoryArray.MarkArrayDirty();
	OnRep_Inventory();
}

void UDualNodeInventoryComponent::OnRep_Inventory()
{
	for (FDualNodeItemInstance& Slot : InventoryArray.Items) Slot.ResolveDefinition();
	OnInventoryUpdated.Broadcast(this);
}

int32 UDualNodeInventoryComponent::FindStackableSlot(const UDualNodeItemDefinition* ItemDef) const
{
	if (!ItemDef) return INDEX_NONE;
	FPrimaryAssetId Id = ItemDef->GetPrimaryAssetId();
	for (int32 i = 0; i < InventoryArray.Items.Num(); ++i)
		if (InventoryArray.Items[i].ItemId == Id && InventoryArray.Items[i].StackCount < ItemDef->MaxStackSize) return i;
	return INDEX_NONE;
}

int32 UDualNodeInventoryComponent::FindFirstEmptySlot() const
{
	for (int32 i = 0; i < InventoryArray.Items.Num(); i++) if (!InventoryArray.Items[i].ItemId.IsValid()) return i;
	return INDEX_NONE;
}