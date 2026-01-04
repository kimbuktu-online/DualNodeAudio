#include "DualNodeInventoryWidget.h"
#include "DualNodeInventoryComponent.h"
#include "DualNodeInventoryViewModel.h"

void UDualNodeInventoryWidget::InitializeInventory(UDualNodeInventoryComponent* InventoryComponent)
{
	if (!InventoryComponent) return;

	// ViewModel erstellen
	MainViewModel = NewObject<UDualNodeInventoryViewModel>(this);
	
	// Initiale Daten laden
	MainViewModel->UpdateFromInventory(InventoryComponent);

	// Bindung an das Delegate der Komponente
	InventoryComponent->OnInventoryUpdated.AddDynamic(MainViewModel.Get(), &UDualNodeInventoryViewModel::UpdateFromInventory);

	// Den TileView initial füllen
	if (InventoryTileView)
	{
		InventoryTileView->SetListItems(MainViewModel->SlotViewModels);
	}
}

// --- HIER WAR DER FEHLER: IMPLEMENTIERUNG MUSS EXISTIEREN ---
void UDualNodeInventoryWidget::HandleSlotViewModelsChanged()
{
	if (InventoryTileView && MainViewModel)
	{
		// Aktualisiert die Anzeige der TileView basierend auf den neuen Daten im ViewModel
		InventoryTileView->SetListItems(MainViewModel->SlotViewModels);
		InventoryTileView->RequestRefresh();
	}
}