# Musterlösung „Spielende“ – nur für Lehrende

Eine mögliche Lösung der Aufgaben auf den Seiten 1 bis 3. Grundlage ist der
Stand am Ende des Kapitels „Objektorientierung in C“ (Startmaterial).

Dieser Ordner wird nicht in das Startmaterial gepackt und auf keiner
Kursseite verlinkt. Er ist aber im öffentlichen Repository und über GitHub
Pages abrufbar.

## Inhalt

- `teil1/tresorraub.c`: Teil 1 in einer Datei (Seiten 1 und 2). Die
  Entwurfsentscheidungen stehen am Anfang der Datei.
- `module/`: Teil 2 (Seite 3), sechs Module und `tresorraub.c` passend zu
  den Headern in `../materialien/tresorraub-ende/module/`. Erzeugt aus
  Teil 1 ohne Änderung des Verhaltens; einzige Umbenennung:
  `zug_zaehlen_original` heißt `zug_zaehlen`, neu ist `ist_wand`.
- `Makefile`: baut beide Teile mit Gerüst, Piktogrammen, `ablauf.c`, Headern
  und `tests.c` aus dem Startmaterial.

## Bauen und prüfen

```sh
make            # programm-teil1, ablauf-teil1, programm, ablauf, tests
make test       # Test-Suite gegen die Module: 79 Pruefungen, 0 fehlgeschlagen
make vergleich  # Vergleichslauf Teil 1 gegen Module: gleich
```

Geprüft mit GCC (`-Wall -Wextra -Wpedantic -Werror`,
`-fsanitize=address,undefined`) und Clang (`-Werror`).

## Die Test-Suite

`materialien/tresorraub-ende/module/tests.c` ist für die Studierenden
bestimmt und prüft jedes Modul über seinen Header:

- `spielfeld`: Abstände, Grenzen, Wände, Überwachung einschließlich des
  Wächterfelds.
- `regeln`: Standardwerte, fehlende Datei, gültige Datei mit Kommentar und
  Leerzeile, Tippfehler im Namen (Regeln unverändert).
- `inventar`: Aufnehmen vorne, volles Inventar, Finden nach Art, Entfernen,
  Leeren (zweimal).
- `objekte`: Startzustand, Objekt- und Gegenstandssuche, Wächter,
  Reichweite aus den Regeln.
- `spiellogik`: Wand, Diagonale, Aufnehmen, Statuszeile, Ablegen, Tür zu,
  zu kleiner Puffer; Spielende: entdeckt (Statuszeile, keine Bewegung
  danach), Zuglimit durch Schritt und durch Aufnehmen, Ausgang ohne Beute,
  Sieg im 60. Zug, keine Aktion nach dem Ende, Speichern verweigert.
- `speicherstand`: Rundlauf mit Inventar; abgelehnt werden überwachte
  Figur, erreichtes Zuglimit, Format Version 1, fehlende Datei, jeweils ohne
  den Spielstand zu verändern.
- `tresorraub.c`: Start, Tasten bis zur Entdeckung, Tasten nach dem Ende,
  N, `zeichne` in einen Software-Renderer, `text_ausgeben`, `aufraeumen`.

Vorhandene `regeln.txt` und `spielstand.txt` werden gesichert und
wiederhergestellt. Stichprobe mit Mutationen der Musterlösung (Reichweite
`<` statt `<=`, Schutz nach dem Spielende entfernt, Zuglimit `>` statt
`>=`, Ablehnung beim Laden entfernt): Jede wird von mindestens einem Test
erkannt.

## Erwartungshorizont

### Seite 1: Spielende

- Ein Aufzählungstyp mit vier Zuständen und ein Feld im Spielstand; keine
  Kombination mehrerer `bool`. Der Startzustand setzt `LAEUFT`.
