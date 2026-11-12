# Planung: Kapitel „Datenstrukturen“

Vorschlag vom 09.11.2026 zur Abstimmung. Stellung im Kurs: Block 2, nach
„Manpages und Bibliotheksdokumentation“, vor „Pointer und Datenaustausch“.

## Voraussetzungen

- Block 1: Variablen, Bedingungen, Schleifen, Funktionen, eigene Tests
- „Vom Problem zum Programm II“: Spielbeschreibung und erster Plan des Tresorraubs
- „Manpages“: Funktionsverträge in der Dokumentation nachschlagen

Arrays, Zeichenketten und Strukturen sind neu. Pointer, dynamischer Speicher
und Listen kommen erst in späteren Kapiteln und werden hier bewusst vermieden.

## Leitidee

Die Studierenden bauen über das ganze Kapitel ein Programm, das den
Startzustand des Tresorraubs speichert und ausgibt. Jede Seite führt ein
Sprachmittel an einem spielfremden Beispiel ein; die Anwendung auf das Spiel
entwerfen die Studierenden selbst. Vorgegeben sind Verhalten, Daten
(SPIELFELD.md) und erwartete Ergebnisse, aber keine Funktionsnamen,
Signaturen oder Implementierungen.

Wiederkehrender Ablauf je Seite:

1. Vorhersagen und ausprobieren (erst Erwartung notieren, dann ausführen)
2. Aufgabe am Tresorraub mit eigenem Entwurf und eigenen Testfällen
3. Aufklappbare Denkhilfen und Selbstkontrollen mit erwarteten Werten
4. Multiple-Choice-Fragen zur Lernerfolgskontrolle, jeweils direkt in dem Abschnitt, dessen Inhalt sie abfragen (in Aufgabenabschnitten vor „Ihr Ergebnis“)

## Seiten

| Seite | Inhalt | Ergebnis am Tresorraub | Schwerpunkt der Kontrollfragen |
|---|---|---|---|
| 1 Eindimensionale Arrays | Deklaration, Initialisierung, Indizes 0 bis n−1, undefiniertes Verhalten, Bytes und `sizeof` (knüpft an die Bits aus „Datentypen“ an), Länge als Parameter | Auswertung mehrerer Spielversuche (Anzahl im Limit, Minimum, Durchschnitt) | Indexgrenzen, Initialisierung, `sizeof` |
| 2 Zeichenketten | `'\0'`, `sizeof` vs. `strlen`, UTF-8-Bytes, `strcmp`, `snprintf` (Nachschlagen mit Manpages) | Statuszeile mit Prüfung auf Kürzung | Nullzeichen, Vergleich, Rückgabewerte |
| 3 Zweidimensionale Arrays | Zeilen und Spalten, `[y][x]`, Konstanten mit `#define`, Prüfreihenfolge; Entwurfsfrage `char` vs. `int`-Codes (Daten und Darstellung), Falle der zusätzlichen Spalte für `'\0'` | Grundriss speichern, ausgeben, zählen; Abfragen „innerhalb“ und „begehbar“ | Indexreihenfolge, Platz für `'\0'`, Bereichsprüfung |
| 4 Strukturen | `typedef struct`, Feldzugriff, Initialisierung, Kopie bei Zuweisung und Aufruf, kein `==` | Typen für Position und Wächter, Array von Wächtern, Rasterabstand, überwachte Felder | Kopiersemantik, verschachtelter Zugriff, Reichweite |
| 5 Spielstand | Strukturen mit Arrays, Entwurfsentscheidungen, Kopie großer Strukturen | Spielstand-Typ, Startzustand, Darstellung mit Legende, Textausgabe | Kopieren von Arrays vs. Strukturen, Entwurf |

Seite 5 endet mit einer Beobachtung, die zum Kapitel „Pointer“ überleitet:
Eine Funktion kann den Spielstand des Aufrufers nicht verändern.

## Daten

