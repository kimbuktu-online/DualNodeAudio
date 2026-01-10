# DualNodeDialogue

## Zweck

Dieses Plugin bietet ein datengetriebenes System zur Erstellung und Ausführung von verzweigten Dialogen. Es soll Autoren ermöglichen, komplexe Gespräche mit Bedingungen und Konsequenzen im Editor zu erstellen, ohne programmieren zu müssen.

## Entkoppelte Integration

Das Dialog-System agiert als Informationsdrehscheibe, die den Zustand des Spiels abfragt und andere Systeme über seine Ergebnisse informiert.

*   **Stellt bereit (Provides):**
    *   Ein `UDialogue` Asset, das einen Gesprächsbaum mit Knoten (Zeilen, Spieler-Antworten, Bedingungen, Events) darstellt.
    *   Ein `UDialogueComponent`, das an einen NPC gehängt wird und einen Dialog starten kann.
    *   Delegates wie `OnDialogueStarted`, `OnDialogueFinished`, `OnDialogueNodeExecuted`.

*   **Hört zu (Listens to):**
    *   Das System ist primär ein "Anbieter", aber seine internen Bedingungsknoten ("Condition Nodes") fragen den Zustand anderer Systeme ab:
        *   `DualNodeQuestSubsystem::GetQuestState()`: Um zu prüfen, ob eine bestimmte Dialogoption angezeigt werden soll.
        *   `DualNodeFactionSubsystem::GetReputation()`: Um zu prüfen, ob ein NPC freundlich genug für eine bestimmte Antwort ist.
        *   `DualNodeInventorySubsystem::HasItem()`: Für "Zeig mir deine Waren"-Optionen oder um zu prüfen, ob der Spieler ein Quest-Item hat.

*   **Andockpunkte für andere Plugins:**
    *   **DualNodeInteraction:** Startet einen Dialog, wenn mit einem Actor interagiert wird, der eine `DialogueComponent` besitzt.
    *   **DualNodeQuest:** Ein "Event Node" im Dialog kann `DualNodeQuestSubsystem::StartQuest()` oder `AdvanceQuest()` aufrufen.
    *   **DualNodeUI:** Hört auf die `On...`-Delegates, um das Dialog-Fenster, die Textzeilen und die Antwortmöglichkeiten anzuzeigen.
    *   **DualNodeAudio:** Kann auf `OnDialogueNodeExecuted` hören, um die passende Sprachausgabe (Voice-Over) abzuspielen.

## Roadmap

1.  **Phase 1: Datenstruktur & Editor**
    *   Definition des `UDialogue` Assets mit einer Graphen-Struktur für Knoten (z.B. `LineNode`, `ChoiceNode`, `ConditionNode`, `EventNode`).
    *   Erstellung eines einfachen Graphen-Editors zur visuellen Bearbeitung der Dialogbäume.

2.  **Phase 2: Ausführungslogik**
    *   Implementierung des `DialogueComponent`, das den Graphen durchläuft und die Knoten nacheinander ausführt.
    *   Implementierung der Logik für die verschiedenen Knotentypen, insbesondere der Bedingungsknoten, die andere Subsysteme abfragen.

3.  **Phase 3: Integration**
    *   Anbindung an `DualNodeUI` zur Darstellung des Dialogs.
    *   Integration mit `DualNodeQuest` und anderen Systemen über "Event Nodes".
    *   Sicherstellen, dass der Zustand von wichtigen Dialog-Entscheidungen über `DualNodeSaveGame` gespeichert werden kann (z.B. "Hat der Spieler diese wichtige Information erhalten?").
