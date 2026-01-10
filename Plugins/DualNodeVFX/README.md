# DualNodeVFX

## Zweck

Dieses Plugin dient als zentraler Manager für alle visuellen Effekte (VFX), insbesondere Niagara-Systeme. Es entkoppelt das Auslösen eines Effekts von der tatsächlichen Implementierung und den Assets, die dafür verwendet werden. Gameplay-Code sagt "spiele einen Treffer-Effekt", und dieses Plugin entscheidet, *welchen* Effekt es wo und wie abspielt.

## Entkoppelte Integration

`DualNodeVFX` ist, genau wie `DualNodeAudio` und `DualNodeUI`, ein reines "Zuhörer"-Plugin. Es reagiert auf Gameplay-Events, aber die Gameplay-Systeme wissen nichts von seiner Existenz.

*   **Stellt bereit (Provides):**
    *   Ein `VFXSubsystem`, das als zentrale Anlaufstelle für das Abspielen von Effekten dient.
    *   Eine `VFXRegistry` (Daten-Tabelle), die abstrakte "Event Tags" (z.B. `Impact.Flesh.Small`) auf konkrete Niagara-Systeme und Parameter mappt.
    *   Eine Bibliothek von Helper-Funktionen, z.B. zum Abspielen von Effekten basierend auf der physikalischen Oberfläche.

*   **Hört zu (Listens to):**
    *   Das `VFXSubsystem` bindet sich an die Delegates relevanter Gameplay-Plugins:
        *   `DualNodeStats::OnDamageTaken` -> Spielt einen "Impact"-Effekt am Trefferort.
        *   `DualNodeInventory::OnItemUsed` -> Spielt einen "Use"-Effekt am Spieler.
        *   `DualNodeQuest::OnQuestCompleted` -> Spielt eine "Quest abgeschlossen"-Fanfare.
        *   `DualNodeCrafting::OnCraftingSuccess` -> Spielt einen "Herstellungs"-Effekt an der Werkbank.

*   **Andockpunkte für andere Plugins:**
    *   Dieses Plugin ist ein Endpunkt. Es ruft keine Gameplay-Logik auf. Stattdessen empfängt es Anfragen (z.B. `PlayVFX(Tag, Location)`) und führt sie aus. Die Gameplay-Plugins senden ihre Anfragen "ins Leere", ohne zu wissen, ob das VFX-Plugin überhaupt aktiv ist.

## Roadmap

1.  **Phase 1: Subsystem & Registry**
    *   Erstellung des `VFXSubsystem` mit einer zentralen Funktion `PlayVFX(FGameplayTag EventTag, FVector Location, FRotator Rotation, ...)`
    *   Aufbau der Datenstruktur für die `VFXRegistry`, die Tags mit Niagara-Assets und Standardparametern verknüpft.

2.  **Phase 2: Basis-Integration**
    *   Implementierung der Listener für die wichtigsten Gameplay-Events (Schaden, Heilung, Item-Nutzung).
    *   Erstellung einer einfachen Logik, die basierend auf dem `PhysicalSurfaceType` unterschiedliche Effekte aus der Registry auswählt (z.B. Einschlag in Fleisch vs. Metall).

3.  **Phase 3: Erweiterung & Optimierung**
    *   Entwicklung eines Systems für das Pooling von Niagara-Effekten, um die Performance zu verbessern.
    *   Erstellung von Komponenten, die an Actors gehängt werden können, um zustandsbasierte Effekte darzustellen (z.B. ein "Brennen"-Effekt, der so lange läuft, wie ein `GameplayEffect` aktiv ist).
