# DualNodeTime

## Zweck

Dieses Plugin verwaltet die Zeit im Spiel. Es stellt einen globalen Tag-Nacht-Zyklus, einen Kalender (Tag, Monat, Jahr) und die Möglichkeit zur Verfügung, Events zu bestimmten Zeiten auszulösen.

## Entkoppelte Integration

`DualNodeTime` ist ein reines "Sender"-Plugin. Es sendet regelmäßig Nachrichten über den aktuellen Zeitstatus, weiß aber nicht und muss nicht wissen, wer darauf hört.

*   **Stellt bereit (Provides):**
    *   Ein `TimeSubsystem`, das die aktuelle Spielzeit verwaltet und vorantreibt.
    *   Konfigurierbare Einstellungen für die Länge eines Tages, die Startzeit etc.
    *   Delegates wie `OnHourPassed`, `OnDayChanged`, `OnNightfall`, `OnSunrise`.

*   **Hört zu (Listens to):**
    *   Dieses Plugin hört auf keine anderen Gameplay-Systeme. Es kann jedoch auf Engine-Events wie `OnGamePaused` reagieren, um das Voranschreiten der Zeit anzuhalten.

*   **Andockpunkte für andere Plugins:**
    *   **DualNodeAI:** NPCs können auf `OnNightfall` hören, um schlafen zu gehen, oder auf `OnSunrise`, um ihre täglichen Routinen zu beginnen. Bestimmte Kreaturen können nur nachts spawnen.
    *   **DualNodeWeather:** Das Wettersystem kann seine Muster an die Tageszeit oder Jahreszeit anpassen.
    *   **DualNodeQuest:** Zeitlich begrenzte Quests können auf `OnDayChanged` hören, um ihre verbleibende Zeit zu aktualisieren und potenziell fehlzuschlagen.
    *   **Lighting:** Ein globaler Licht-Manager (oft Teil des Hauptprojekts) hört auf `OnHourPassed`, um die Position und Farbe der Directional Light (Sonne) und des Skylights anzupassen.
    *   **DualNodeDialogue:** NPCs können je nach Tageszeit unterschiedliche Dialoge haben.

## Roadmap

1.  **Phase 1: Kern-Subsystem**
    *   Erstellung des `TimeSubsystem` mit Variablen für die aktuelle Zeit, den Tag und das Jahr.
    *   Implementierung einer `Tick`-Funktion, die die Zeit basierend auf einer konfigurierbaren "Time Scale" vorantreibt.
    *   Deklaration der grundlegenden Delegates (`OnHourPassed`, `OnDayChanged`).

2.  **Phase 2: Event-Auslösung**
    *   Implementierung der Logik, die die Delegates zur richtigen Zeit auslöst (z.B. `OnDayChanged`, wenn die Uhrzeit 00:00 erreicht).
    *   Definition von konfigurierbaren Zeitpunkten für "Sonnenaufgang" und "Sonnenuntergang".

3.  **Phase 3: Integration & Speicherung**
    *   Anbindung an `DualNodeSaveGame`, um die aktuelle Zeit und das Datum zu speichern und zu laden.
    *   Bereitstellung von Blueprint-Funktionen, um die Zeit abzufragen, zu setzen oder vorzuspulen.
