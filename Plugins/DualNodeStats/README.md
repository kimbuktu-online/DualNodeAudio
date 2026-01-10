# DualNodeStats

## Zweck

Dieses Plugin dient als Brücke zum **Gameplay Ability System (GAS)** von Unreal Engine. Es stellt die grundlegenden Klassen und Schnittstellen zur Verfügung, um Charakter-Attribute (wie Leben, Mana, Stärke etc.), Zustände und Fähigkeiten zu verwalten. Es soll **GAS nicht ersetzen**, sondern die Nutzung im Kontext unseres Plugin-Frameworks standardisieren und vereinfachen.

## Entkoppelte Integration

Die Entkopplung ist das Kernprinzip dieses Plugins.

*   **Stellt bereit (Provides):**
    *   Basisklassen für `AttributeSet` und `AbilitySystemComponent`, die bereits für die Replikation und das Speichern vorbereitet sind.
    *   C++ Interfaces wie `IDamageable` oder `IStatOwner`, die von beliebigen Actors implementiert werden können, um mit dem System zu interagieren.
    *   Eine Bibliothek von grundlegenden `GameplayAbilities` und `GameplayEffects` (z.B. für Heilung, Schaden, temporäre Buffs).

*   **Hört zu (Listens to):**
    *   Dieses Plugin ist primär ein "Anbieter" und hört nicht aktiv auf andere Systeme. Andere Systeme hören auf seine Delegates (z.B. `OnHealthChanged`).

*   **Andockpunkte für andere Plugins:**
    *   **DualNodeInventory:** Das `ItemFragment_Stats` im Inventar-Plugin enthält eine Referenz auf ein `GameplayEffect`-Asset. Wenn ein Item ausgerüstet wird, wendet das `EquipmentComponent` diesen Effekt auf den `AbilitySystemComponent` des Trägers an. `DualNodeStats` weiß nichts vom Inventar, es empfängt nur die Anfrage, einen Effekt anzuwenden.
    *   **DualNodeUI:** Hört auf Delegates wie `OnAttributeChanged` im `AttributeSet`, um Lebensbalken und andere UI-Elemente zu aktualisieren.
    *   **DualNodeAI:** Greift Actors an, die das `IDamageable`-Interface implementieren, und ruft dessen `TakeDamage`-Funktion auf.

## Roadmap

1.  **Phase 1: Kern-Implementierung**
    *   Erstellung der Basis-Klasse `UDualNodeAttributeSet` mit Standard-Attributen (Health, MaxHealth, Mana, Stamina).
    *   Erstellung der Basis-Klasse `UDualNodeAbilitySystemComponent` mit Helper-Funktionen für die Blueprint-Nutzung.
    *   Definition des `IDamageable`-Interfaces.

2.  **Phase 2: Integration & Speicherung**
    *   Implementierung der Speicherlogik in Zusammenarbeit mit `DualNodeSaveGame`. Das `AbilitySystemComponent` muss seine aktiven Effekte und die `AttributeSet`-Werte serialisieren können.
    *   Sicherstellen, dass das `ItemFragment_Stats` aus `DualNodeInventory` korrekt mit dem `AbilitySystemComponent` kommunizieren kann.

3.  **Phase 3: Erweiterung**
    *   Entwicklung einer Bibliothek von Standard-`GameplayAbilities` (z.B. Sprint, einfacher Angriff).
    *   Erstellung eines `StatModificationComponent`, das die Anwendung von Effekten aus verschiedenen Quellen (Items, Buffs, Zonen) verwaltet.
