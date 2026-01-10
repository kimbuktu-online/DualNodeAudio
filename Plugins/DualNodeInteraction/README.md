# DualNodeInteraction

## Zweck

Dieses Plugin stellt ein generisches System zur Verfügung, mit dem der Spieler mit Objekten in der Welt interagieren kann. Es ist der "Kleber", der den Spieler mit der Spielwelt verbindet und Aktionen in anderen Systemen auslöst.

## Entkoppelte Integration

Das Interaktions-System ist ein Vermittler. Es findet heraus, *womit* der Spieler interagieren kann, und sagt diesem Objekt dann "interagiere", ohne zu wissen, *was* diese Interaktion bewirkt.

*   **Stellt bereit (Provides):**
    *   Ein `IInteractable` Interface mit einer Funktion `Interact(AActor* Interactor)`. Jeder Actor, der dieses Interface implementiert, wird interaktiv.
    *   Eine `UInteractionComponent` für den Spieler, die kontinuierlich nach interaktiven Objekten in Reichweite sucht.
    *   Delegates wie `OnInteractableFound`, `OnInteractableLost`, `OnInteractionSuccess`.

*   **Hört zu (Listens to):**
    *   Hört auf `DualNodeInput`, um zu wissen, wann der Spieler die Interaktionstaste drückt.

*   **Andockpunkte für andere Plugins:**
    *   **Jedes Objekt in der Welt:** Eine Tür, ein Hebel, ein NPC, eine Truhe – sie alle können das `IInteractable`-Interface implementieren.
    *   **Die `Interact`-Funktion:** Die Magie passiert in der Implementierung der `Interact`-Funktion im jeweiligen Objekt:
        *   Eine Truhe ruft `DualNodeInventorySubsystem::OpenContainer()` auf.
        *   Ein NPC ruft `DualNodeDialogueSubsystem::StartDialogue()` auf.
        *   Ein Quest-Gegenstand ruft `DualNodeQuestSubsystem::AdvanceQuest()` auf.
        *   Ein Hebel ändert seinen eigenen Zustand und öffnet vielleicht eine Tür.
    *   **DualNodeUI:** Hört auf `OnInteractableFound`/`Lost`, um den Interaktions-Prompt (z.B. "E drücken") ein- und auszublenden.

## Roadmap

1.  **Phase 1: Kern-Komponenten**
    *   Definition des `IInteractable`-Interfaces.
    *   Implementierung der `UInteractionComponent` für den Spieler, die mittels Sphere-Trace oder Overlap nach `IInteractable`-Actors sucht.

2.  **Phase 2: Input & Auslösung**
    *   Anbindung an `DualNodeInput`, um die `Interact`-Funktion auf dem aktuell anvisierten Objekt aufzurufen.
    *   Implementierung der Logik zur Auswahl des "besten" Ziels, wenn mehrere Objekte in Reichweite sind (z.B. das, das am nächsten am Fadenkreuz ist).

3.  **Phase 3: Integration & Beispiele**
    *   Refactoring der Interaktionslogik, die eventuell bereits in `DualNodeInventory` existiert, in dieses Plugin, um die Verantwortlichkeiten klar zu trennen.
    *   Erstellung von Beispiel-Actors (z.B. eine einfache Tür, ein Lichtschalter), die das Interface implementieren, um die Funktionalität zu demonstrieren.