- Sieg als Methode des Ausgangs (`betreten` o. ä.), aufgerufen von der
  Bewegung für Objekte auf dem Zielfeld, **bevor** der Zug gezählt wird.
  Akzeptabel ist auch eine andere Methode, solange die Bewegung keinen
  Namensvergleich mit dem Ausgang enthält. Eine Lösung, die in der Bewegung
  den Ausgang abfragt, erfüllt die Regeln, verfehlt aber das Ziel aus
  „Objektorientierung in C“; im Gespräch darauf hinweisen.
- Verlieren zentral nach jedem gezählten Zug, Entdeckung vor Zuglimit; ein
  beendetes Spiel wird nicht mehr geprüft. Häufiger Fehler: Prüfung nur nach
  Pfeiltasten, dann endet das Spiel nicht, wenn der 60. Zug ein Aufnehmen
  ist.
- Nach dem Ende ändern Bewegen, Leertaste, Ablegen nichts. Der Schutz nur in
  `taste_gedrueckt` erfüllt Seite 1, fällt aber in Teil 2 durch die Tests
  (`figur_bewegen` und `handeln` nach dem Ende).
- N gibt die Liste frei, bevor der Startzustand geschrieben wird
  (prüfbar mit ASan).
- Laden lehnt Spielstände ab, in denen das Spiel zu Ende wäre; allgemein
  über „Objekt mit Methode `betreten`“ ist eleganter als über den Namen des
  Ausgangs, beides ist richtig.
- Gewinnweg der Musterlösung (35 Züge, Leertaste zählt):
  hoch, hoch, Leertaste, rechts, hoch, hoch, rechts, Leertaste, rechts,
  rechts, hoch, hoch, sechsmal rechts, Leertaste, sechsmal links, runter,
  runter, dreimal links, hoch, hoch, dreimal links. Mit `zuglimit = 35`
  gewonnen, mit `zuglimit = 34` Zeit abgelaufen auf (2, 1).
- Vergleichslauf: ab Zug 22 (Feld (9, 3)) `Entdeckt!`, danach
  `Das Spiel ist vorbei.`; das zweite F5 speichert nicht, das letzte F9 lädt
  den Stand aus Zug 11.

### Seite 2: WASTED

- `zeichne_wasted` als Letztes in `zeichne`, mit dem Rechteck des
  Spielfelds; entdeckte Figur. Fehler werden weitergegeben.
- Sieg: frei gestaltet, deutlich verschieden von laufendem Spiel und
  Niederlage. Musterlösung: grüner Rahmen um das Spielfeld.
- Auswahlfrage: W hat 18 Kästchen.

### Seite 3: Module

- Header unverändert; sechs Quelldateien plus `tresorraub.c`. Alles, was
  nicht im Header steht, ist `static`: Grundriss, Regelangaben, Objektarten
  mit Methoden, Hilfsfunktionen, `Gelesen`, der laufende Spielstand.
- `make test`: 0 fehlgeschlagen. Vergleichslauf gleich mit dem Stand nach
  Seite 2.
- Typische Abweichungen der Studierenden, weil Namen außerhalb der
  OOP-Vorlagen frei waren: `figur_bewegen`, `handeln`, `ablegen`,
  `inventar_finden`, `inventar_entfernen`, `spielstand_lesen`,
  `feldzeichen`; Rückgabewerte von `ablegen` (Anzahl) und
  `spielstand_lesen` (−1 bei zu kleinem Puffer). Die Seite verlangt, den
  eigenen Code an die Header anzupassen.
- Zeilen der Musterlösung: spielfeld 49, inventar 81, regeln 135,
  tresorraub 184, objekte 199, spiellogik 219, speicherstand 288.

### Seite 4: Rückblick

Keine richtige oder falsche Antwort. Erwartet ist ein schriftlicher
Rückblick von etwa einer Seite, der konkret auf die eigenen Notizen aus
„Vom Problem zum Programm II“ Bezug nimmt (Anforderungen, Programmplan,
Kapitelzuordnung) und mindestens zwei Abweichungen zwischen Zielbild und
Spiel begründet. Gute Rückblicke benennen eine konkrete Gewohnheit für
Block 3.
