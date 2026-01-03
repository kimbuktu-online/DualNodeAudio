#pragma once

#include "CoreMinimal.h"
#include "DualNodeItemFragment.h"
#include "DualNodeItemFragment_UseAction.generated.h"

/** * Basisklasse für alle Aktionen, die beim "Benutzen" eines Items getriggert werden.
 * Designer können hiervon BP-Unterklassen erstellen.
 */
UCLASS(Abstract, Blueprintable, EditInlineNew, DefaultToInstanced)
class DUALNODEINVENTORY_API UDualNodeItemAction : public UObject
{
	GENERATED_BODY()

public:
	/** Führt die Logik aus (z.B. Heilung anwenden) */
	UFUNCTION(BlueprintNativeEvent, Category = "Item Action")
	void ExecuteAction(AActor* User, class UDualNodeInventoryComponent* SourceInventory);
	virtual void ExecuteAction_Implementation(AActor* User, UDualNodeInventoryComponent* SourceInventory) {}
};

UCLASS()
class DUALNODEINVENTORY_API UDualNodeItemFragment_UseAction : public UDualNodeItemFragment
{
	GENERATED_BODY()

public:
	/** Liste der Aktionen, die beim Benutzen ausgeführt werden */
	UPROPERTY(EditDefaultsOnly, Instanced, Category = "Actions")
	TArray<TObjectPtr<UDualNodeItemAction>> Actions;

	/** Soll das Item beim Benutzen verbraucht werden? (z.B. Trank: Ja, Werkzeug: Nein) */
	UPROPERTY(EditDefaultsOnly, Category = "Actions")
	bool bConsumeOnUse = true;
};