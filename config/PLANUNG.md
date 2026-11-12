# Planung: Kapitel „Config“

Stand 12.11.2026. Stellung im Kurs: Block 2, Kapitel 8, nach
„Datei-Ein- und -Ausgabe“, vor „Objektorientierung in C“. Bewusst kurz:
höchstens ein Termin.

## Abgrenzung

- **Spielstand** (Kapitel „Datei-Ein- und -Ausgabe“): der aktuelle
  Fortschritt. Das Programm schreibt und liest sein eigenes Format.
- **Config** (dieses Kapitel): Einstellungen und Ausgangsbedingungen. Ein
  Mensch schreibt die Datei, das Programm liest sie nur. Schwerpunkt ist
  deshalb robustes Einlesen und Prüfen, nicht das Dateiformat an sich.

## Voraussetzungen

- „Datei-Ein- und -Ausgabe“: `FILE *`, `fopen`, Lesen, `fclose`, Rückgabewerte
  prüfen, Datei auch nach Fehlern schließen
- „Datenstrukturen“: Zeichenketten, `strcmp`, `snprintf`
- „Manpages und Bibliotheksdokumentation“: neue Funktionen nachschlagen
- Spielgerüst: `spiel_starten` liefert `int`; negativ heißt „Spiel kann
  nicht sinnvoll starten“

## Pflichtteil: Spielregeln als Werte

Eine einfache Textdatei mit Zeilen `name = wert` legt Spielregeln fest, die
bisher als Konstanten im Quelltext stehen. Kandidaten mit sichtbarer Wirkung
im Spiel:

| Regel | Bisher | Sichtbare Wirkung |
|---|---|---|
| Zuglimit | `ZUGLIMIT 60` | Fenstertitel „Zuege z/limit“ |
| Reichweite Wächter 1 und 2 | 1 und 2 im Startzustand | überwachte Felder im Bild |
| Größe des Inventars | `INVENTAR_GROESSE 3` | bei 1: zweiter Gegenstand wird abgelehnt |

Lernziele (Entwurf):

- Eine Textdatei zeilenweise lesen und Leerzeilen und Kommentare überspringen.
- Name und Wert einer Zeile trennen und Zahlen sicher umwandeln.
- Unbekannte Namen, fehlende und ungültige Werte erkennen und mit Zeilennummer
  melden.
- Festlegen, was ohne Datei oder bei fehlenden Angaben gilt.

## Ausblick als aufklappbarer Text (kein Pflichtteil)

Hinweis auf das Projekt am Kursende: verschiedene Level mit Maps. Nur Idee
und Leitfragen, kein Lösungsweg. Entwurf:

> **Ausblick: Eigene Level für Ihr Projekt**
>
> Am Ende des Kurses entwickeln Sie ein eigenes Projekt. Das kann auch eine
> Weiterentwicklung dieses Spiels sein. Eine lohnende Erweiterung sind
> verschiedene Level: Grundriss, Startposition und die Lage von Tür, Tresor,
> Gegenständen und Wächtern stehen dann nicht mehr im Quelltext, sondern
> werden aus Dateien gelesen, und das Spiel bietet mehrere Maps an.
>
> Überlegen Sie dabei zum Beispiel: Wie lässt sich eine Map so in einer
> Textdatei beschreiben, dass ein Mensch sie lesen und bearbeiten kann?
> Woran erkennt Ihr Programm, dass eine Map nicht spielbar ist? Wie wechselt
> das Spiel zum nächsten Level, und was geschieht dabei mit Inventar und
> Zugzahl? Was muss ein gespeicherter Spielstand zusätzlich festhalten, damit
> er zur richtigen Map passt?

## Randbedingungen

- Das Spielgerüst bleibt unverändert. Eine Option wie `--config datei` ist
  deshalb nicht möglich; die Datei hat einen festen Namen.
- Startmaterial = Musterlösung von „Datei-Ein- und -Ausgabe“ ohne Versuche.
  Bis dieses Kapitel vorliegt, dient der Endstand von „Dynamischer Speicher
  und Listen“ als Platzhalter. Branch `feature/config` baut vorerst auf
  `feature/dynamischer-speicher` auf.

## Entscheidungen

- Regeln: `zuglimit` (1–999, Standard 60), `reichweite_waechter_1` und
  `reichweite_waechter_2` (0–5, Standard 1 und 2), `inventar_groesse`
  (1–3, Standard 3).
- Fester Dateiname `regeln.txt`, geladen in `spiel_starten` und zu Beginn
  von `text_ausgeben`.
- Datei fehlt: Standardwerte und Hinweis im Terminal.
- Datei fehlerhaft: erster Fehler mit Zeilennummer nach `stderr`, keine
  Werte übernommen („alles oder nichts“), negative Rückgabe.
- Fehlende Regeln in der Datei: Standardwerte.

## Seiten

1. Eine Textdatei Zeile für Zeile lesen (`fgets`, Zeilennummern,
   Kommentare, zu lange Zeilen; Versuche mit `wecker.txt`)
2. Name und Wert auswerten (`strchr`, `sscanf` für den Namen, `strtol` mit
   Endzeiger, Bereich; Aufgabe: Weckzeit einlesen)
3. Aufgabe: Spielregeln aus `regeln.txt` lesen; Prüftabelle; Ausblick
   „Eigene Level“ aufklappbar
