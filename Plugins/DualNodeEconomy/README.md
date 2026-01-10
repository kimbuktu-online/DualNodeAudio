# DualNodeEconomy

## Zweck

Dieses Plugin verwaltet die wirtschaftlichen Aspekte des Spiels. Dazu gehören Währungen, Händler-Inventare sowie das Kaufen und Verkaufen von Gegenständen.

## Entkoppelte Integration

`DualNodeEconomy` agiert als Vermittler zwischen dem Spieler-Inventar und der Welt. Es nutzt andere Systeme, um seine Aktionen auszuführen, ohne tief mit ihnen verflochten zu sein.

*   **Stellt bereit (Provides):**
    *   Ein `EconomySubsystem`, das die Logik für Transaktionen verwaltet.
    *   Ein `IVendor`-Interface, das von jedem Actor (z.B. einem NPC) implementiert werden kann, um als Händler zu fungieren.
    *   Daten-Assets zur Definition von Händler-Inventaren und Preis-Modifikatoren.

*   **Hört zu (Listens to):**
    *   Dieses Plugin ist primär ein "Anbieter" und reagiert auf direkte Aufrufe (z.B. `AttemptToPurchaseItem`).

*   **Andockpunkte für andere Plugins:**
    *   **DualNodeInventory:** Beim Kaufen oder Verkaufen kommuniziert das `EconomySubsystem` direkt mit dem `DualNodeInventorySubsystem`, um Items zu transferieren. Es fragt: "Hat Spieler A genug Platz?" oder "Entferne Item X von Spieler B".
    *   **DualNodeStats:** Währung kann als Attribut im `AttributeSet` von `DualNodeStats` implementiert werden (z.B. "Gold"). Das `EconomySubsystem` würde dann das `AbilitySystemComponent` anweisen, dieses Attribut zu modifizieren. Alternativ kann Währung auch ein normales Item im Inventar sein. Das System sollte beide Methoden unterstützen und prüfen, welche aktiv ist.
    *   **DualNodeInteraction:** Startet eine Handels-Session, wenn mit einem Actor interagiert wird, der das `IVendor`-Interface implementiert.
    *   **DualNodeUI:** Stellt das Händler-Fenster dar, listet die Waren des Händlers und des Spielers auf und initiiert die Kauf-/Verkauf-Anfragen.

## Roadmap

1.  **Phase 1: Währung & Transaktionen**
    *   Implementierung der Logik zur Verwaltung von Währung. Zuerst als Item im Inventar, später optional als Attribut in `DualNodeStats`.
    *   Erstellung der Kernfunktionen `PurchaseItem` und `SellItem` im `EconomySubsystem`.

2.  **Phase 2: Händler-Implementierung**
    *   Definition des `IVendor`-Interfaces.
    *   Erstellung von Daten-Assets für Händler-Inventare, inklusive Logik für das Auffüllen von Waren (z.B. alle 24 Ingame-Stunden via `DualNodeTime`).

3.  **Phase 3: Integration**
    *   Anbindung an `DualNodeUI` für das Handels-Interface.
    *   Sicherstellen, dass die Transaktionen mit `DualNodeInventory` und `DualNodeStats` reibungslos funktionieren.
    *   Speichern von Händler-Zuständen (falls sich deren Inventar dynamisch ändert) über `DualNodeSaveGame`.
