# Musterlösung „Datei-Ein- und -Ausgabe“ – nur für Lehrende

Eine mögliche Lösung der Aufgaben auf den Seiten 3, 5 und 6. Grundlage ist der
Stand am Ende des Kapitels „Dynamischer Speicher und Listen“ (Startmaterial).
Andere Funktionszuschnitte und Rückgabekonventionen sind ebenso vertretbar,
solange Format und Verhalten aus `FORMAT.md` eingehalten werden. Die
Entwurfsentscheidungen stehen am Anfang von `tresorraub.c`.

Dieser Ordner wird nicht in das Startmaterial gepackt und auf keiner
Kursseite verlinkt. Er ist aber im öffentlichen Repository und über GitHub
Pages abrufbar.

## Inhalt

- `tresorraub.c`: Lösung, ersetzt `tresorraub.c` aus dem Startmaterial. Die
  Versuche der Seiten 1, 2 und 4 sind nicht enthalten.
- `tests.c`: 89 Prüfungen: alle Beispieldateien, Rundlauf Speichern–Laden,
  Grenzen, unbekannte und doppelte Namen, Widersprüche, Listenreihenfolge,
  eigene Adressen nach dem Laden, Freigaben, fehlschlagendes `malloc` (vor
  und nach einem neuen Eintrag), fehlschlagendes Öffnen und Schließen,
  `/dev/full` unter Linux, F5/F9 über `taste_gedrueckt`, Statuszeile und
  Zeichnen mit dem Software-Renderer. Zählt eigene `malloc`/`free`- und
  `fopen`/`fclose`-Aufrufe.
- `Makefile`: verwendet Gerüst und Piktogramme aus
  `../materialien/tresorraub-dateien`.

## Bauen und prüfen

```sh
make
make test
./programm --text
./programm            # F5 speichert, F9 lädt spielstand.txt
```

Mit Speicherprüfung (Linux):

```sh
make clean
make CFLAGS="-std=c17 -Wall -Wextra -Wpedantic -Werror -g -fsanitize=address,undefined"
make test
```

Geprüft mit SDL 2.30 unter Linux mit GCC und Clang, übersetzt mit
`-std=c17 -Wall -Wextra -Wpedantic -Werror`, mit GCC zusätzlich mit
`-fsanitize=address,undefined` (einschließlich LeakSanitizer).

## Erwartungshorizont für Fragen ohne Code

**Seite 1**

- `FILE *` zeigt auf ein Verwaltungsobjekt der Bibliothek, nicht auf den
  Dateiinhalt.
- `"w"`: nach zwei Läufen nur `Messwert: 9`; `"a"`: danach dreimal
  `Messwert: 9`. `fehlt/notiz.txt`: `perror` meldet „No such file or
  directory“ auf `stderr`, auch bei umgeleiteter Ausgabe im Terminal sichtbar.
- Rückgaben: `fopen` auf `NULL`, `fprintf` auf negativ, `fclose` auf ungleich
  0; nach einem Schreibfehler trotzdem genau einmal `fclose`.

**Seite 2**

- Veränderlich: Figur, Züge, Tür, Lage und Zustand der Gegenstände,
  Inventar in Reihenfolge. Fest: Grundriss, Ausgang, Tür- und
  Tresorposition, Wächter. Keine Adressen speichern.
- Text statt binär: lesbar, keine Pointer, unabhängig vom Speicherlayout.

**Seite 3**

- Vertrag z. B. `int spielstand_speichern(const Spielstand *, const char *)`,
  0 Erfolg, negativ Fehler; Erfolg erst nach erfolgreichem `fclose`.
- Startzustand gespeichert ist identisch mit `beispiele/start.txt`.
- F5 verbraucht keinen Zug und liefert auch bei einem Dateifehler 0 an das
  Gerüst.

**Seite 4**

- `feof`-Falle: vier Durchläufe, Summe 42 statt 27; richtig ist
  `while (fscanf(...) == 1)`.
- MC: abgelehnt werden `12abc`, `FIGUR` und die überlange Zahl.
- Versuch mit dem Dateikopf: Meldungen laut Tabelle; Datei wird in jedem
  Fall genau einmal geschlossen.

**Seite 5**

- Eigener Spielstand für den Ladeversuch, eigene Einträge; bei Fehler nur
  diese freigeben; bei Erfolg altes Inventar freigeben und Besitz übernehmen.
- `inventar.txt`: (5, 3), 23 Züge, Tür offen, Beute vor Muenze.

**Seite 6**

- `start.txt`, `inventar.txt`, `abgelegt.txt` werden angenommen, alle
  `ungueltig-*` und die beiden eigenen Kopien (Münze auf dem Ausgang, zwei
  Gegenstände auf einem Feld) abgelehnt.
- `abgelegt.txt`: (5, 3), 24 Züge, Tür offen, Inventar leer, Beute (5, 2),
  Münze (5, 4).
