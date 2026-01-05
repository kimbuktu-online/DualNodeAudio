#include "DualNodeInventoryWidget.h"
#include "DualNodeInventoryComponent.h"
#include "DualNodeInventoryViewModel.h"
#include "DualNodeContextMenu.h"
#include "Blueprint/WidgetLayoutLibrary.h"

void UDualNodeInventoryWidget::InitializeInventory(UDualNodeInventoryComponent* InventoryComponent)
{
	if (!InventoryComponent) return;
	CachedComponent = InventoryComponent;

	MainViewModel = NewObject<UDualNodeInventoryViewModel>(this);
	MainViewModel->UpdateFromInventory(InventoryComponent);

	InventoryComponent->OnInventoryUpdated.AddDynamic(MainViewModel.Get(), &UDualNodeInventoryViewModel::UpdateFromInventory);

	UE::FieldNotification::FFieldId FieldId = MainViewModel->GetFieldNotificationDescriptor().GetField(UDualNodeInventoryViewModel::StaticClass(), FName("SlotViewModels"));
	if (FieldId.IsValid())
	{
		MainViewModel->AddFieldValueChangedDelegate(FieldId, FFieldValueChangedDelegate::CreateUObject(this, &UDualNodeInventoryWidget::HandleSlotViewModelsChanged_Internal));
	}

	if (InventoryTileView) InventoryTileView->SetListItems(MainViewModel->SlotViewModels);
}

void UDualNodeInventoryWidget::OpenContextMenu(UDualNodeInventorySlotViewModel* SlotVM, FVector2D ScreenPosition)
{
	if (!ContextMenuClass || !SlotVM) return;

	if (ActiveContextMenu) ActiveContextMenu->CloseMenu();

	ActiveContextMenu = CreateWidget<UDualNodeContextMenu>(this, ContextMenuClass);
	if (ActiveContextMenu)
	{
		ActiveContextMenu->InitializeMenu(SlotVM);
		ActiveContextMenu->AddToViewport(1000); 

		// FIX: bRemoveDPIScale = true konvertiert Screen-Pixel korrekt in Viewport-Einheiten
		ActiveContextMenu->SetPositionInViewport(ScreenPosition, true);
	}
}

void UDualNodeInventoryWidget::HandleSlotClick(UDualNodeInventorySlotViewModel* ClickedSlot)
{
	if (ActiveContextMenu) { ActiveContextMenu->CloseMenu(); ActiveContextMenu = nullptr; }
	if (!ClickedSlot || !CachedComponent) return;

	if (!HeldItemVM)
	{
		if (ClickedSlot->StackCount > 0)
		{
			HeldItemVM = ClickedSlot;
			HeldItemVM->SetIsSourceOfTransfer(true);
			TransferAmount = 1;
			if (HeldItemVisualClass && !HeldItemVisualInstance)
			{
				HeldItemVisualInstance = CreateWidget<UUserWidget>(this, HeldItemVisualClass);
				HeldItemVisualInstance->AddToViewport(999);
			}
			OnHeldAmountChanged(TransferAmount);
		}
	}
	else
	{
		if (HeldItemVM == ClickedSlot)
		{
			HeldItemVM->SetIsSourceOfTransfer(false);
			HeldItemVM = nullptr;
			if (HeldItemVisualInstance) HeldItemVisualInstance->RemoveFromParent();
			HeldItemVisualInstance = nullptr;
		}
		else
		{
			CachedComponent->Server_TransferQuantity(HeldItemVM->SlotIndex, ClickedSlot->SlotIndex, TransferAmount);
			HeldItemVM->SetIsSourceOfTransfer(false);
			HeldItemVM = nullptr;
			if (HeldItemVisualInstance) HeldItemVisualInstance->RemoveFromParent();
			HeldItemVisualInstance = nullptr;
		}
	}
}

FReply UDualNodeInventoryWidget::NativeOnMouseWheel(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	if (HeldItemVM)
	{
		float ScrollDelta = InMouseEvent.GetWheelDelta();
		int32 NewAmount = TransferAmount + (ScrollDelta > 0 ? 1 : -1);
		TransferAmount = FMath::Clamp(NewAmount, 1, HeldItemVM->StackCount);
		OnHeldAmountChanged(TransferAmount);
		return FReply::Handled();
	}
	return Super::NativeOnMouseWheel(InGeometry, InMouseEvent);
}

void UDualNodeInventoryWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
	if (HeldItemVisualInstance)
	{
		FVector2D MousePos = UWidgetLayoutLibrary::GetMousePositionOnViewport(GetWorld());
		HeldItemVisualInstance->SetPositionInViewport(MousePos + FVector2D(20, 20));
	}
}

void UDualNodeInventoryWidget::HandleSlotViewModelsChanged_Internal(UObject* Source, UE::FieldNotification::FFieldId FieldId) { HandleSlotViewModelsChanged(); }

void UDualNodeInventoryWidget::HandleSlotViewModelsChanged()
{
	if (InventoryTileView && MainViewModel)
	{
		InventoryTileView->SetListItems(MainViewModel->SlotViewModels);
		InventoryTileView->RequestRefresh();
	}
}

void UDualNodeInventoryWidget::ShowTooltip(UDualNodeInventorySlotViewModel* SlotVM, const FGeometry& SlotGeometry)
{
	if (!TooltipClass || !SlotVM) return;

	if (!ActiveTooltip)
	{
		ActiveTooltip = CreateWidget<UDualNodeItemTooltipWidget>(this, TooltipClass);
		ActiveTooltip->AddToViewport(1100); // Über dem Inventar
	}

	ActiveTooltip->SetTooltipData(SlotVM);
	ActiveTooltip->SetVisibility(ESlateVisibility::HitTestInvisible);

	// Positionierung: Rechts neben dem Slot
	FVector2D TooltipPos = SlotGeometry.GetAbsolutePosition() + FVector2D(SlotGeometry.GetAbsoluteSize().X + 10.0f, 0.0f);
	ActiveTooltip->SetPositionInViewport(TooltipPos, true);
}

void UDualNodeInventoryWidget::HideTooltip()
{
	if (ActiveTooltip)
	{
		ActiveTooltip->SetVisibility(ESlateVisibility::Collapsed);
	}
}