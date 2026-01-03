#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "DualNodeItemFragment.generated.h"

/** Basisklasse für alle modularen Daten-Erweiterungen (Stats, Audio, Tool-Logik)  */
UCLASS(DefaultToInstanced, EditInlineNew, Abstract)
class DUALNODEINVENTORY_API UDualNodeItemFragment : public UObject
{
	GENERATED_BODY()
};