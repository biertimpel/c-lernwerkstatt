# Startmaterial: Datei-Ein- und -Ausgabe

Dieses Material gehört zum Kapitel „Datei-Ein- und -Ausgabe“. Es enthält:

- `tresorraub.c`: **Ihre Datei.** Sie enthält den Stand am Ende des Kapitels
  „Dynamischer Speicher und Listen“: Bewegung, Inventar als verkettete Liste
  im Spielstand, Aufnehmen und Tür mit der Leertaste, Ablegen mit L.
- `FORMAT.md`: das gemeinsame Spielstandformat und seine Prüfregeln. Lesen
  Sie es erst, nachdem Sie auf Seite 2 Ihr eigenes Format entworfen haben.
- `beispiele/`: gültige und ungültige Spielstände für die Seiten 3 bis 6.
  Verändern Sie diese Dateien nicht; arbeiten Sie mit Kopien.
- `programm.c`, `tresorraub.h`: dasselbe SDL2-Programmgerüst wie in den
  vorigen Kapiteln. Diese Dateien müssen Sie nicht verändern.
- `piktogramme/`, `SPIELFELD.md`, `INVENTAR.md`, `Makefile`: wie bisher.

Sie können auch mit Ihrer eigenen `tresorraub.c` aus dem vorigen Kapitel
weiterarbeiten. Kopieren Sie dann `FORMAT.md` und den Ordner `beispiele/` in
Ihren Ordner.

## Bauen und starten

```sh
make
./programm --text     # ruft text_ausgeben() und danach aufraeumen() auf
./programm            # Fenster; Tasten laut INVENTAR.md, Escape beendet
```

Unter Windows (MSYS2) heißt das Programm `programm.exe`.

In den Aufgaben ergänzen Sie F5 zum Speichern und F9 zum Laden von
`spielstand.txt`. Beide verbrauchen keinen Zug.

Relative Dateinamen wie `spielstand.txt` beziehen sich auf den Ordner, in dem
Sie das Programm starten, nicht auf den Ordner der Quelldatei. Starten Sie
das Programm deshalb im Ordner des Startmaterials.

## Speicherfehler finden

Wie im Kapitel „Dynamischer Speicher und Listen“:

```sh
make clean
make CFLAGS="-std=c17 -Wall -Wextra -Wpedantic -g -fsanitize=address"
./programm --text
```

Unter macOS prüfen Sie vergessenen Speicher mit
`leaks --atExit -- ./programm --text`.
