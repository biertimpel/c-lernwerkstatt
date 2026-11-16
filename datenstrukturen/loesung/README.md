# Musterlösung „Datenstrukturen“ – nur für Lehrende

Eine mögliche Lösung aller Aufgaben der Seiten 1 bis 5. Andere
Datendarstellungen und Funktionszuschnitte sind ebenso vertretbar. Die
Entwurfsentscheidungen stehen am Anfang von `tresorraub.c`.

Dieser Ordner wird nicht in das Startmaterial gepackt und auf keiner
Kursseite verlinkt.

## Inhalt

- `tresorraub.c`: Lösung, ersetzt `tresorraub.c` aus dem Startmaterial
- `tests.c`: prüft alle erwarteten Werte aus den Selbstkontrollen
- `Makefile`: verwendet Gerüst und Piktogramme aus
  `../materialien/tresorraub-daten`, es gibt also nur eine Fassung davon

## Bauen und prüfen

```sh
make
./tests            # alle erwarteten Werte der Selbstkontrollen
./programm --text  # Textausgaben der Seiten 1, 2, 3 und 5
./programm         # Spielfeld mit Piktogrammen im Fenster
```

Geprüft mit SDL 2.30 unter Linux, übersetzt mit
`-std=c17 -Wall -Wextra -Wpedantic -Werror` sowie mit
`-fsanitize=address,undefined`.

## Zuordnung zu den Aufgaben

`./programm --text` gibt die Ergebnisse abschnittsweise aus, jeweils mit
Seite und Abschnitt als Überschrift. Die Darstellung im Fenster (Seiten 3
bis 5) übernimmt `zeichne()`; sie zeigt gleich den vollständigen
Startzustand aus Seite 5.

Bewusst nicht im Programm enthalten sind Versuche, die nur eine Warnung oder
einen Übersetzungsfehler zeigen sollen: das Array ohne Initialisierung
(Seite 1), der Vergleich von Zeichenketten mit `==` (Seite 2) und der
Vergleich von Strukturen mit `==` (Seite 4). Die erwarteten Meldungen stehen
unten.

## Erwartungshorizont für Fragen ohne Code

**Seite 1**

- Array ohne Initialisierung: unbestimmte Werte, GCC warnt mit
  `-Wmaybe-uninitialized`, Clang meldet nichts. Folgerung: vor dem Lesen
  füllen.
- `<` durch `<=` ersetzen: Die Schleife greift auf `zuege[5]` zu,
  undefiniertes Verhalten.
- Leere Reihe: Anzahl im Limit ist 0. Minimum und Durchschnitt sind nicht
  definiert; vertretbar sind ein vereinbarter Sonderwert (hier −1 und 0.0),
  eine Vorbedingung „anzahl > 0“ oder ein zusätzlicher Erfolgswert. Wichtig
  ist, dass die Entscheidung im Vertrag steht.

**Seite 2**

- `==` bei Zeichenketten: vergleicht die Speicherstellen, nicht den Inhalt.
  Array gegen Text in Anführungszeichen ist immer falsch. GCC und Clang
  warnen (`-Waddress` bzw. `-Wstring-compare`).
- Größe der Statuszeile: Längster Fall im Format der Lösung ist
  `Position (14, 8) | Zuege 60/60` mit 30 Zeichen, also mindestens 31 Bytes.

**Seite 3**

- `BREITE + 1`: 15 sichtbare Zeichen plus `'\0'`.
- Indexreihenfolge: `[y][x]`, weil eine Zeile einem festen y-Wert entspricht.
  Prüffeld zum Beispiel (6, 1): `[1][6]` ist eine Wand, `[6][1]` ein freies
  Feld.
- Schleifen vertauschen: Spielfeld gekippt, 15 Zeilen mit je 9 Zeichen.
- Prüfreihenfolge: Schon ein Zugriff mit ungültigem Index ist undefiniertes
  Verhalten; `&&` wertet von links aus und bricht ab.
- Zeichen oder Zahlencodes: Zeichen sind im Quelltext lesbar und direkt
  ausgebbar; Zahlencodes trennen Daten und Darstellung, brauchen aber Namen
  (`#define` oder später `enum`). Neuer Feldtyp: bei Zeichen ein neues Zeichen
  und eine neue Darstellung, bei Zahlen ein neuer Code, ein Name und eine
  neue Darstellung. Beides ist vertretbar, wenn begründet.

**Seite 4**

- Vergleich von Strukturen mit `==`: Fehler „invalid operands to binary ==“.
- Array von Wächtern: Schleifen statt doppeltem Code; für einen dritten
  Wächter nur die Arraygröße und die Daten ändern.
- Betrag: `abs` aus `<stdlib.h>`.
- Überwachte Felder in Zeile 5 beim Wächter (9, 5): x = 7 bis 11; (12, 5) ist
  das erste freie Feld rechts davon, das nicht überwacht ist. Diagonal
  überwacht ist zum Beispiel (10, 6).

**Seite 5**

- Konstant: Grundriss, Positionen von Ausgang, Tür, Tresor und Wächtern.
  Veränderlich: Position der Figur, Züge, Inventar, Zustände von Tür und
  Tresor, ob Schlüssel und Münze noch liegen.
- Objekte im Grundriss eintragen: Ausgabe einfacher, aber beim Aufnehmen
  muss das Feld zurückgesetzt werden und Zustände wie „Tür offen“ stecken im
  Zeichen. Getrennt speichern: Aufnehmen ändert nur einen Wert, die Ausgabe
  braucht eine Rangfolge.
- Kopie: Der Aufrufer behält 0 Züge. `sizeof(Spielstand)` ist in der Lösung
  96 Bytes (ohne Grundriss); mit Grundriss kämen 144 Bytes dazu.
