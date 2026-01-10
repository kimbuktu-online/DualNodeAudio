# DualNodeDevConnect

## Zweck

Dieses Plugin ist ein hochprivilegiertes **Entwickler-Werkzeug** für Live-Ops, Community-Interaktion und Remote-Debugging. Es ermöglicht es einem auf einer Whitelist stehenden Entwickler, jede aktive Spielsitzung unbemerkt zu betreten, das Geschehen zu beobachten, dem Voice-Chat zuzuhören und bei Bedarf einzugreifen. Der Name signalisiert die Funktion: Ein Werkzeug für **Devs** zum **Verbinden**.

**WICHTIG:** Dieses Plugin ist optional und darf **niemals** in einem finalen Shipping-Build für Kunden enthalten sein. Es wird für den Release-Build im `.uproject` deaktiviert.

## Entkoppelte Integration

`DualNodeDevConnect` ist ein reiner "Nutzer" des `DualNodeCore`-Plugins und anderer Systeme. Es wird über eine "weiche" Abhängigkeit integriert, sodass `DualNodeCore` nichts von seiner Existenz wissen muss.

*   **Stellt bereit (Provides):**
    *   Ein `DevConnectSubsystem`, das nur für whitelisted Entwickler aktiv ist.
    *   Ein Einstellungs-Asset, in dem die Steam-IDs der Entwickler hinterlegt sind.
    *   Ein spezielles Entwickler-Menü (UI), das alle aktiven Spielsitzungen auflistet.
    *   Einen unsichtbaren `SpectatorPawn` und einen `SpectatorPlayerController` mit Admin-Fähigkeiten.

*   **Hört zu (Listens to):**
    *   Nutzt das `OnlineSubsystem`, um die Liste aller aktiven Server abzurufen.

*   **Andockpunkte (So funktioniert die Magie):**
    *   **Die Backdoor:** Das Plugin nutzt die Schnittstelle im `DualNodeCore::CoreGameMode`.
        1.  Im `PreLogin` des `CoreGameMode` wird das `DevConnectSubsystem` abgefragt: `IsDeveloper(SteamID)`.
        2.  Wenn `true`, werden alle Beitrittsbeschränkungen (Server voll, Passwort) vom `CoreGameMode` ignoriert.
        3.  Im `PostLogin` weist das `DevConnectSubsystem` den `CoreGameMode` an, einen `SpectatorPawn` anstelle eines normalen Spieler-Pawns zu spawnen.
    *   **Die Tarnung:** Der `PlayerState` des Entwicklers wird markiert, sodass er für `DualNodeUI` (Spielerliste, Scoreboard) unsichtbar ist.
    *   **Steam Voice:** Der `SpectatorPlayerController` verwaltet die Voice-Verbindung und ermöglicht eine "Push-to-Talk"-Funktion nur für Entwickler.

## Roadmap

1.  **Phase 1: Whitelist & UI**
    *   Erstellung des Einstellungs-Assets für die Entwickler-Steam-IDs.
    *   Entwicklung des UI-Menüs, das alle Server über das `OnlineSubsystem` findet und anzeigt.

2.  **Phase 2: Backdoor-Implementierung**
    *   Implementierung der `IsDeveloper`-Logik im `DevConnectSubsystem`.
    *   Sicherstellen, dass der `CoreGameMode` korrekt auf die Anweisungen des Subsystems reagiert (Beitritt erlauben, Spectator spawnen).

3.  **Phase 3: Spectator-Modus**
    *   Erstellung des `SpectatorPawn` (unsichtbar, noclip) und des `SpectatorPlayerController`.
    *   Implementierung der Push-to-Talk-Logik für die Voice-Kommunikation.

4.  **Phase 4: Admin-Fähigkeiten**
    *   Integration mit `DualNodeDevTools`, sodass der Entwickler im Spectator-Modus Cheat-Befehle auf dem Server ausführen kann.
