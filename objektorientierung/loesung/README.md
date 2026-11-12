# Musterlösung „Objektorientierung in C“ – nur für Lehrende

Eine mögliche Lösung der Aufgaben auf den Seiten 1, 4 und 5. Grundlage ist
der Stand am Ende des Kapitels „Config“ (Startmaterial). Andere Zuschnitte
von Art, Objekt und Methoden sind ebenso vertretbar, solange das Verhalten
laut `INVENTAR.md` und `OBJEKTE.md` erhalten bleibt. Die
Entwurfsentscheidungen stehen am Anfang von `tresorraub.c`.

Dieser Ordner wird nicht in das Startmaterial gepackt und auf keiner
Kursseite verlinkt. Er ist aber im öffentlichen Repository und über GitHub
Pages abrufbar.

## Inhalt

- `tresorraub.c`: Lösung einschließlich Seite 5 (Geheimtür, zweite Münze),
  ersetzt `tresorraub.c` aus dem Startmaterial.
- `tests.c`: prüft Objektarten, Startzustand, alle Methoden über
  `taste_gedrueckt` (Wand, Tür ohne und mit Schlüssel, Aufnehmen, Tresor,
  Ablegen, Wiederaufnehmen), zweite Münze, Geheimtür, Vorrang des
  Gegenstands vor der Tür, `inventar_groesse = 1` aus `regeln.txt`,
  Speicherfehler beim Aufnehmen (Spielstand unverändert) und `zeichne`.
  Eine vorhandene `regeln.txt` wird gesichert und wiederhergestellt.
- `Makefile`: verwendet Gerüst, Piktogramme und `ablauf.c` aus
  `../materialien/tresorraub-objekte`.

## Bauen und prüfen

```sh
make
./tests
cp ../materialien/tresorraub-objekte/regeln.txt .
./ablauf > ende.txt
```

Vergleich mit dem Startmaterial (dort `make ablauf && ./ablauf > vorher.txt`):
Meldungen und Statuszeilen sind gleich, alle Bild-Prüfsummen weichen ab,
weil die zweite Münze sichtbar ist. Ohne die beiden Objekte von Seite 5
(Zeilen in `startzustand`, Feld (6, 6) im Grundriss) ist die Ausgabe
einschließlich aller Prüfsummen identisch mit `vorher.txt`; das ist der
erwartete Stand nach Seite 4.

Mit Speicherprüfung (Linux):

```sh
make clean
make CFLAGS="-std=c17 -Wall -Wextra -Wpedantic -g -fsanitize=address,undefined"
./tests && ./ablauf > /dev/null
```

Geprüft mit SDL 2.30 unter Linux mit GCC und Clang, übersetzt mit
`-std=c17 -Wall -Wextra -Wpedantic -Werror`, mit GCC zusätzlich mit
`-fsanitize=address,undefined` (einschließlich LeakSanitizer).

## Erwartungshorizont für Fragen ohne Code

**Seite 1**

- Schlüssel im Startmaterial: Felder `schluessel`, `schluessel_liegt` im
  `Spielstand`; Funktionen `startzustand`, `gegenstand_auf`, `feldzeichen`,
  `spielfeld_ausgeben` (Legende), `vom_feld_nehmen`, `auf_feld_legen`,
  `handeln`, `feld_zeichnen`, `zeichne`. Tür: `Spielstand`,
  `startzustand`, `feldzeichen`, `spielfeld_ausgeben`, `figur_bewegen`,
  `handeln`, `feld_frei`, `feld_zeichnen`.
- Gedankenexperiment: zweite Münze braucht neue Felder und Fälle an allen
  Stellen; die Namensregel im Inventar würde sie ablehnen.
- Vergleichslauf: Inhalt siehe Seite; geänderte Meldung erscheint im
  `diff` genau an ihren Stellen.
- `enum`: `0 1 2`; mit `AUS = -1`: −1, 0, 1, 2; `-Wswitch`-Warnung bei
  fehlendem `case` ohne `default`; `a = 7` ohne Meldung, kein `case`
  trifft.
- Aufgabe: z. B. `typedef enum { FEHLER_NAME = -2, FEHLER_SPEICHER,
  AUFGENOMMEN, SCHON_VORHANDEN, INVENTAR_VOLL } Aufnahme;` als Rückgabetyp.
  Die Lösung hat nach Seite 4 nur noch `FEHLER_SPEICHER`, `AUFGENOMMEN`,
  `INVENTAR_VOLL`, weil Namen und die Doppelregel entfallen.

**Seite 2**

- `9`, `5`; Tabelle ruft beide auf; `Rechnung r = plus(7, 2);` GCC
  `-Wint-conversion`-Warnung, Clang Fehler; `double halb(double)` GCC
  `incompatible pointer type`, Clang Fehler; Aufruf über `NULL` ist
  undefiniertes Verhalten.
- `typedef int (*Piktogramm)(SDL_Renderer *, int, int, int);`; die
  Inventar-`strcmp`-Kette entfällt, wenn jeder Eintrag sein Piktogramm
  kennt.

**Seite 3**

- Ausgabe des Beispiels und Speicherbild siehe Selbstkontrolle.
- Aufteilung: Art = Name, Zeichen, Piktogramm, aufnehmbar, Verhalten
  (Leertaste, Betreten); Objekt = Position, auf dem Feld, offen,
  Inhalt/voll. Zustandsabhängiges Aussehen über zwei Angaben oder eine
  Methode.
- Methoden: Leertaste und Betreten; Aufnehmen und Ablegen sind für alle
  Gegenstände gleich.

**Seite 4**

- Prüfkriterien: `diff` leer; Arten nur noch in Art, Methoden,
  Startzustand (Tür nennt die Art Schlüssel), Legende.
- Besitz: Objekte gehören dem Spielstand (kein `free`); Inventar besitzt
  nur seine Einträge.
- Kopie: `startzustand` füllt über Pointer, weil `Tresor.inhalt` und
  Inventareinträge auf Objekte im Spielstand zeigen.
- Häufige Fehler: Reihenfolge der Objekte ändert Zeichen oder Vorrang
  bei der Leertaste (zeigt sich im `diff`); Schlüssel nach dem Verbrauch
  noch `auf_feld`; Tresor wird bei vollem Inventar trotzdem leer.

**Seite 5**

- Zweite Münze: eine Zeile im Startzustand. Geheimtür: Art, Methode
  `benutzen`, Zeile im Startzustand, Grundriss (6, 6); `betretbar` mit der
  Tür geteilt. Leertaste, Bewegung, Ablegen, Ausgabe, Zeichnen unverändert.
- Vergleichslauf: Meldungen und Statuszeilen gleich, alle Prüfsummen
  anders.
