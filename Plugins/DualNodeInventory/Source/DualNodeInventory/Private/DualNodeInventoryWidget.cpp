#include "DualNodeInventoryWidget.h"
#include "DualNodeInventoryComponent.h"
#include "DualNodeInventoryViewModel.h"

void UDualNodeInventoryWidget::InitializeInventory(UDualNodeInventoryComponent* InventoryComponent)
{
	if (!InventoryComponent) return;

	CachedComponent = InventoryComponent;

	// 1. ViewModel erstellen
	MainViewModel = NewObject<UDualNodeInventoryViewModel>(this);
	
	// 2. Initiale Daten laden
	MainViewModel->UpdateFromInventory(InventoryComponent);

	// 3. Bindung: C++ Component -> ViewModel (Echtzeit-Datenaktualisierung)
	InventoryComponent->OnInventoryUpdated.AddDynamic(MainViewModel.Get(), &UDualNodeInventoryViewModel::UpdateFromInventory);

	// 4. FIX FÜR ECHTZEIT-UI-UPDATES (Klassen-Parameter hinzugefügt)
	// Wir holen uns die Feld-ID für "SlotViewModels" unter Angabe der ViewModel-Klasse
	UE::FieldNotification::FFieldId FieldId = MainViewModel->GetFieldNotificationDescriptor().GetField(UDualNodeInventoryViewModel::StaticClass(), FName("SlotViewModels"));

	// Wir binden den internen Callback an dieses spezifische Feld
	if (FieldId.IsValid())
	{
		MainViewModel->AddFieldValueChangedDelegate(FieldId, FFieldValueChangedDelegate::CreateUObject(this, &UDualNodeInventoryWidget::HandleSlotViewModelsChanged_Internal));
	}

	// 5. TileView initial füllen
	if (InventoryTileView)
	{
		InventoryTileView->SetListItems(MainViewModel->SlotViewModels);
	}
}

void UDualNodeInventoryWidget::HandleSlotViewModelsChanged_Internal(UObject* Source, UE::FieldNotification::FFieldId FieldId)
{
	HandleSlotViewModelsChanged();
}

void UDualNodeInventoryWidget::HandleSlotViewModelsChanged()
{
	if (InventoryTileView && MainViewModel)
	{
		// Die TileView wird mit den neuen SlotViewModels synchronisiert und neu gezeichnet
		InventoryTileView->SetListItems(MainViewModel->SlotViewModels);
		InventoryTileView->RequestRefresh();
	}
}