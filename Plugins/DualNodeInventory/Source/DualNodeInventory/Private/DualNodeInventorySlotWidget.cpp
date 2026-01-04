#include "DualNodeInventorySlotWidget.h"
#include "DualNodeInventoryComponent.h"
#include "DualNodeInventoryWidget.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "Blueprint/DragDropOperation.h"

void UDualNodeInventorySlotWidget::NativeOnListItemObjectSet(UObject* ListItemObject)
{
	if (UDualNodeInventorySlotViewModel* NewVM = Cast<UDualNodeInventorySlotViewModel>(ListItemObject))
	{
		SlotViewModel = NewVM;
		OnSlotUpdated();
	}
}

FReply UDualNodeInventorySlotWidget::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	if (InMouseEvent.GetEffectingButton() == EKeys::RightMouseButton)
	{
		OnRightClicked();
		return FReply::Handled();
	}

	if (InMouseEvent.GetEffectingButton() == EKeys::LeftMouseButton)
	{
		// Shift + Klick -> Quick Move
		if (InMouseEvent.IsShiftDown())
		{
			// Hier Logik für QuickMove (z.B. in Truhe verschieben) einbauen
			return FReply::Handled();
		}

		// Normaler Klick -> An Haupt-Widget delegieren für Held Item Logic
		if (UDualNodeInventoryWidget* ParentWidget = Cast<UDualNodeInventoryWidget>(GetTypedOuter<UDualNodeInventoryWidget>()))
		{
			ParentWidget->HandleSlotClick(SlotViewModel);
			return FReply::Handled();
		}
	}

	return Super::NativeOnMouseButtonDown(InGeometry, InMouseEvent);
}

void UDualNodeInventorySlotWidget::NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent, UDragDropOperation*& OutOperation)
{
	if (SlotViewModel && SlotViewModel->ItemIcon != nullptr)
	{
		UDragDropOperation* DragOp = NewObject<UDragDropOperation>();
		DragOp->Payload = SlotViewModel;
		DragOp->DefaultDragVisual = this;
		OutOperation = DragOp;
	}
}

bool UDualNodeInventorySlotWidget::NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
	if (!InOperation || !SlotViewModel) return false;
	if (UDualNodeInventorySlotViewModel* FromVM = Cast<UDualNodeInventorySlotViewModel>(InOperation->Payload))
	{
		if (FromVM == SlotViewModel) return true;
		if (APawn* PlayerPawn = GetOwningPlayerPawn())
		{
			if (UDualNodeInventoryComponent* Inv = PlayerPawn->FindComponentByClass<UDualNodeInventoryComponent>())
			{
				Inv->Server_SwapSlots(FromVM->SlotIndex, SlotViewModel->SlotIndex);
				return true;
			}
		}
	}
	return false;
}