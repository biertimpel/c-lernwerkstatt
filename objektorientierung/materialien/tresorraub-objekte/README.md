# Startmaterial: Objektorientierung in C

Dieses Material gehört zum Kapitel „Objektorientierung in C“. Es enthält:

- `tresorraub.c`: **Ihre Datei.** Sie enthält den Stand am Ende des Kapitels
  „Config“: Bewegung, Inventar als verkettete Liste im Spielstand,
  Aufnehmen und Tür mit der Leertaste, Ablegen mit L, Speichern mit F5,
  Laden mit F9 und Spielregeln aus `regeln.txt`.
- `ablauf.c`: ein Vergleichslauf. Er drückt eine feste Folge von Tasten und
  gibt nach jeder Taste die Statuszeile und eine Prüfsumme des Bildes aus.
  Diese Datei müssen Sie nicht verändern.
- `FORMAT.md`: Spielstandformat Version 2, auf das Sie auf Seite 4
  umstellen.
- `vorlagen/objekte.c`, `vorlagen/speichern.c`: Vorlagen zur direkten
  Übernahme auf Seite 4: Typen für Objekte und Spielstand, Speichern und
  Laden in Version 2. Wie Sie sie einfügen, steht am Anfang jeder Datei.
- `OBJEKTE.md`: Geheimtür und zweite Münze für die Aufgabe auf Seite 5.
- `regeln.txt`, `REGELN.md`: die Spielregeln wie im Kapitel „Config“.
- `programm.c`, `tresorraub.h`: dasselbe SDL2-Programmgerüst wie in den
  vorigen Kapiteln. Diese Dateien müssen Sie nicht verändern.
- `piktogramme/`, `SPIELFELD.md`, `INVENTAR.md`, `Makefile`: wie bisher.

Sie können auch mit Ihrer eigenen `tresorraub.c` aus dem vorigen Kapitel
weiterarbeiten. Kopieren Sie dann `ablauf.c`, `FORMAT.md`, `OBJEKTE.md`,
`INVENTAR.md`, den Ordner `vorlagen/` und das `Makefile` in Ihren Ordner. Die
Vorlagen erwarten die Namen aus dem Startmaterial, etwa `Regeln`,
`Waechter` und `punkt_gleich`.

## Bauen und starten

```sh
make
./programm --text     # ruft text_ausgeben() und danach aufraeumen() auf
./programm            # Fenster; Tasten laut INVENTAR.md, Escape beendet
./ablauf              # Vergleichslauf, siehe unten
```

## Vergleichslauf

Bevor Sie etwas umbauen, halten Sie das bisherige Verhalten fest:

```sh
make ablauf
./ablauf > vorher.txt
```

Nach jedem Umbauschritt vergleichen Sie:

```sh
make ablauf
./ablauf > nachher.txt
diff vorher.txt nachher.txt    # keine Ausgabe: Verhalten unverändert
```

Der Vergleichslauf drückt auch F5 und F9 und überschreibt dabei
`spielstand.txt` im aktuellen Ordner. Lassen Sie `regeln.txt` unverändert; andere Regeln ergeben eine
andere Ausgabe. Unter Windows (MSYS2) heißt das Programm `ablauf.exe`.
Wenn `diff` fehlt,
vergleichen Sie die beiden Dateien in Ihrem Editor.

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
