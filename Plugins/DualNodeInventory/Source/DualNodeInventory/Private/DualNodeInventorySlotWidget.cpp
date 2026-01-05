#include "DualNodeInventorySlotWidget.h"
#include "DualNodeInventoryComponent.h"
#include "DualNodeInventoryWidget.h"
#include "DualNodeInventorySlotViewModel.h"
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
	// RECHTSKLICK -> Kontextmenü
	if (InMouseEvent.GetEffectingButton() == EKeys::RightMouseButton)
	{
		if (SlotViewModel && SlotViewModel->StackCount > 0)
		{
			if (UDualNodeInventoryWidget* ParentWidget = Cast<UDualNodeInventoryWidget>(GetTypedOuter<UDualNodeInventoryWidget>()))
			{
				// Wir senden die Screen-Position (Monitor-Pixel)
				ParentWidget->OpenContextMenu(SlotViewModel, InMouseEvent.GetScreenSpacePosition());
				return FReply::Handled();
			}
		}
	}

	// LINKSKLICK -> Drag/Drop oder Transfer
	if (InMouseEvent.GetEffectingButton() == EKeys::LeftMouseButton)
	{
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

void UDualNodeInventorySlotWidget::NativeOnMouseEnter(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseEnter(InGeometry, InMouseEvent);
	if (SlotViewModel && SlotViewModel->StackCount > 0)
	{
		if (auto* Parent = Cast<UDualNodeInventoryWidget>(GetTypedOuter<UDualNodeInventoryWidget>()))
		{
			Parent->ShowTooltip(SlotViewModel, InGeometry);
		}
	}
}

void UDualNodeInventorySlotWidget::NativeOnMouseLeave(const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseLeave(InMouseEvent);
	if (auto* Parent = Cast<UDualNodeInventoryWidget>(GetTypedOuter<UDualNodeInventoryWidget>()))
	{
		Parent->HideTooltip();
	}
}