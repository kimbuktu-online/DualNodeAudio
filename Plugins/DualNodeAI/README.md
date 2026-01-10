# DualNodeAI

## Zweck

Dieses Plugin stellt eine Bibliothek von wiederverwendbaren KI-Grundlagen und -Verhaltensweisen zur Verfügung. Das Ziel ist es, die Erstellung von Gegnern und NPCs zu beschleunigen, indem grundlegende Logik (wie Patrouillieren, Angreifen, Flüchten) als modulare Komponenten bereitgestellt wird.

## Entkoppelte Integration

Die KI interagiert nicht mit konkreten Klassen, sondern mit abstrakten Interfaces. Sie ist ein "Anfrager" und "Nutzer" der von anderen Systemen bereitgestellten Informationen.

*   **Stellt bereit (Provides):**
    *   Basisklassen für `AIController` und `Character`.
    *   Eine Bibliothek von `Behavior Tree` Tasks und Services (z.B. `BTTask_FindPatrolPoint`, `BTService_DetectTarget`).
    *   Ein `UAISense_Damage` für das Perception System, damit KIs auf Schaden reagieren können, auch wenn sie den Angreifer nicht sehen.

*   **Hört zu (Listens to):**
    *   Das Perception System ist der primäre "Zuhörer" für Sicht- und Hör-Events.
    *   Kann optional auf Delegates von `DualNodeStealth` oder `DualNodeWorldEvents` hören, um auf globale Zustandsänderungen zu reagieren.

*   **Andockpunkte für andere Plugins:**
    *   **DualNodeStats:** Die KI sucht nach Zielen, die das `IDamageable`-Interface implementieren. Wenn ein Ziel gefunden wird, greift die KI es an und ruft dessen `TakeDamage`-Funktion auf. Die KI selbst implementiert ebenfalls `IDamageable`.
    *   **DualNodeFactions:** Vor einem Angriff fragt die KI das `FactionSubsystem`: `GetAttitude(MyFaction, TargetFaction)`. Sie greift nur an, wenn die Antwort `Hostile` ist.
    *   **DualNodeInventory:** Wenn eine KI stirbt, kann sie das `DualNodeInventorySubsystem` anweisen, eine Loot-Tabelle an ihrer Position zu spawnen.
    *   **DualNodeDialogue:** NPCs, die Dialoge führen, nutzen eine `DialogueComponent` aus dem `DualNodeDialogue`-Plugin, aber ihr grundlegendes Verhalten (z.B. Herumwandern) wird von diesem AI-Plugin gesteuert.

## Roadmap

1.  **Phase 1: Basis-Verhalten**
    *   Implementierung eines einfachen Patrouillen-Verhaltens mittels `Behavior Tree`.
    *   Erstellung eines `BTTask` zum Finden und Angreifen eines Ziels.

2.  **Phase 2: Wahrnehmung & Reaktion**
    *   Integration des `AIPerceptionComponent` zur Erkennung von Gegnern durch Sehen und Hören.
    *   Implementierung der Logik zur Reaktion auf Schaden (`UAISense_Damage`).

3.  **Phase 3: Integration mit anderen Systemen**
    *   Anbindung an `DualNodeFactions`, um Freund-Feind-Erkennung zu ermöglichen.
    *   Implementierung der `IDamageable`-Schnittstelle für die Interaktion mit `DualNodeStats`.
    *   Einrichtung der Loot-Spawning-Logik in Zusammenarbeit mit `DualNodeInventory`.
