#pragma once

#include "CoreMinimal.h"
#include "DualNodeItemFragment.h"
#include "DualNodeInventoryTypes.h"
#include "DualNodeItemFragment_Durability.generated.h"

UCLASS(meta=(DisplayName="Fragment: Durability & Perishability"))
class DUALNODEINVENTORY_API UDualNodeItemFragment_Durability : public UDualNodeItemFragment
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, Category = "Config")
	EDualNodeDurabilityType DurabilityType = EDualNodeDurabilityType::None;

	/** Maximalwert der Haltbarkeit (z.B. 100 Schläge oder 100% Frische) */
	UPROPERTY(EditDefaultsOnly, Category = "Config", meta=(ClampMin="0.1"))
	float MaxDurability = 100.0f;

	/** * Use-Based: Abzug pro Ausführung.
	 * Time-Based: Lebensdauer in Sekunden bis zum Verfall.
	 */
	UPROPERTY(EditDefaultsOnly, Category = "Config")
	float DegradationRate = 1.0f;
};