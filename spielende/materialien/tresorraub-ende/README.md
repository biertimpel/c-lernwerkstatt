# Startmaterial: Spielende

Dieses Material gehört zum Kapitel „Spielende“. Es enthält:

- `tresorraub.c`: **Ihre Datei für Teil 1.** Sie enthält den Stand am Ende
  des Kapitels „Objektorientierung in C“: Objektarten mit Methoden,
  Geheimtür und zweite Münze, Inventar, Speichern (F5) und Laden (F9) im
  Format Version 2, Spielregeln aus `regeln.txt`.
- `ENDE.md`: wie ein Spiel endet. Das bauen Sie in Teil 1 ein.
- `piktogramme/`: wie bisher, neu ist `zeichne_wasted` für den Stempel
  WASTED. Er braucht SDL 2.0.18 oder neuer (`sdl2-config --version`).
- `module/`: **Ihr Ordner für Teil 2.** Er enthält die Header der Module,
  die Test-Suite `tests.c` und ein eigenes `Makefile`. Diese Dateien müssen
  Sie nicht verändern. Die Quelldateien der Module schreiben Sie selbst.
- `ablauf.c`: der Vergleichslauf aus dem Kapitel „Objektorientierung in C“.
- `programm.c`, `tresorraub.h`: dasselbe SDL2-Programmgerüst wie in den
  vorigen Kapiteln. Diese Dateien müssen Sie nicht verändern.
- `FORMAT.md`, `INVENTAR.md`, `OBJEKTE.md`, `REGELN.md`, `SPIELFELD.md`,
  `regeln.txt`, `Makefile`: wie bisher, ergänzt um das Spielende.

Sie können auch mit Ihrer eigenen `tresorraub.c` aus dem vorigen Kapitel
weiterarbeiten. Kopieren Sie dann `ENDE.md`, den Ordner `module/` und den
Ordner `piktogramme/` in Ihren Ordner. Die Header in `module/` erwarten die
Namen aus dem Startmaterial, etwa `Objektart`, `Spielstand`, `Regeln` und
`inventar_aufnehmen`. Weichen Ihre Namen ab, passen Sie in Teil 2 Ihren
Code an die Header an, nicht umgekehrt.

## Teil 1: eine Datei

```sh
make
./programm --text     # ruft text_ausgeben() und danach aufraeumen() auf
./programm            # Fenster; Tasten laut INVENTAR.md, Escape beendet
make ablauf
./ablauf > vorher.txt # Vergleichslauf vor Ihren Änderungen
```

Unter Windows (MSYS2) heißen die Programme `programm.exe` und `ablauf.exe`.
Starten Sie die Programme in diesem Ordner; dort liegen `regeln.txt` und
`spielstand.txt`.

## Teil 2: Module

Alle Befehle stehen hier so, wie Sie sie in **diesem** Ordner eingeben.
`make -C module` arbeitet mit dem `Makefile` im Ordner `module/`.

```sh
make ablauf && ./ablauf > vorher.txt   # einmal, am Ende von Teil 1
make -C module                         # Programm, Vergleichslauf, Tests
make -C module test                    # Test-Suite starten
./module/programm                      # das Spiel aus den Modulen
./module/ablauf > nachher.txt
diff vorher.txt nachher.txt            # keine Ausgabe: Verhalten unverändert
```

Die Test-Suite gibt zwischendurch Fehlermeldungen aus, zum Beispiel
`unbekannte Regel "zuglimt"`. Sie gehören zu Tests, die prüfen, ob Ihr
Programm Fehler richtig meldet. Entscheidend ist die letzte Zeile, etwa
`79 Pruefungen, 0 fehlgeschlagen.` Jeder fehlgeschlagene Test nennt eine
Zeile in `module/tests.c`; dort steht, was erwartet wird.

## Speicherfehler finden

Wie im Kapitel „Dynamischer Speicher und Listen“:

```sh
make -C module clean
make -C module CFLAGS="-std=c17 -Wall -Wextra -Wpedantic -g -fsanitize=address"
make -C module test
```

Unter macOS prüfen Sie vergessenen Speicher mit
`leaks --atExit -- ./module/tests`.