Das Spielfeld entspricht dem Referenzspiel aus
`Block2_Tresorraub_Arbeitsfassung/referenz/c-spiel-mit-gui` (Grundriss,
Positionen, Wächter mit Reichweite 1 und 2, Überwachung über den
Manhattan-Abstand). Spätere Kapitel können darauf aufbauen.

Geprüfte Kontrollwerte (mit einem Python-Skript nachgerechnet):

- 57 Wandfelder, 78 freie Felder
- 17 freie Felder sind überwacht
- (3, 7) nicht überwacht; (2, 5), (9, 7), (11, 5) überwacht; (12, 5) nicht
- Beispieldaten Seite 1: 4 Versuche im Limit, Minimum 37, Durchschnitt 50,2

Alle C-Aussagen der Erklärungen und Kontrollfragen wurden mit GCC
(`-std=c17 -Wall -Wextra -Wpedantic`) nachgeprüft, unter anderem: Rest-Initialisierung
mit 0, `sizeof`-Werte, `strlen("Tür")` = 4 in UTF-8, Rückgabewert von
`snprintf` bei Kürzung, Fehler bei `==` für Strukturen, Fehler bei Zuweisung an
ein Array, Fehler bei Initialisierung eines Arrays mit `const int`-Größe,
Kopie eines Arrays innerhalb einer Struktur.

## Startmaterial

`materialien/tresorraub-daten.zip` enthält:

- das einheitliche SDL2-Programmgerüst für Block 2 (`programm.c`,
  `tresorraub.h`), aufgebaut wie in „Manpages“, mit `--text`
  (`text_ausgeben()` einmal ohne Fenster) und den Anschlüssen
  `spiel_starten`, `taste_gedrueckt`, `spiel_status`, `aufraeumen` für die
  folgenden Kapitel
- `tresorraub.c` als Datei der Studierenden mit `text_ausgeben()` und
  `zeichne()`; `zeichne()` zeigt als Beispiel eine Spielfigur. Die übrigen
  vier Funktionen sind Platzhalter bis zum Kapitel „Pointer“
- die Piktogramme aus „Manpages“ (Kopie von
  `manpages/materialien/sdl2-start/piktogramme/`)
- `SPIELFELD.md`, `README.md`, `Makefile`

So verbringen die Studierenden keine Zeit mit der SDL2-Anbindung. Für Arrays,
Zeichenketten und Strukturen gibt es weiterhin keinen vorbereiteten Code.
Seiten 3 bis 5 enthalten je eine Teilaufgabe zur Darstellung im Fenster.

Nach Änderungen im Ordner `materialien/tresorraub-daten/` das ZIP neu erzeugen
(ohne `programm`, `programm.exe` und versteckte Dateien).

## Musterlösung

`loesung/` enthält eine vollständige Lösung aller Aufgaben, Tests für alle
erwarteten Werte der Selbstkontrollen und ein Makefile, das Gerüst und
Piktogramme aus dem Startmaterial verwendet. Der Ordner ist auf keiner Seite
verlinkt und nicht im ZIP.

**Achtung:** Das Repository ist öffentlich und wird über GitHub Pages
veröffentlicht. Die Musterlösung ist damit für alle sichtbar, die das
Repository auf GitHub öffnen, und unter
`…/datenstrukturen/loesung/tresorraub.c` auch über die Kursseite abrufbar.

## Zeitbedarf (Schätzung)

Etwa zwei Termine: Seiten 1–3 im ersten, Seiten 4–5 im zweiten Termin.
Seite 5 kann bei Zeitmangel auf Abschnitt 1 und 2 gekürzt werden.

## Offene Punkte

- Umfang abstimmen: Reichen zwei Termine, oder wird Seite 1 (Versuchsauswertung)
  gekürzt?
- Soll `enum` hier schon vorkommen (z. B. für Gegenstände), oder erst später?
- Die alte Platzhalterseite `arrays-zeichenketten/index.html` ist nicht mehr
  verlinkt und kann entfernt werden.
- Musterlösung vor Studierenden verbergen? Möglich über ein privates
  Repository für Lösungen oder indem GitHub Pages nur einen Teilordner
  veröffentlicht.
