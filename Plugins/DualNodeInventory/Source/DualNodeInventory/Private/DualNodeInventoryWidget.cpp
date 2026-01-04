#include "DualNodeInventoryWidget.h"
#include "DualNodeInventoryComponent.h"
#include "DualNodeInventoryViewModel.h"
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

void UDualNodeInventoryWidget::HandleSlotClick(UDualNodeInventorySlotViewModel* ClickedSlot)
{
	if (!ClickedSlot || !CachedComponent) return;

	// FALL 1: Wir halten noch nichts -> Item aufnehmen
	if (!HeldItemVM)
	{
		if (ClickedSlot->StackCount > 0)
		{
			HeldItemVM = ClickedSlot;
			HeldItemVM->SetIsSourceOfTransfer(true);
			TransferAmount = 1; // Startwert

			// Visuelles Widget am Cursor spawnen (falls definiert)
			if (HeldItemVisualClass && !HeldItemVisualInstance)
			{
				HeldItemVisualInstance = CreateWidget<UUserWidget>(this, HeldItemVisualClass);
				HeldItemVisualInstance->AddToViewport(999);
			}
			OnHeldAmountChanged(TransferAmount);
		}
	}
	// FALL 2: Wir halten bereits etwas -> Drop / Transfer
	else
	{
		// Auf denselben Slot klicken -> Abbrechen/Zurücklegen
		if (HeldItemVM == ClickedSlot)
		{
			HeldItemVM->SetIsSourceOfTransfer(false);
			HeldItemVM = nullptr;
			if (HeldItemVisualInstance) HeldItemVisualInstance->RemoveFromParent();
			HeldItemVisualInstance = nullptr;
		}
		// Auf anderen Slot klicken -> Transfer ausführen
		else
		{
			CachedComponent->Server_TransferQuantity(HeldItemVM->SlotIndex, ClickedSlot->SlotIndex, TransferAmount);
			
			// Reset
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
		
		// Clamp: Zwischen 1 und maximaler Menge im Quell-Slot
		TransferAmount = FMath::Clamp(NewAmount, 1, HeldItemVM->StackCount);
		
		OnHeldAmountChanged(TransferAmount);
		return FReply::Handled();
	}
	return Super::NativeOnMouseWheel(InGeometry, InMouseEvent);
}

void UDualNodeInventoryWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	// Held Item dem Cursor folgen lassen
	if (HeldItemVisualInstance)
	{
		FVector2D MousePos = UWidgetLayoutLibrary::GetMousePositionOnViewport(GetWorld());
		HeldItemVisualInstance->SetPositionInViewport(MousePos + FVector2D(20, 20)); // Kleiner Offset
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