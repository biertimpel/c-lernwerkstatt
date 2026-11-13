# Startmaterial: Lernstandscheck II – Licht aus

Dieses Material ist der Stand am Ende des Kapitels „Spielende“, Teil 2:
alle Module in einem Ordner. Es enthält:

- `spielfeld.c/.h`, `regeln.c/.h`, `spiel.h`, `inventar.c/.h`,
  `objekte.c/.h`, `spiellogik.c/.h`, `speicherstand.c/.h`, `tresorraub.c`:
  **Ihr Code.** Eine funktionierende Fassung des Spiels mit Sieg,
  Entdeckung, Zuglimit, Speichern und Laden.
- `programm.c`, `tresorraub.h`: das Programmgerüst, **neu** mit dem Aufruf
  `spiel_aktualisieren(jetzt_ms)` in jedem Durchlauf der Spielschleife.
  Diese Dateien müssen Sie nicht verändern.
- `piktogramme/`: wie bisher, **neu** ist `zeichne_lichtschalter`.
- `tests.c`: die Test-Suite aus dem Kapitel „Spielende“.
- `Makefile`, `regeln.txt` und die Beschreibungen `SPIELFELD.md`,
  `INVENTAR.md`, `OBJEKTE.md`, `ENDE.md`, `REGELN.md`, `FORMAT.md`.

Die Regeln für das Licht stehen auf der Seite des Lernstandschecks.

## Mit dem eigenen Stand weiterarbeiten

Sie können mit Ihren Modulen aus dem Kapitel „Spielende“ weiterarbeiten:

1. Legen Sie einen neuen Ordner an und kopieren Sie aus Ihrem Ordner
   `module/` alle `.c`- und `.h`-Dateien und `tests.c` hinein.
2. Kopieren Sie aus diesem Material `programm.c`, `tresorraub.h`, den
   Ordner `piktogramme/`, das `Makefile`, `regeln.txt` und die
   `.md`-Dateien dazu.
3. Ergänzen Sie in Ihrer `tresorraub.c` den neuen Anschluss:

   ```c
   int spiel_aktualisieren(Uint64 jetzt_ms) {
       (void)jetzt_ms;
       return 0;
   }
   ```

4. `make` und `make test` müssen danach ohne Fehler durchlaufen.

## Bauen und starten

```sh
make
./programm --text     # ruft text_ausgeben() und danach aufraeumen() auf
./programm            # Fenster; Tasten laut INVENTAR.md, Escape beendet
make test             # Test-Suite
```

Unter Windows (MSYS2) heißen die Programme `programm.exe` und `tests.exe`.
Starten Sie die Programme in diesem Ordner; dort liegen `regeln.txt` und
`spielstand.txt`. `spiel_aktualisieren` und `zeichne_wasted` brauchen SDL
2.0.18 oder neuer (`sdl2-config --version`).

Die Test-Suite gibt zwischendurch Fehlermeldungen aus, zum Beispiel
`unbekannte Regel "zuglimt"`. Sie gehören zu Tests, die prüfen, ob Fehler
richtig gemeldet werden. Entscheidend ist die letzte Zeile.

## Speicherfehler finden

```sh
make clean
make CFLAGS="-std=c17 -Wall -Wextra -Wpedantic -g -fsanitize=address"
make test
```

Unter macOS prüfen Sie vergessenen Speicher mit
`leaks --atExit -- ./tests`.
