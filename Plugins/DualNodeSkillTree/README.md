# DualNodeSkillTree

## Zweck

Dieses Plugin bietet ein datengetriebenes System zur Erstellung und Verwaltung von Fähigkeitsbäumen. Spieler können Punkte investieren, um neue Fähigkeiten, passive Boni oder andere Vorteile freizuschalten.

## Entkoppelte Integration

Der Skill-Tree ist ein "Dirigent". Er enthält selbst keine Gameplay-Logik, sondern weist andere Systeme an, dem Spieler neue Fähigkeiten oder Boni zu geben.

*   **Stellt bereit (Provides):**
    *   Ein `USkillTree` Asset, das den Graphen des Fähigkeitsbaums mit Knoten, Verbindungen und Kosten definiert.
    *   Ein `USkillTreeComponent`, das dem Spieler hinzugefügt wird und den Fortschritt im Baum (freigeschaltete Knoten) speichert.
    *   Delegates wie `OnSkillUnlocked`.

*   **Hört zu (Listens to):**
    *   Hört auf `DualNodeStats::OnExperienceChanged` oder `OnLevelUp`, um Fähigkeitspunkte zu vergeben.

*   **Andockpunkte für andere Plugins:**
    *   **DualNodeStats:** Wenn ein Knoten freigeschaltet wird, der eine neue Fähigkeit gewährt, ruft das `SkillTreeComponent` das `AbilitySystemComponent` des Spielers auf und führt `GiveAbility()` aus. Für passive Boni wird ein permanenter `GameplayEffect` angewendet.
    *   **DualNodeCrafting:** Ein freigeschalteter Knoten kann das `CraftingSubsystem` anweisen, ein neues Rezept freizuschalten (`UnlockRecipe`).
    *   **DualNodeUI:** Stellt die Skill-Tree-Ansicht dar, zeigt freischaltbare Knoten an und sendet die "Unlock"-Anfrage an das `SkillTreeComponent`.
    *   **DualNodeSaveGame:** Das `SkillTreeComponent` serialisiert die Liste der freigeschalteten Knoten-IDs zum Speichern.

## Roadmap

1.  **Phase 1: Datenstruktur & Editor**
    *   Definition des `USkillTree` Assets mit einer Graphen-Struktur für Knoten (Skills) und deren Verbindungen.
    *   Erstellung eines Graphen-Editors zur visuellen Bearbeitung der Fähigkeitsbäume. Jeder Knoten sollte definieren können, *was* er gewährt (z.B. ein `GameplayAbility`-Asset, ein `GameplayEffect`-Asset oder eine `CraftingRecipe`-Referenz).

2.  **Phase 2: Kernlogik**
    *   Implementierung des `SkillTreeComponent` mit der Logik zum Freischalten von Knoten und zum Verwalten von Fähigkeitspunkten.
    *   Implementierung der "Unlock"-Funktion, die prüft, ob die Voraussetzungen erfüllt sind und die Kosten abgezogen werden können.

3.  **Phase 3: Integration**
    *   Anbindung an `DualNodeStats`, um Fähigkeiten und Effekte zu gewähren.
    *   Anbindung an `DualNodeCrafting` zum Freischalten von Rezepten.
    *   Anbindung an `DualNodeUI` für die Darstellung.
    *   Speichern des Fortschritts über `DualNodeSaveGame`.
