#include "DualNodeInventorySlotWidget.h"

void UDualNodeInventorySlotWidget::NativeOnListItemObjectSet(UObject* ListItemObject)
{
	// FIX: Funktionsname angepasst auf Native-Variante
	if (UDualNodeInventorySlotViewModel* NewVM = Cast<UDualNodeInventorySlotViewModel>(ListItemObject))
	{
		SlotViewModel = NewVM;
		
		// Triggert das BP-Event für visuelle Updates
		OnSlotUpdated();
	}
}