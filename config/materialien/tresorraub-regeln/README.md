# Startmaterial: Config

Dieses Material gehört zum Kapitel „Config“. Es enthält:

- `tresorraub.c`: **Ihre Datei.** Sie enthält den Stand am Ende des Kapitels
  „Dynamischer Speicher und Listen“: Bewegung, Inventar als verkettete Liste
  im Spielstand, Aufnehmen und Tür mit der Leertaste, Ablegen mit L.
- `regeln.txt`: die Spielregeln mit ihren Standardwerten. Diese Datei liest
  Ihr Programm in diesem Kapitel ein.
- `REGELN.md`: Bedeutung der Regeln, Format von `regeln.txt` und erwartetes
  Verhalten bei fehlender oder fehlerhafter Datei.
- `wecker.txt`: eine kleine Beispieldatei für die Versuche auf den Seiten 1
  und 2.
- `programm.c`, `tresorraub.h`: dasselbe SDL2-Programmgerüst wie in den
  vorigen Kapiteln. Diese Dateien müssen Sie nicht verändern.
- `piktogramme/`, `SPIELFELD.md`, `INVENTAR.md`, `Makefile`: wie bisher.

Sie können auch mit Ihrer eigenen `tresorraub.c` aus dem vorigen Kapitel
weiterarbeiten. Kopieren Sie dann `regeln.txt`, `REGELN.md` und `wecker.txt`
in Ihren Ordner.

## Bauen und starten

```sh
make
./programm --text     # ruft text_ausgeben() und danach aufraeumen() auf
./programm            # Fenster; Tasten laut INVENTAR.md, Escape beendet
```

Starten Sie das Programm in dem Ordner, in dem `regeln.txt` liegt. Das
Programm sucht die Datei dort, wo es gestartet wird.

Unter Windows (MSYS2) heißt das Programm `programm.exe`.

## Speicherfehler finden

Wie im Kapitel „Dynamischer Speicher und Listen“:

```sh
make clean
make CFLAGS="-std=c17 -Wall -Wextra -Wpedantic -g -fsanitize=address"
./programm --text
```

Unter macOS prüfen Sie vergessenen Speicher mit
`leaks --atExit -- ./programm --text`.
