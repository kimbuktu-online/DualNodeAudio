#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "DualNodeInventoryTypes.h"
#include "DualNodeInventoryValidator.generated.h"

class UDualNodeInventoryComponent;
class UDualNodeItemDefinition;

UCLASS(DefaultToInstanced, EditInlineNew, Abstract, Blueprintable)
class DUALNODEINVENTORY_API UDualNodeInventoryValidator : public UObject
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintNativeEvent, Category = "Inventory|Validation")
	bool CanAddItem(const UDualNodeInventoryComponent* Inventory, const UDualNodeItemDefinition* Item, int32 Amount, FText& OutFailureMessage) const;
	virtual bool CanAddItem_Implementation(const UDualNodeInventoryComponent* Inventory, const UDualNodeItemDefinition* Item, int32 Amount, FText& OutFailureMessage) const { return true; }
};

/** Validator: Beschränkt Container auf bestimmte Kategorien */
UCLASS(meta=(DisplayName="Rule: Category Restriction"))
class DUALNODEINVENTORY_API UDualNodeValidator_Category : public UDualNodeInventoryValidator
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, Category = "Config")
	TArray<EDualNodeMainCategory> AllowedCategories;

	virtual bool CanAddItem_Implementation(const UDualNodeInventoryComponent* Inventory, const UDualNodeItemDefinition* Item, int32 Amount, FText& OutFailureMessage) const override;
};

/** Validator: Prüft das Gewichtslimit des Containers */
UCLASS(meta=(DisplayName="Rule: Weight Limit"))
class DUALNODEINVENTORY_API UDualNodeValidator_Weight : public UDualNodeInventoryValidator
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, Category = "Config")
	float MaxWeight = 100.0f;

	virtual bool CanAddItem_Implementation(const UDualNodeInventoryComponent* Inventory, const UDualNodeItemDefinition* Item, int32 Amount, FText& OutFailureMessage) const override;
};