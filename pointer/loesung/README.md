# Musterlösung „Pointer und Datenaustausch“ – nur für Lehrende

Eine mögliche Lösung aller Aufgaben der Seiten 1 bis 4. Andere
Funktionszuschnitte sind ebenso vertretbar. Die Entwurfsentscheidungen
stehen am Anfang von `tresorraub.c`.

Dieser Ordner wird nicht in das Startmaterial gepackt und auf keiner
Kursseite verlinkt.

## Inhalt

- `tresorraub.c`: Lösung, ersetzt `tresorraub.c` aus dem Startmaterial
- `tests.c`: prüft alle erwarteten Werte aus den Selbstkontrollen
- `Makefile`: verwendet Gerüst und Piktogramme aus
  `../materialien/tresorraub-pointer-start`, es gibt also nur eine Fassung
  davon

## Bauen und prüfen

```sh
make
./tests            # alle erwarteten Werte der Selbstkontrollen
./programm --text  # Textausgaben der Seiten 1, 2 und 3
./programm         # Spiel im Fenster (Seite 4), Escape beendet es
```

Geprüft mit SDL 2.30 unter Linux, übersetzt mit GCC 13 und Clang 18 mit
`-std=c17 -Wall -Wextra -Wpedantic -Werror` sowie mit
`-fsanitize=address,undefined`. Die Tests brauchen kein Fenster; `zeichne`
wird mit einem Software-Renderer geprüft.

## Zuordnung zu den Aufgaben

| Seite | Aufgabe | In der Lösung |
|---|---|---|
| 1 | Vorhersagen und ausprobieren, Rückblick | `text_ausgeben`, Abschnitte „Seite 1“ |
| 2 | Den Spielstand des Aufrufers verändern | `zug_zaehlen_original`, Abschnitt „Seite 2“ |
| 3 | Einen Schritt der Figur ausführen | `figur_bewegen`, `spielstand_lesen` mit `const`, Abschnitt „Seite 3“ |
| 4 | Ein Spielstand für das ganze Programm | `static Spielstand spielstand;` |
| 4 | Tastatur, Bild und Statuszeile verbinden | `spiel_starten`, `taste_gedrueckt`, `spiel_status`, `zeichne`; `feldzeichen`, `spielfeld_ausgeben`, `feld_zeichnen` mit `const Spielstand *` |

Bewusst nicht im Programm enthalten sind Versuche, die nur eine Warnung oder
einen Übersetzungsfehler zeigen sollen: `sizeof` auf einem Array-Parameter
und der Vergleich von Zeichen-Arrays mit `==` (Seite 1), eine Zuweisung
über einen `const`-Pointer (Seite 3) und ein Funktionsaufruf als
Anfangswert der `static`-Variablen (Seite 4). Die erwarteten Meldungen
stehen unten.

## Erwartungshorizont für Fragen ohne Code

**Seite 1**

- `var` und `*p` sind 10, `p` und `&var` dieselbe Adresse. Nach `*p = 20;`
  ist `var` 20.
- Vertiefung: `pp` zeigt direkt auf `p`; `*pp` ist `p`, `**pp` ist `var`,
  also 20.
- `sizeof`: beim Aufrufer 20 (5 × 4 Bytes), in der Funktion 8 (Größe eines
  Pointers). Mit zehn Elementen 40 und weiterhin 8. GCC und Clang warnen mit
  `-Wsizeof-array-argument` („will return size of 'int *'“). Folgerung: Die
  Länge als eigenen Parameter beim Aufrufer berechnen und mitgeben.
- `a == b`: immer 0, weil zwei verschiedene Adressen verglichen werden;
  `%p` zeigt zwei verschiedene Adressen. GCC warnt mit `-Warray-compare`
  („comparison between two arrays“), Clang mit `-Wtautological-compare`
  („array comparison always evaluates to false“). `strcmp(a, b)` ist 0.
  Mit `"Muenze"` bleibt `a == b` 0, `strcmp` liefert einen negativen Wert,
  weil `'B'` vor `'M'` kommt. Der genaue Wert ist nicht festgelegt.

**Seite 2**

- Nach `zug_zaehlen(s)` hat `s` weiter 0 Züge (die Funktion gibt intern 1
  aus), nach `zug_zaehlen_original(&s)` 1.
- `p->zuege` entspricht `(*p).zuege`. Ohne Klammern bindet `.` stärker:
  `*p.zuege` ist ein Übersetzungsfehler.
- Erklärung mit der Speicheransicht: Beim ersten Aufruf entsteht eine Kopie
  aller Felder, die nach dem `return` verschwindet. Beim zweiten Aufruf
  wird nur die Adresse kopiert; der Pointer zeigt auf `s` beim Aufrufer.

**Seite 3**

- Erwartete Werte der Testfolge: siehe Ausgabe von `./programm --text` und
  `tests.c`. Tür verschlossen: `false`, Figur bleibt auf (5, 3). Tür offen:
  `true`, (6, 3), Zugzahl + 1. `(2, 0)`, `(1, 1)` und `NULL`: `false`, nichts
  verändert.
- Zuweisung über `const Spielstand *p` (`p->zuege = 5;`): GCC „assignment of
  member 'zuege' in read-only object“, Clang „cannot assign to variable 'p'
  with const-qualified type“.
- Reihenfolge der Prüfungen: `NULL` vor dem ersten Zugriff, Grenzen vor dem
  Arrayzugriff (`begehbar` prüft zuerst `innerhalb`). Position und Zugzahl
  erst ändern, wenn alle Prüfungen bestanden sind.
- Vertretbare Varianten: Die Zugzahl direkt erhöhen statt über
  `zug_zaehlen_original`; die Gültigkeit des Schritts über
  `abs(dx) + abs(dy) == 1` prüfen.

**Seite 4**

- Zähler-Experiment: mit globaler Variable „Aufruf Nummer 1, 2, 3“; als
  lokale Variable mit `= 0` dreimal „Aufruf Nummer 1“, weil sie bei jedem
  Aufruf neu angelegt wird.
- `static Spielstand spielstand = startzustand();`: GCC „initializer element
  is not constant“, Clang „initializer element is not a compile-time
  constant“.
- Auswahlfrage zu `static`: richtig sind „existiert vom Programmstart bis
  zum Programmende“ und „Code in `programm.c` kann die Variable nicht über
  ihren Namen verwenden“. Vor `spiel_starten` steht die Figur auf (0, 0)
  mit 0 Zügen (Nullinitialisierung), siehe ersten Test in `tests.c`.
- Auswahlfrage zu `zeichne`: richtig sind „Jedes Bild würde den
  Ausgangszustand zeigen“ und „mit derselben Variablen arbeiten“.
- Beobachtungen im Fenster: Die Figur bleibt auf dem neuen Feld, Bild und
  Titel zeigen dieselbe Position und Zugzahl, gegen Wand und verschlossene
  Tür ändert sich nichts. Typischer Fehler: Die Figur springt zurück, weil
  `zeichne` weiter einen eigenen Startzustand anlegt.
- `figur_bewegen` muss oberhalb von `text_ausgeben` und `taste_gedrueckt`
  stehen; sonst „implicit declaration of function“ (GCC 13 Warnung, ab GCC
  14 Fehler) bzw. „call to undeclared function“ (Clang, Fehler).
- Das Zuglimit wird noch nicht ausgewertet; nach 60 Zügen zeigt der Titel
  zum Beispiel `61/60`. Das ist so gewollt und in den Tests festgehalten.
