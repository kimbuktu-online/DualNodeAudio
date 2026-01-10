# DualNodeQuest

## Zweck

Dieses Plugin verwaltet Quests, deren Ziele, Bedingungen und Belohnungen. Es soll ein robustes, datengetriebenes System sein, das komplexe Quest-Strukturen ermöglicht, ohne dass Gameplay-Programmierer tief in den Code eingreifen müssen.

## Entkoppelte Integration

`DualNodeQuest` ist ein zentraler "Dirigent", der viele andere Systeme nutzt, um seinen Zustand zu überprüfen und zu aktualisieren.

*   **Stellt bereit (Provides):**
    *   Ein `UQuest` Asset, in dem eine Quest mit all ihren Schritten, Zielen und Belohnungen in einem Editor definiert wird.
    *   Ein `UQuestLogComponent`, das dem Spieler hinzugefügt wird und seinen gesamten Quest-Fortschritt verfolgt.
    *   Delegates wie `OnQuestAccepted`, `OnQuestObjectiveCompleted`, `OnQuestCompleted`.

*   **Hört zu (Listens to):**
    *   Das Plugin hört auf eine Vielzahl von Delegates aus anderen Systemen, um den Quest-Fortschritt zu validieren:
        *   `DualNodeAI::OnEnemyDefeated`: Für "Töte X Gegner"-Ziele.
        *   `DualNodeInventory::OnItemAdded`: Für "Sammle X Items"-Ziele.
        *   `DualNodeDialogue::OnDialogueFinished`: Für "Sprich mit NPC X"-Ziele.
        *   Es kann auch Zonen (Trigger Volumes) nutzen, um "Erreiche Ort X"-Ziele zu erfüllen.

*   **Andockpunkte für andere Plugins:**
    *   **DualNodeUI:** Hört auf die `OnQuest...`-Delegates, um das Quest-Log, Tracker und Benachrichtigungen zu aktualisieren.
    *   **DualNodeDialogue:** Kann Quests starten oder abschließen. Dialogoptionen können vom Quest-Status abhängen (z.B. "Ich habe die 10 Wolfsfelle für dich").
    *   **DualNodeEconomy / DualNodeInventory:** Bei Quest-Abschluss ruft das `QuestLogComponent` die Subsysteme dieser Plugins auf, um Belohnungen (Gold, Items) zu vergeben.
    *   **DualNodeSaveGame:** Das `QuestLogComponent` serialisiert seinen Zustand (aktive Quests, abgeschlossene Ziele) und übergibt die Daten zum Speichern.

## Roadmap

1.  **Phase 1: Datenstruktur**
    *   Definition des `UQuest` Assets mit einer Struktur für Phasen, Ziele (Objectives) und Belohnungen.
    *   Erstellung des `UQuestLogComponent` zur Verwaltung der aktiven und abgeschlossenen Quests eines Spielers.

2.  **Phase 2: Ziel-Typen & Validierung**
    *   Implementierung der Logik für die gängigsten Ziel-Typen (Töten, Sammeln, Interagieren, Erreichen).
    *   Einrichtung der Delegate-Listener, um auf die entsprechenden Gameplay-Events zu reagieren.

3.  **Phase 3: Integration & UI**
    *   Anbindung an `DualNodeSaveGame` zum Speichern des Fortschritts.
    *   Bereitstellung von Daten und Delegates für `DualNodeUI` zur Anzeige des Quest-Logs und der Marker auf der Weltkarte (`DualNodeMap`).
    *   Integration mit `DualNodeDialogue`, um Quests über NPCs zu verwalten.
