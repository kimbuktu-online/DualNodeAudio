# DualNodeCore

## Zweck

Dieses Plugin ist das **Rückgrat** für jedes Multiplayer-Spiel von "dual node interactive". Es implementiert einen standardisierten, robusten und wiederverwendbaren Game-Flow, der den gesamten Lebenszyklus einer Spielsitzung abdeckt – vom Hauptmenü über die Lobby bis zum Post-Game-Screen. Ziel ist es, die Entwicklungszeit für neue Projekte drastisch zu verkürzen, indem eine stabile Multiplayer-Grundlage "out of the box" bereitsteht.

## Entkoppelte Integration

`DualNodeCore` ist das zentrale Fundament, auf dem andere Systeme aufbauen. Es ist ein "Anbieter" von Zuständen und Events.

*   **Stellt bereit (Provides):**
    *   **Basis-Klassen:** `CoreGameInstance` (Session-Management), `CoreGameState` (globaler Spielzustand), `CorePlayerState` (individueller Spielerzustand) und eine Hierarchie von `CoreGameMode`-Klassen für die verschiedenen Spielphasen.
    *   **Game-Phase State Machine:** Eine replizierte State Machine, die den Übergang zwischen Phasen wie `MainMenu`, `Lobby`, `Gameplay`, `PostGameplay` steuert.
    *   **Delegates:** `OnGamePhaseChanged`, `OnPlayerJoinedLobby`, `OnPlayerLeftLobby`, `OnCountdownTick`.
    *   **Backdoor-Schnittstelle:** Eine "weiche" Schnittstelle im `CoreGameMode`, die es privilegierten Systemen wie `DualNodeDevConnect` ermöglicht, die Standard-Beitrittsregeln zu umgehen.

*   **Hört zu (Listens to):**
    *   Primär auf Engine-Events und Spieler-Input für das Session-Management (Host, Join).

*   **Andockpunkte für andere Plugins:**
    *   **DualNodeUI:** Hört auf `OnGamePhaseChanged`, um die kontextsensitive UI (Lobby-Menü, HUD, Scoreboard) anzuzeigen.
    *   **DualNodeSaveGame:** Wird vom `CoreGameInstance` genutzt, um persistente Spielerdaten und Einstellungen zu laden/speichern.
    *   **DualNodeDevConnect:** Nutzt die Backdoor im `CoreGameMode`, um Entwicklern den Beitritt zu vollen oder privaten Servern zu ermöglichen.

## Roadmap

1.  **Phase 1: Basis-Struktur & Session-Management**
    *   Erstellung der `CoreGameInstance`, `CoreGameState`, `CorePlayerState` und `CoreGameMode`-Basisklassen.
    *   Implementierung der grundlegenden Steam-Session-Logik (Host, Find, Join) im `CoreGameInstance`.

2.  **Phase 2: Game-State-Machine**
    *   Implementierung der replizierten State Machine im `CoreGameState`.
    *   Entwicklung der Logik für die Phasenübergänge, insbesondere das `ServerTravel` von der Lobby zum Spiel-Level.

3.  **Phase 3: Backdoor & Integration**
    *   Implementierung der "weichen" Abfrage im `PreLogin` und `PostLogin` des `CoreGameMode`, die `DualNodeDevConnect` die nötigen Haken zur Verfügung stellt.
    *   Sicherstellen, dass die Delegates zuverlässig von `DualNodeUI` und anderen Systemen empfangen werden können.
