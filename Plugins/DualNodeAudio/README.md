# 🧬 Dual Node Audio (DNA) – System Documentation

**Version:** 11.0 (Enterprise Edition)
**Engine:** Unreal Engine 5.7+
**Architecture:** Data-Driven, Tag-Based, Server-Authoritative

---

## 1. Einleitung & Philosophie

Dual Node Audio (DNA) ist ein hochmodernes Audio-Framework, das entwickelt wurde, um die klassischen Probleme der Unreal Engine Audio-Entwicklung zu lösen: **Spaghetti-Code, String-Referenzen und Multiplayer-Desync.**

### Kern-Features
* **Tag-Based Workflow:** Nie wieder Tippfehler. Nutze `GameplayTags` (z.B. `Audio.Music.Boss`) statt Strings.
* **Priority Stack System:** Intelligentes Musik-Layering (`Combat` überschreibt `Exploration`, `UI` überschreibt alles).
* **Time-Sync Replication:** Musik startet auf Clients exakt an der Stelle, wo sie auf dem Server ist (Sample-accurate Join-in-Progress).
* **Context Awareness:** Automatische Anpassung durch Tageszeit (Time-of-Day) und Umgebungs-Trigger (Volumes).
* **AI Barks:** Integriertes System für KI-Stimmen mit Cooldowns und Wahrscheinlichkeiten.

---

## 2. Installation & Setup

### Schritt 1: Plugin aktivieren
Stelle sicher, dass das Plugin im `Plugins` Ordner liegt und aktiviere es unter **Edit -> Plugins -> Dual Node Audio**.

### Schritt 2: Registry erstellen
DNA benötigt eine zentrale Datenbank.
1.  Rechtsklick im Content Browser -> **Miscellaneous -> Data Asset**.
2.  Wähle `DualNodeAudioRegistry`.
3.  Nenne es `DA_Audio_Global`.

### Schritt 3: Projekt konfigurieren (WICHTIG!)
Damit das Subsystem die Daten findet:
1.  Gehe zu **Project Settings -> Game -> Dual Node Audio**.
2.  Füge unter **Registries** dein `DA_Audio_Global` hinzu.

### Schritt 4: Multiplayer Setup
Damit RPCs (Netzwerk-Befehle) funktionieren, müssen zwei Komponenten existieren:
1.  **Global Audio (Music Manager):**
    * Öffne deinen `GameMode`.
    * Im `BeginPlay`: Spawn Actor of Class -> `DualNodeMusicManager`.
2.  **Local/Targeted Audio (Player Component):**
    * Öffne deinen `PlayerController`.
    * Füge via "+ Add" den `DualNodePlayerAudioComponent` hinzu.

---

## 3. Die Registry (Datenbank)

Die Registry (`DA_Audio_Global`) ist das Herzstück. Hier verknüpfst du Tags mit Assets.

### A. Sound Database (SFX)
Für One-Shot Sounds (Schüsse, UI, Explosionen).
* **Sound:** Der MetaSound/Cue/Wave.
* **Concurrency:** (Wichtig!) Verhindert Audio-Spam. Lege hier fest "Max 3 Instanzen".
* **Physics Map:** Verknüpfe `PhysicalMaterials` (z.B. PM_Grass) mit alternativen Sounds (Schritte).

### B. Music Database (BGM)
Für Loops und Musik.
* **Layers:** Definiere Intro, Loop und Outro Verhalten.
* **Time-of-Day:** Aktiviere `bUseTimeOfDay`, um Tracks nur zu bestimmten Uhrzeiten (z.B. 20:00 - 06:00) zuzulassen.

### C. Bark Database (KI Stimmen) - *Neu in v11.0*
Für Ausrufe wie "Nachladen!", "Deckung!".
* **Cooldown Group:** Ein Tag (z.B. `AI.Bark.Reload`). Wenn *irgendein* Bark dieser Gruppe spielt, ist die ganze Gruppe für `CooldownDuration` gesperrt.
* **Trigger Chance:** 0.0 bis 1.0 (Zufallswahrscheinlichkeit).

---

## 4. Blueprint API (Nodes)

Alle Funktionen findest du unter der Kategorie `Dual Node Audio`.

### 🎵 Musik Steuerung

#### `Set Global Music`
* **Nutzen:** Startet Musik für **alle** Spieler (Server Authority).
* **Input:**
    * `Priority`: Wie wichtig ist das? (`Ambient` < `Combat` < `Critical`).
    * `Tag`: Welcher Song?
    * `Settings` (Optional): Überschreibe Fade-Zeit, Pitch oder Startzeit zur Laufzeit.

