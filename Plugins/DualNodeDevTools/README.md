# DualNodeDevTools

## Zweck

Dieses Plugin stellt ein In-Game-Debug-Menü und erweiterte Konsolenbefehle zur Verfügung. Es dient ausschließlich zum Testen, Debuggen und zur schnellen Demonstration von Features während der Entwicklung und ist in Release-Builds nicht enthalten.

## Entkoppelte Integration

Dieses Plugin ist der ultimative "Dirigent" und "Kontrolleur". Es ist das einzige Plugin, das explizit über alle anderen Bescheid wissen darf, um sie zu steuern. Kein anderes Plugin weiß jedoch von der Existenz der DevTools.

*   **Stellt bereit (Provides):**
    *   Ein zentrales Debug-Widget, das über eine Tastenkombination aufgerufen wird.
    *   Eine Reihe von Cheat-Konsolenbefehlen (z.B. `dn.giveitem`, `dn.setstat`).
    *   Die Möglichkeit, sich Debug-Informationen verschiedener Systeme auf dem Bildschirm anzeigen zu lassen (z.B. aktueller Quest-Status, KI-Zustand).

*   **Hört zu (Listens to):**
    *   Hört auf eine spezifische Tastenkombination, um sich selbst zu öffnen.

*   **Andockpunkte für andere Plugins:**
    *   Das Plugin ruft über Subsystem-Checks direkt Funktionen in **allen anderen Plugins** auf:
        *   `dn.giveitem <ItemID> <Amount>` -> `DualNodeInventorySubsystem::AddItem(...)`
        *   `dn.setstat <StatName> <Value>` -> `DualNodeStatsSubsystem::ApplyEffect(...)`
        *   `dn.completequest <QuestID>` -> `DualNodeQuestSubsystem::ForceCompleteQuest(...)`
        *   `dn.settime <Hour>` -> `DualNodeTimeSubsystem::SetTimeOfDay(...)`
        *   `dn.toggleui` -> `DualNodeUISubsystem::ToggleHUD()`
        *   `dn.god` -> Wendet einen unsterblichen Gameplay-Effekt über `DualNodeStats` an.
    *   Die Verbindung ist immer eine Einbahnstraße: DevTools -> Anderes Plugin.

## Roadmap

1.  **Phase 1: Basis-Framework**
    *   Erstellung des Haupt-Debug-Widgets.
    *   Implementierung der Logik, um das Widget nur in Entwicklungs-Builds zu laden und über eine Tastenkombination zu öffnen.
    *   Einrichtung eines Systems zur Registrierung von "Debug-Seiten" oder "Tabs" im Widget.

2.  **Phase 2: Konsolenbefehle**
    *   Implementierung der ersten grundlegenden Cheat-Befehle für Inventar und Stats.
    *   Nutzung von `UCheatManager` für eine einfache Integration.

3.  **Phase 3: Erweiterung**
    *   Hinzufügen von Debug-Seiten und Befehlen für jedes neue Plugin, das dem Framework hinzugefügt wird.
    *   Implementierung von On-Screen-Debug-Anzeigen, die den Zustand von wichtigen Systemen in Echtzeit anzeigen.
