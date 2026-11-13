# Startmaterial: Dynamischer Speicher und Listen

Dieses Material gehört zum Kapitel „Dynamischer Speicher und Listen“. Es
enthält:

- `tresorraub.c`: **Ihre Datei.** Sie enthält den Stand am Ende des Kapitels
  „Pointer und Datenaustausch“: Die Figur bewegt sich mit den Pfeiltasten,
  Bild und Fenstertitel zeigen den laufenden Spielstand. Neu sind nur
  `INVENTAR_X` und `INVENTAR_Y` für die Anzeige des Inventars.
- `programm.c`, `tresorraub.h`: dasselbe SDL2-Programmgerüst wie in den
  vorigen Kapiteln. Diese Dateien müssen Sie nicht verändern.
- `piktogramme/`: die bekannten Piktogramme.
- `SPIELFELD.md`: Grundriss und Startpositionen.
- `INVENTAR.md`: Regeln für das Inventar, Aufnehmen, Tür und Ablegen.
- `Makefile`: übersetzt alles mit eingeschalteten Warnungen.

Für Listen und dynamischen Speicher gibt es keinen vorbereiteten Code.
Datendarstellung, Funktionen und Tests entwerfen Sie selbst.

Sie können auch mit Ihrer eigenen `tresorraub.c` aus dem Kapitel „Pointer“
weiterarbeiten. Ergänzen Sie dann die beiden Zeilen für `INVENTAR_X` und
`INVENTAR_Y` aus der mitgelieferten Datei.

## Bauen und starten

```sh
make
./programm --text     # ruft text_ausgeben() und danach aufraeumen() auf
./programm            # Fenster; Tasten laut INVENTAR.md, Escape beendet
./programm --hilfe    # zeigt alle Optionen
```

Unter Windows (MSYS2) heißt das Programm `programm.exe`.

## Ihr Spielzustand

Der laufende Spielstand liegt wie im Kapitel „Pointer“ als `static`-Variable
in `tresorraub.c`. Auf Seite 5 übernimmt er auch das Inventar. Ihre eigenen
Funktionen arbeiten nicht direkt mit dieser Variablen, sondern mit dem
Pointer, den sie als Parameter bekommen.

## Speicherfehler finden

Fehler beim Umgang mit dynamischem Speicher fallen oft nicht sofort auf. Lassen
Sie Ihr Programm deshalb regelmäßig prüfen.

**Linux und Windows mit WSL:** AddressSanitizer meldet Zugriffe auf
freigegebenen Speicher, doppeltes Freigeben und am Programmende nicht
freigegebenen Speicher:

```sh
make clean
make CFLAGS="-std=c17 -Wall -Wextra -Wpedantic -g -fsanitize=address"
./programm --text
```

**macOS:** Der AddressSanitizer findet dort Zugriffe auf freigegebenen
Speicher und doppeltes Freigeben, aber keinen vergessenen Speicher. Dafür
gibt es das Werkzeug `leaks`:

```sh
make
leaks --atExit -- ./programm --text
```

**Windows mit MSYS2:** Diese Werkzeuge stehen dort nicht zur Verfügung. Nutzen
Sie einen Kursrechner oder WSL.

## Beachten

Lesen und beheben Sie Compilerwarnungen, bevor Sie weiterarbeiten. Auf Seite 1
entsteht absichtlich eine Warnung zu einer Funktion, die die Adresse einer
lokalen Variablen zurückgibt. Auf Seite 2 bauen Sie absichtlich Speicherfehler
ein. Entfernen Sie diese Versuche wieder, wenn Sie sie ausgewertet haben.

`zeichne` wird etwa 60-mal pro Sekunde aufgerufen. Schreiben Sie dort keine
Ausgaben mit `printf`. Meldungen gehören in `taste_gedrueckt` oder
`text_ausgeben`.
