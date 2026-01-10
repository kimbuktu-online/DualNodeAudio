# Zukünftige Plugin-Ideen für das DualNode-Framework

Dieses Dokument listet potenzielle zukünftige Plugins auf, die das DualNode-Framework erweitern könnten. Die Ideen sind nach Priorität und Komplexität geordnet.

---

## Kern-Erweiterungen

Diese Plugins würden das Kern-Gameplay-Framework erheblich erweitern und vervollständigen.

*   **`DualNodeFactions` (Fraktionen & Rufsystem):** Verwaltet die Beziehungen zwischen Gruppen (Spieler, Stadtwache, Orks). Definiert, wer wem gegenüber freundlich, neutral oder feindlich gesinnt ist. Andere Systeme (AI, Dialogue) fragen dieses System um Rat.

*   **`DualNodeInput` (Erweitertes Input-Management):** Verwaltet kontextabhängige Input-Zustände (Gameplay, Menü, Dialog, Fahrzeug). Verhindert, dass der Spieler im Dialog kämpfen kann.

*   **`DualNodeWeather` (Dynamisches Wettersystem):** Steuert globale Wetterzustände (Regen, Schnee, Nebel) und deren Übergänge. Sendet Events, auf die andere Systeme (Audio, VFX, AI) reagieren können.

*   **`DualNodeMap` (Weltkarten- & Kompass-System):** Verwaltet die Logik für eine In-Game-Weltkarte, das Aufdecken von "Fog of War", Wegpunkte und die Anzeige von Quest-Markern.

## Spezialisierte Gameplay-Systeme

Diese Plugins fügen spezifische, aber oft gewünschte Gameplay-Mechaniken hinzu.

*   **`DualNodeBuilding` (Basenbau-System):** Ermöglicht es Spielern, Strukturen aus vordefinierten Teilen zu errichten. Interagiert stark mit dem Inventar (Ressourcen) und SaveGame (Speichern der Bauten).

*   **`DualNodeStealth` (Schleich-System):** Verwaltet die Logik für Sichtbarkeit und Geräusche. Die KI fragt dieses System, wie gut der Spieler zu sehen ist.

*   **`DualNodeMounts` (Reittier- & Fahrzeugsystem):** Verwaltet das Auf- und Absteigen sowie die Steuerung von Reittieren oder Fahrzeugen.

*   **`DualNodeTargeting` (Fortgeschrittenes Zielerfassungssystem):** Verwaltet Lock-On, Zielwechsel und die Priorisierung von Zielen.

*   **`DualNodeTraversal` (Fortgeschrittene Bewegungsmechaniken):** Ein Framework für Klettern, Rutschen, Wall-Running oder Greifhaken.

*   **`DualNodePets` (Begleiter-System):** Verwaltet nicht-kämpfende Begleiter, die dem Spieler folgen. Kann optional als mobiles Lager dienen.

## Soziale & Meta-Systeme

Diese Plugins konzentrieren sich auf soziale Interaktion und Meta-Fortschritt.

*   **`DualNodeGroups` (Party- & Gilden-System):** Verwaltet soziale Zusammenschlüsse von Spielern.

*   **`DualNodeAchievements` (Erfolgssystem):** Ein datengetriebenes System zum Verfolgen und Freischalten von Erfolgen. Hört auf Events aus allen anderen Systemen.

*   **`DualNodeMail` (Ingame-Postsystem):** Ermöglicht Spielern, sich asynchron Nachrichten und Gegenstände zu schicken.

*   **`DualNodeHousing` (Spieler-Behausung):** Ermöglicht Spielern, ein eigenes, instanziiertes Haus mit Möbeln auszustatten.

*   **`DualNodeTitles` (Titel- & Suffix-System):** Ein kosmetisches System, mit dem Spieler freigeschaltete Titel anzeigen können.

*   **`DualNodeChat` (Text-Chat-System):** Ein Chat-System mit verschiedenen Kanälen.

## "Quality of Life" & Technische Systeme

Diese Plugins verbessern das Spielerlebnis oder die Entwicklungspipeline.

*   **`DualNodeAppearance` (Charakter-Anpassungssystem):** Verwaltet modulare Körperteile, Morph-Targets und Materialparameter für die Charaktererstellung.

*   **`DualNodeProcedural` (Prozedurale Generierung):** Ein Framework zur Erstellung von prozeduralen Inhalten wie Dungeons oder Höhlen.

*   **`DualNodeCutscenes` (Cinematic-Management):** Ein System zum Abspielen von In-Game-Zwischensequenzen und zur Verwaltung des Spielzustands währenddessen.

*   **`DualNodeMinigames` (Minispiel-Framework):** Ein Framework für in sich geschlossene Minispiele (Schlossknacken, Angeln, Hacking).

*   **`DualNodePhoto` (Fotomodus):** Ein vollwertiger Fotomodus, um das Spiel zu pausieren und Screenshots zu machen.

*   **`DualNodeSettings` (Einstellungs-Management):** Ein zentrales System zum Speichern und Laden aller Spieleinstellungen.

*   **`DualNodeLoading` (Ladebildschirm-Management):** Zur Anzeige von dynamischen Ladebildschirmen.

*   **`DualNodeLore` (Journal- & Kodex-System):** Zum Sammeln und Nachlesen von Lore-Einträgen.

*   **`DualNodeTutorial` (Onboarding- & Tutorial-System):** Ein kontextsensitives System, um neuen Spielern die Mechaniken beizubringen.

## Monetarisierungs-Systeme

*   **`DualNodeStore` (Item-Shop / Monetarisierung):** Ein UI- und Logik-Framework für einen Ingame-Shop.

*   **`DualNodeSeasons` (Battle-Pass / Saison-System):** Ein System für zeitlich begrenzte Saisons mit einem Fortschrittspfad.