#### `Set Local Music`
* **Nutzen:** Startet Musik nur für den **lokalen** Spieler (z.B. Hauptmenü, Inventar).
* **Besonderheit:** Überschreibt Global Music, solange die Priorität höher oder gleich ist.

#### `Pause / Resume Global Music`
* **Nutzen:** Friert die Musik ein (z.B. bei Pause-Menü) und setzt sie später exakt fort.

---

### 🔊 Sound Effects (SFX)

#### `Play Sound By Tag (2D / At Location)`
* **Nutzen:** Standard Fire-and-Forget Sound.
* **Network:** Spielt nur lokal! Für Multiplayer nutze `Broadcast Sound`.

#### `Broadcast Sound By Tag`
* **Nutzen:** Spielt einen Sound an einer Position für **alle** Spieler.
* **Reliable:** `Boolean`. Setze auf `True` für wichtige Gameplay-Cues (Level Up), `False` für Explosionen/Schüsse (Bandbreite sparen).

#### `Play Sound By Tag With Physics`
* **Nutzen:** Führt automatisch einen LineTrace durch, liest das Material des Bodens/Wand und spielt die passende Variation aus der Registry.

#### `Play Bark By Tag`
* **Nutzen:** Versucht einen KI-Shout abzuspielen. Handhabt automatisch Cooldowns und Zufallschancen. Gibt `false` zurück, wenn blockiert.

---

### 🌍 World & Environment

#### `DualNodeAudioVolume` (Actor)
Ziehe diesen Actor in dein Level.
* **Funktion:** Wenn der Spieler das Volume betritt, wird automatisch der definierte `Music Tag` auf dem gewählten `Priority Slot` gestartet.
* **Client Only:** Wenn angehakt, passiert das nur lokal (Ambience). Wenn aus, triggert der Server das für alle (Global Events).

#### `Set System Time Of Day`
* **Input:** Float (0.0 - 24.0).
* **Funktion:** Aktualisiert die interne Uhr von DNA. Musik-Tracks mit Time-of-Day Restrictions faden automatisch ein/aus.

---

## 5. Prioritäts-System (Stack Logic)

Das System spielt immer nur den Sound des Slots mit der **höchsten Priorität**, der gerade aktiv ist.

1.  **Critical / Cutscene** (Höchste)
2.  **UI / Menu**
3.  **Combat / Event**
4.  **Gameplay / State**
5.  **Ambient / Exploration** (Niedrigste)

**Beispiel:**
* `Ambient` spielt "Wald-Loop".
* `Combat` wird aktiviert ("Boss-Musik"). -> System fadet zu Boss-Musik.
* Spieler drückt Pause (`UI` aktiviert "Pause-Loop"). -> System fadet zu Pause-Musik.
* Spieler drückt Resume (`UI` wird gelöscht). -> System fadet zurück zu Boss-Musik (Combat).

---

## 6. Runtime Overrides (Playback Settings)

In fast allen Musik-Nodes kannst du das Struct `PlaybackSettings` aufklappen.
* **Werte < 0:** Nutzen den Standard aus der Registry (Data Asset).
* **Werte >= 0:** Überschreiben den Standard temporär für diesen Aufruf.
* **Use Case:** Ein Bosskampf soll sofort (0s Fade) starten, statt wie üblich mit 2s Fade. Setze `FadeInDuration = 0.0`.

---

## 7. Limitierungen & Best Practices

1.  **Kein Audio im Konstruktor:** Rufe DNA-Funktionen niemals im Construction Script auf. Nutze `BeginPlay`.
2.  **Server Authority:** `SetGlobalMusic` und `BroadcastSound` funktionieren nur, wenn sie vom Server (Authority) aufgerufen werden. Nutze `SwitchHasAuthority` in Blueprints.
3.  **Tags pflegen:** Lösche keine Tags aus der Registry, die im Code/Level verwendet werden. Aktiviere `Warn On Missing Tags` in den Project Settings, um Lücken zu finden.
4.  **Netzwerk-Bandbreite:** Nutze `BroadcastSound` (Reliable) sparsam. Zu viele Reliable RPCs können die Verbindung verstopfen. Für Maschinengewehr-Feuer nutze `Unreliable` oder Client-seitige Simulation.

---

**Ende der Dokumentation.**
*Built for High-Performance Unreal Engine 5.7 Projects.*