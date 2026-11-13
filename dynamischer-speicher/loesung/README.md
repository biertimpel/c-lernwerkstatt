# Musterlösung „Dynamischer Speicher und Listen“ – nur für Lehrende

Eine mögliche Lösung aller Aufgaben der Seiten 2 bis 6. Grundlage ist der
Stand am Ende des Kapitels „Pointer“ (Startmaterial). Andere
Funktionszuschnitte, Rückgabekonventionen und Darstellungen des Listenkopfs
sind ebenso vertretbar. Die Entwurfsentscheidungen stehen am Anfang von
`tresorraub.c`.

Dieser Ordner wird nicht in das Startmaterial gepackt und auf keiner
Kursseite verlinkt. Er ist aber im öffentlichen Repository und über GitHub
Pages abrufbar.

## Inhalt

- `tresorraub.c`: Lösung, ersetzt `tresorraub.c` aus dem Startmaterial
- `tests.c`: prüft alle erwarteten Ergebnisse aus den Selbstkontrollen.
  Ersetzt `malloc` durch eine Testfassung, die auf Wunsch `NULL` liefert,
  und prüft damit die Fehlerpfade beim Anlegen, Aufnehmen und bei der
  Leertaste. Spielt Laufen, Aufnehmen, Tür und Ablegen über
  `taste_gedrueckt` nach und prüft `zeichne` mit einem Software-Renderer.
- `Makefile`: verwendet Gerüst und Piktogramme aus
  `../materialien/tresorraub-inventar`

## Bauen und prüfen

```sh
make
./tests            # alle erwarteten Ergebnisse, auch bei fehlschlagendem malloc
./programm --text  # Textausgaben der Seiten 2 bis 6
./programm         # Spiel im Fenster, Tasten laut INVENTAR.md
```

Mit Speicherprüfung (Linux):

```sh
make clean
make CFLAGS="-std=c17 -Wall -Wextra -Wpedantic -g -fsanitize=address,undefined"
./tests && ./programm --text
```

Geprüft mit SDL 2.30 unter Linux mit GCC und Clang, übersetzt mit
`-std=c17 -Wall -Wextra -Wpedantic -Werror` sowie mit
`-fsanitize=address,undefined` (einschließlich LeakSanitizer).

## Erwartungshorizont für Fragen ohne Code

**Seite 1**

- Speicherbild: Programmcode und statische Daten während der ganzen
  Laufzeit, Freigabe durch das Betriebssystem am Ende. Stack bis Block- bzw.
  Funktionsende, automatisch. Heap bis zum ausdrücklichen `free`.
- Versuch mit lokaler Adresse: GCC `-Wreturn-local-addr`, Clang
  `-Wreturn-stack-address`. Zugriff ist undefiniertes Verhalten; ein zufällig
  richtiger Wert beweist nichts. Nicht in `tresorraub.c` enthalten.

**Seite 2**

- Notizen: `<stdlib.h>`; Größe in Bytes; Pointer oder `NULL`; Inhalt
  unbestimmt; `free(NULL)` tut nichts; danach kein Zugriff und kein zweites
  `free`.
- Array mit fünf `int`: `malloc(5 * sizeof(*werte))`.
- Versuche (nicht in `tresorraub.c` enthalten): ohne Werkzeug oft keine
  Meldung; AddressSanitizer meldet `heap-use-after-free` und `double-free`;
  LeakSanitizer (Linux) bzw. `leaks` (macOS) meldet das vergessene `free`.
  Vertauschte Reihenfolge (`wecker = NULL` vor `free`): kein Absturz, keine
  Compilerwarnung, `free(NULL)` tut nichts; LeakSanitizer bzw. `leaks`
  meldet ein Leck wie beim vergessenen `free`.
- Fehlerpfad bei zu langem Namen: Speicher ist schon angefordert und muss vor
  `return NULL` freigegeben werden.

**Seite 3**

- Testtabelle: leer → „Schluessel“ → „Beute, Schluessel“ → abgelehnt
  (unverändert) → „Muenze, Beute, Schluessel“ → abgelehnt, voll (unverändert).
- Reihenfolge der Prüfungen: Regeln vor `malloc` prüfen, sonst muss bei jeder
  Ablehnung freigegeben werden.
- Liste oder drei `bool`: Für drei feste Gegenstände sind `bool`-Werte
  einfacher. Die Liste lohnt sich bei vielen oder wechselnden
  Gegenstandsarten, Mehrfachvorkommen oder wenn die Reihenfolge zählt.
  Im Spiel ist die Liste vor allem eine Übung, die beim Zusammenführen
  weiterverwendet wird.

**Seite 4**

- Pointeränderungen: Mitte und Ende über den Nachfolger des Vorgängers, Kopf
  über den Listenkopf, nicht vorhanden und leer ohne Änderung.
- Aufräumen: Nachfolger vor `free` merken, am Ende Kopf `NULL`.
- Besitz: `eintrag_anlegen` gibt Besitz an den Aufrufer; nach erfolgreichem
  Aufnehmen besitzt das Inventar den Eintrag; lesende Funktionen mit
  `const` leihen nur; bei Fehlern hat die Funktion nichts angefordert oder
  bereits freigegeben.

**Seite 5**

- Inventar ersetzt `hat_schluessel`, `hat_beute`, `hat_muenze`; Startzustand
  mit leerem Kopf. `aufraeumen` leert das Inventar des laufenden
  Spielstands, lokale Spielstände leert ihre Funktion selbst.
- Versuch Kopie: Die Kopie teilt sich die Einträge; nach dem Leeren der
  Kopie zeigt der Kopf des Originals auf freigegebenen Speicher,
  AddressSanitizer meldet `heap-use-after-free`. Nicht in `tresorraub.c`.
- Testtabelle: siehe Selbstkontrolle und `./programm --text`, Abschnitt
  „Seite 5“ (Züge 0, 0, 0, 1, 2, 3, 4; Inventar am Ende „Beute, Muenze“).
- Fehlerfälle: Spielfälle liefern 0, Speicherfehler einen negativen Wert;
  der Spielstand bleibt bei Speicherfehler unverändert (Inventar, Feld,
  Zugzahl); `aufraeumen` wird vom Gerüst in jedem Fall aufgerufen.

**Seite 6**

- Papier: Beute auf (11, 3), Münze auf (11, 4), ein Zug.
- Testtabelle: siehe Selbstkontrolle und `./programm --text`, Abschnitt
  „Seite 6“ (Züge 5, 5, 5, 6, 7; Münze zuletzt auf (2, 1)).
- Kein Platz: tritt im Spiel nicht auf (höchstens zwei Gegenstände
  gleichzeitig im Inventar). Auf Papier: abgelegte Einträge werden beim
  Aushängen freigegeben, verbleibende bleiben korrekt verkettet. In den
  Tests mit einem Eintrag ohne Position („Kompass“) geprüft.
- Varianten: Ablegen über die Entfernen-Funktion von Seite 4 (Nachfolger
  vorher merken) ist ebenso richtig wie das direkte Aushängen in der Lösung.
