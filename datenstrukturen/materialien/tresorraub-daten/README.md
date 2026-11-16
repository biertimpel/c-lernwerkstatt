# Startmaterial: Datenstrukturen

Dieses Material gehört zum Kapitel „Datenstrukturen“. Es enthält:

- `tresorraub.c`: **Ihre Datei.** Hier entsteht Ihr Code für alle Seiten des
  Kapitels. In diesem Kapitel schreiben Sie zwei Funktionen, die das
  Programmgerüst aufruft:
  - `text_ausgeben`: für Ihre Versuche und Textausgaben
  - `zeichne`: für die Darstellung des Spielfelds im Fenster

  Am Ende der Datei stehen vier weitere Funktionen, die das Gerüst aufruft.
  Sie brauchen sie erst ab dem Kapitel „Pointer und Datenaustausch“. Lassen
  Sie diese Platzhalter bis dahin unverändert.
- `programm.c`, `tresorraub.h`: das SDL2-Programmgerüst, aufgebaut wie im
  Kapitel „Manpages“. Dasselbe Gerüst verwenden Sie in den folgenden
  Kapiteln weiter. Diese Dateien müssen Sie nicht verändern.
- `piktogramme/`: die Piktogramme aus dem Kapitel „Manpages“, zum Beispiel
  für Wand, Boden, Spielfigur und Wächter. Die Vorschau öffnen Sie mit
  `piktogramme/index.html` im Browser.
- `SPIELFELD.md`: Grundriss, Positionen und Regeln des Tresorraubs.
- `Makefile`: übersetzt alles mit eingeschalteten Warnungen.

Für Arrays, Zeichenketten und Strukturen gibt es keinen vorbereiteten Code.
Datendarstellung, Funktionsnamen und Tests entwerfen Sie selbst.

## Bauen und starten

Voraussetzung ist die SDL2-Einrichtung aus dem Kapitel „Manpages“. Im
entpackten Ordner:

```sh
make
./programm --text     # ruft text_ausgeben() auf, ohne Fenster
./programm            # öffnet das Fenster und ruft zeichne() für jedes Bild auf
./programm --galerie  # zeigt alle Piktogramme
./programm --hilfe    # zeigt alle Optionen
```

Unter Windows (MSYS2) heißt das Programm `programm.exe`.

Das unveränderte Material zeigt im Fenster eine Spielfigur als Beispiel. Mit
Escape oder dem Schließsymbol beenden Sie das Programm.

## Das Spielfeld zeichnen

Ein Piktogramm besteht aus 16 × 16 Rasterzellen. In `tresorraub.c` ist
`SKALIERUNG` auf 2 gesetzt, ein Spielfeld ist damit 32 × 32 Pixel groß. Das
Feld (x, y) beginnt bei den Pixelkoordinaten `x * FELDGROESSE` und
`y * FELDGROESSE`. Das ganze Spielfeld passt mit 480 × 288 Pixeln in das
Fenster mit 640 × 480 Pixeln.

Jede Zeichenfunktion der Piktogramme liefert 0 bei Erfolg und einen negativen
Wert bei einem Fehler. Geben Sie einen Fehler aus `zeichne` sofort zurück,
wie im Kapitel „Manpages“.

## Beachten

Lesen und beheben Sie Compilerwarnungen, bevor Sie weiterarbeiten. Drei
Meldungen entstehen absichtlich bei Versuchen auf den Kapitelseiten:

- Seite 1: eine Warnung zu einem Array ohne Initialisierung (nur bei GCC)
- Seite 2: eine Warnung zum Vergleich von Zeichenketten mit `==`
- Seite 4: ein Fehler beim Vergleich zweier Strukturen mit `==`

Entfernen Sie diese Versuche wieder, wenn Sie sie ausgewertet haben.

`zeichne` wird etwa 60-mal pro Sekunde aufgerufen. Schreiben Sie dort keine
Ausgaben mit `printf`, sonst läuft das Terminal voll. Textausgaben gehören in
`text_ausgeben`.

Mit GCC oder Clang können Sie zusätzlich einen Prüfmodus verwenden, der viele
Zugriffe außerhalb eines Arrays während der Ausführung meldet:

```sh
make clean
make CFLAGS="-std=c17 -Wall -Wextra -Wpedantic -g -fsanitize=address"
```

Dieser Modus steht nicht auf allen Systemen zur Verfügung und ersetzt keine
eigene Prüfung der Grenzen.
