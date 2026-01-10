# DualNodeUI

## Zweck

Dieses Plugin dient als zentraler Manager für die gesamte Benutzeroberfläche (UI). Es verwaltet das HUD, Menüs, Pop-ups und Benachrichtigungen. Sein Hauptzweck ist es, die UI-Logik von der Gameplay-Logik zu entkoppeln und als "Zuhörer" für alle anderen Systeme zu agieren.

## Entkoppelte Integration

`DualNodeUI` ist das Paradebeispiel für ein entkoppeltes "Zuhörer"-Plugin. Es weiß über alle anderen Systeme Bescheid, aber keines der anderen Systeme weiß etwas über die UI.

*   **Stellt bereit (Provides):**
    *   Ein `UUISubsystem`, das den Lebenszyklus von Widgets verwaltet (z.B. das Hinzufügen zum Viewport, das Entfernen, das Verwalten von UI-Layern).
    *   Eine Bibliothek von Basis-Widgets (z.B. `DualNodeHUD`, `DualNodeMenu`, `DualNodeNotificationPopup`).
    *   Ein System zur Verwaltung von "UI Policies", die festlegen, welche UI-Elemente in welchem Spielzustand sichtbar sein sollen (z.B. HUD nur im Gameplay, nicht im Hauptmenü).

*   **Hört zu (Listens to):**
    *   Das `UUISubsystem` bindet sich an die Delegates **aller** relevanten Gameplay-Plugins:
        *   `DualNodeStats::OnAttributeChanged` -> Aktualisiert Lebens-, Mana- und Ausdauerbalken.
        *   `DualNodeInventory::OnInventoryChanged` -> Aktualisiert die Inventaransicht.
        *   `DualNodeQuest::OnQuestUpdated` -> Aktualisiert das Quest-Log und den Tracker.
        *   `DualNodeInteraction::OnInteractableInRange` -> Zeigt den "Interagieren"-Prompt an oder blendet ihn aus.
        *   `DualNodeDialogue::OnDialogueStarted` -> Blendet das Spiel-HUD aus und das Dialogfenster ein.
        *   `DualNodeTargeting::OnTargetAcquired` -> Zeigt das Lock-On-Widget an.

*   **Andockpunkte für andere Plugins:**
    *   Dieses Plugin ist primär ein Endpunkt. Es ruft keine Gameplay-Logik in anderen Systemen auf. Stattdessen empfängt es Daten und stellt sie dar. Die einzige Ausnahme ist, wenn ein UI-Element eine Aktion auslöst (z.B. Klick auf "Item benutzen"), dann ruft die UI-Logik die entsprechende Funktion im jeweiligen Subsystem auf (z.B. `DualNodeInventorySubsystem::UseItem`).

## Roadmap

1.  **Phase 1: Subsystem & Widget-Management**
    *   Erstellung des `UUISubsystem` mit Funktionen zum Hinzufügen und Entfernen von Widgets zum Viewport.
    *   Implementierung eines Layer-Systems (z.B. `Game, Menu, Modal`), um die Z-Ordnung von Widgets zu verwalten.

2.  **Phase 2: HUD-Implementierung**
    *   Erstellung eines Basis-HUDs, das sich an die Delegates von `DualNodeStats` und `DualNodeInventory` bindet.
    *   Implementierung des Interaktions-Prompts.

3.  **Phase 3: Menüs & Integration**
    *   Entwicklung der UI für die Haupt-Plugins (Inventar, Quest-Log, Skill-Tree, Dialog).
    *   Sicherstellen, dass die UI korrekt auf alle relevanten Gameplay-Events reagiert und die Daten anzeigt, die von den Subsystemen bereitgestellt werden.
