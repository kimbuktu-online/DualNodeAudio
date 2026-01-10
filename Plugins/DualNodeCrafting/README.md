# DualNodeCrafting

## Zweck

Dieses Plugin ermöglicht es Spielern, aus vorhandenen Gegenständen neue Gegenstände herzustellen. Es soll ein flexibles, datengetriebenes System für verschiedene Arten des Craftings (z.B. an einer Werkbank, direkt im Inventar) bereitstellen.

## Entkoppelte Integration

`DualNodeCrafting` ist eng mit dem Inventar-System verbunden, sollte aber so entkoppelt wie möglich sein.

*   **Stellt bereit (Provides):**
    *   Ein `UCraftingRecipe` Asset, das die benötigten Zutaten (Ingredients) und das Ergebnis (Result) definiert.
    *   Ein `UCraftingComponent`, das an eine "Werkbank" oder den Spieler selbst gehängt werden kann.
    *   Delegates wie `OnCraftingSuccess`, `OnCraftingFailure`.

*   **Hört zu (Listens to):**
    *   Dieses Plugin ist primär ein "Anbieter" und reagiert auf direkte Aufrufe. Es hört nicht auf Broadcast-Delegates anderer Systeme.

*   **Andockpunkte für andere Plugins:**
    *   **DualNodeInventory:** Das ist die wichtigste Verbindung. Wenn ein Spieler versucht, ein Rezept herzustellen, fragt das `CraftingComponent` das `DualNodeInventorySubsystem`: "Hat der Spieler die Items X, Y und Z in ausreichender Menge?". Wenn ja, werden die Zutaten entfernt und das Ergebnis hinzugefügt.
    *   **DualNodeUI:** Stellt das Crafting-Menü dar, listet die verfügbaren Rezepte und zeigt die benötigten Zutaten an. Die UI ruft die `CraftItem`-Funktion im `CraftingComponent` auf.
    *   **DualNodeSkillTree:** Kann das Freischalten von neuen Rezepten als Belohnung für das Erlernen einer Fähigkeit auslösen (`CraftingComponent::UnlockRecipe`).
    *   **DualNodeVFX / DualNodeAudio:** Können auf `OnCraftingSuccess` hören, um visuelle Effekte oder Sounds abzuspielen.

## Roadmap

1.  **Phase 1: Datenstruktur**
    *   Definition des `UCraftingRecipe` Assets. Dieses sollte eine Liste von Zutaten (Item-Definition + Menge) und eine Liste von Ergebnissen unterstützen.
    *   Erstellung des `UCraftingComponent` mit einer Liste von freigeschalteten Rezepten.

2.  **Phase 2: Kernlogik**
    *   Implementierung der `CraftItem`-Funktion, die die Verfügbarkeit der Zutaten im `DualNodeInventory` prüft.
    *   Implementierung der Logik zum Verbrauchen der Zutaten und Hinzufügen des Ergebnisses.

3.  **Phase 3: Integration & UI**
    *   Anbindung an `DualNodeUI` für das Crafting-Menü.
    *   Integration mit `DualNodeSkillTree` oder anderen Systemen zum Freischalten von Rezepten.
    *   Speichern der freigeschalteten Rezepte über `DualNodeSaveGame`.
