# Planung: Kapitel „Objektorientierung in C“

Stand 12.11.2026. Verantwortung: Larissa (vom Kollegen übernommen).
Stellung im Kurs: Block 2, Kapitel 9, nach „Config“, vor „Die Spielbausteine zusammenführen & Refactoring“.
Grundlage ist der Endstand von „Config“ (`config/loesung/tresorraub.c`).
Stil wie in den vorigen Kapiteln: keine vorgegebenen Funktionsnamen und
Signaturen, Auswahlfragen dort, wo der Inhalt vermittelt wird, Hinweise und
Selbstkontrollen direkt bei der Aufgabe. Umfang: etwa zwei Termine.

## Voraussetzungen

- „Datenstrukturen“: Strukturen, `typedef`, Arrays von Strukturen,
  Zeichenketten
- „Header und Quelldateien“: `static` für Funktionen und Variablen einer
  Datei
- „Pointer und Datenaustausch“: `->`, `const T *`, Pointer auf Felder
- „Dynamischer Speicher und Listen“: Inventar als Liste, Besitz
- „Config“: Tabelle aus Strukturen statt paralleler Arrays (Regelangabe)

## Leitidee: ein sichtbares Problem lösen

Im Endstand von Config ist jede Objektart über die ganze Datei verstreut.
Wer wissen will, was ein Schlüssel ist, findet ihn in `gegenstand_auf`,
`feldzeichen`, `spielfeld_ausgeben`, `vom_feld_nehmen`, `auf_feld_legen`,
`handeln`, beim Zeichnen von Feld und Inventar, in `startzustand` und in
eigenen Feldern des Spielstands (`schluessel`, `schluessel_liegt`),
zusammen zehn Stellen.
Für Tür, Tresor, Beute und Münze gilt dasselbe. Eine neue Objektart hieße:
alle diese Stellen ändern und keine vergessen.

Das Kapitel baut das Spiel so um, dass jede Objektart an einer Stelle
beschrieben ist: welche Daten sie hat, wie sie aussieht und was bei der
Leertaste geschieht. Der übrige Code ruft nur noch eine gemeinsame
Schnittstelle auf und kennt die einzelnen Arten nicht. Das Spielverhalten
bleibt dabei unverändert; das ist zugleich die wichtigste Prüfung.

Am Ende fügen die Studierenden eine neue Objektart hinzu und erleben, dass
dafür eine Tabellenzeile und eine Funktion genügen. Genau diese Fähigkeit
verlangt später Lernstandscheck II (Lichtschalter als neue Objektart).

## Seiten

| Seite | Inhalt | Ergebnis | Auswahlfragen |
|---|---|---|---|
| 1 Wo steckt der Schlüssel? | Stellen suchen; Gedankenexperiment; Vergleichslauf `ablauf.c` (54 Tasten, Statuszeile, Bild-Prüfsumme); `enum` mit Versuchen (Werte, `-Wswitch`, keine Typsicherheit); Aufgabe: Ergebnisse von `inventar_aufnehmen` als `enum` | Stellenliste, `vorher.txt`, Aufzählungstyp | `enum` |
| 2 Funktionen als Werte | Funktionspointer: Deklaration, `typedef`, Zuweisung, Aufruf, Tabelle, Fehlerfälle; Piktogramme als gemeinsamer Typ; Versuch Reihe zeichnen | Typ für Zeichenfunktionen | richtige Zeilen mit `Piktogramm` |
| 3 Objektarten mit Methoden | Art, Objekt, Instanz, Methode, `selbst`, `static const`, Kapselung; Beispiel Geräte; Aufteilung Art/Objekt im Spiel; Vertiefung Einbettung | Aufteilungstabelle, Methodenliste | Art und Objekt |
| 4 Gegenstände als Objekte | Aufgabe Umbau in sechs Schritten; Hinweise Reihenfolge, Schlüssel erkennen, Tresor/Beute, Besitz, Kopie, Fehlersuche mit `diff` | Spiel unverändert laut Vergleichslauf | Besitz und Kopie |
| 5 Eine neue Objektart | Aufgabe Geheimtür und zweite Münze (`OBJEKTE.md`); erwarteter `diff`; Prüftabelle; Ausblick | Erweiterung mit wenigen Stellen | neue Art vs. Instanz |

## Entwurfsentscheidungen

- **Gemeinsame Objektstruktur statt Einbettung als Pflicht.** In der
  Musterlösung hat ein Objekt Art, Position, `auf_feld`, `offen` und
  `inhalt` (Tresor). Das reicht für alle Arten im Spiel und kommt ohne
  Typumwandlung aus.
  Struktureinbettung mit Rückwandlung („Vererbung“ in C) wird als
  aufklappbare Vertiefung erklärt, mit dem Hinweis, wann die Umwandlung
  erlaubt ist.
- **Wächter bleiben außen vor.** Sie werden nicht benutzt, sondern wirken
  über ihre Reichweite aus Config. Sie als Objekte zu modellieren wäre
  eine mögliche Vertiefung, keine Pflicht.
- **`programm.c` und `tresorraub.h` bleiben unverändert.** Alles bleibt in
  `tresorraub.c`. Das Aufteilen in Module ist nicht Teil dieses Kapitels.
- **Konfiguration bleibt wie in Config.** `regeln.txt` und die Regeln im
  Spielstand werden nicht angefasst.
- **Methoden der Musterlösung:** `benutzen` (Leertaste, mit Abstand zur
  Figur; Ergebnis `ERLEDIGT`, `NICHTS_ZU_TUN` oder Fehler) und
  `betretbar`. Aussehen als Angaben (`zeichen`/`zeichen_offen`,
  `bild`/`bild_offen`).
- **Spielstand nicht mehr als Wert kopieren:** `startzustand` füllt über
  einen Pointer, weil Objekte und Inventar auf Objekte im Spielstand zeigen.
- **Vergleichslauf `ablauf.c`** im Startmaterial (eigenes `main`, Target
  `make ablauf`): Charakterisierungstest für den Umbau.
- **Arten sind konstant, Objekte veränderlich.** Methodentabellen als
  `static const`; ein Objekt verändert nur seine eigenen Daten und den
  übergebenen Spielstand.

## Abhängigkeiten und offene Punkte

- **Datei-Ein- und -Ausgabe (Kapitel 7):** Wird dort der Spielstand
  gespeichert, ändert sich nach dem Umbau seine Struktur. Gespeichert werden
  müssten Art (als Name) und Zustand je Objekt, weil sich Funktionspointer
  nicht in eine Datei schreiben lassen. Beim Anschluss des Datei-Kapitels
  klären, welches Kapitel das Speichern anpasst.
- **Startmaterial:** vorläufig der Endstand von Config; später der Endstand
  nach Datei-I/O und Config. `ablauf.c` drückt keine Taste zum Speichern;
  bei neuen Tasten im Datei-Kapitel die Folge prüfen.
- **Lernstandscheck II:** Eine neue Objektart (z. B. Lichtschalter) passt
  direkt auf diese Struktur; beim Planen von Kapitel 11 festlegen.
- **Umfang:** Seite 4 ist groß. Falls die Zeit knapp wird: Typen für Art
  und Objekt im Startmaterial vorgeben.
