#include "DualNodeItemTooltipWidget.h"
#include "DualNodeInventorySlotViewModel.h"

void UDualNodeItemTooltipWidget::SetTooltipData(UDualNodeInventorySlotViewModel* InSlotVM)
{
	if (InSlotVM)
	{
		SlotVM = InSlotVM;
		OnTooltipDataUpdated(); // Trigger für BP (z.B. Animationen oder Stats-Icons)
	}
}