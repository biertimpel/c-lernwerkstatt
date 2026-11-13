# Musterlösung „Config“ – nur für Lehrende

Eine mögliche Lösung der Aufgabe auf Seite 3. Grundlage ist der Stand am
Ende des Kapitels „Datei-Ein- und -Ausgabe“ mit Speichern (F5) und Laden (F9)
(Startmaterial). Andere
Funktionszuschnitte, Fehlertexte und Darstellungen der Regeln sind ebenso
vertretbar, solange Verhalten und Format aus `REGELN.md` eingehalten
werden. Die Entwurfsentscheidungen stehen am Anfang von `tresorraub.c`.

Dieser Ordner wird nicht in das Startmaterial gepackt und auf keiner
Kursseite verlinkt. Er ist aber im öffentlichen Repository und über GitHub
Pages abrufbar.

## Inhalt

- `tresorraub.c`: Lösung, ersetzt `tresorraub.c` aus dem Startmaterial.
  Die Versuche der Seiten 1 und 2 sind nicht enthalten.
- `tests.c`: prüft das Laden der Regeln mit eigenen Testdateien: fehlende,
  vollständige, teilweise und leere Datei, Grenzwerte, eine Zeile mit genau
  80 Zeichen, Windows-Zeilenenden, letzte Zeile ohne Zeilenende, alle
  Fehlerfälle mit Zeilennummer und „alles oder nichts“. Prüft außerdem die
  Wirkung im Spiel (Reichweite, Zuglimit in der Statuszeile, Inventargröße)
  sowie `spiel_starten` und `text_ausgeben`. Laden eines Spielstands mit
  den geltenden Regeln: zu großes Inventar wird mit Meldung abgelehnt,
  Zuglimit und Reichweiten kommen aus den Regeln, Rundlauf und F5/F9 über
  `taste_gedrueckt`. Vorhandene `regeln.txt` und `spielstand.txt` werden
  vorher gesichert und danach wiederhergestellt.
- `Makefile`: verwendet Gerüst und Piktogramme aus
  `../materialien/tresorraub-regeln`

## Bauen und prüfen

```sh
make
./tests            # alle erwarteten Ergebnisse
cp ../materialien/tresorraub-regeln/regeln.txt .
./programm --text  # Regeln, Spielfeld und Statuszeile
./programm         # Spiel im Fenster, Tasten laut INVENTAR.md
```

Ohne `regeln.txt` im Ordner meldet das Programm die fehlende Datei und
spielt mit den Standardregeln.

Mit Speicherprüfung (Linux):

```sh
make clean
make CFLAGS="-std=c17 -Wall -Wextra -Wpedantic -g -fsanitize=address,undefined"
./tests && ./programm --text
```

Geprüft mit SDL 2.30 unter Linux mit GCC und Clang, übersetzt mit
`-std=c17 -Wall -Wextra -Wpedantic -Werror`, mit GCC zusätzlich mit
`-fsanitize=address,undefined` (einschließlich LeakSanitizer).

## Erwartungshorizont für Fragen ohne Code

**Seite 1**

- Notizen: `'\n'` steht mit im Array, wenn die Zeile passt; Rest einer
  langen Zeile kommt beim nächsten Aufruf.
- Kein `\n` im `printf`, weil es schon in der Zeile steht; letzte Zeile
  ohne Zeilenende endet ohne Umbruch.
- Leerzeilen und Kommentare: führenden Leerraum überspringen (`isspace`),
  dann `'\0'` bzw. `'#'` prüfen. Ausgabe nur `2: stunde = 6` und
  `4: minute = 45`; die Nummer zählt alle Zeilen.
- Array mit 8 Zeichen: Zeilen kommen in Stücken zu 7 Zeichen, jedes Stück
  bekommt eine eigene Nummer. Erkennbar an fehlendem `'\n'`, obwohl
  `feof` noch nicht gilt.

**Seite 2**

- `strchr`: Zeiger auf das erste Vorkommen oder `NULL`. Aufteilen mit
  `'\0'` ergibt `[stunde ]` und `[ 6\n]`. Zeile ohne `=` ist ein Fehler.
- `sscanf(…, "%31s %c", …)`: `"stunde "` → 1, `"   "` → `EOF`,
  `"st unde "` → 2. Genau ein Name heißt Rückgabe 1.
- `strtol`-Tabelle: siehe Selbstkontrolle auf der Seite. Ein Wert ist
  gültig, wenn `ende` nicht am Anfang steht, danach nur Leerraum folgt und
  der Bereich stimmt. Anders als im Datei-Kapitel darf nach der Zahl
  Leerraum folgen. Überlauf (`LONG_MAX`) fällt bei der Bereichsprüfung auf;
  `errno` zusätzlich zu prüfen ist ebenso richtig.

**Seite 3**

- Regeln als Struktur mit Standardwerten; Laden in eine Kopie, erst bei
  fehlerfreiem Ende übernehmen.
- Zuordnung der Namen: Die Lösung nutzt eine Tabelle (Array von
  `Regelangabe` mit Name, Grenzen, Zeiger auf das Feld in der Kopie und
  `gelesen`). Ebenso gut: ein `if`-Block je Regel mit gemeinsamer
  Hilfsfunktion für die Prüfung. Parallele Arrays (Namen, Minima, Maxima,
  Markierungen getrennt) im Gespräch als Schwachstelle ansprechen.
- Der Spielstand trägt seine Regeln; `startzustand` erhält sie als
  Parameter; Statuszeile und Aufnehmen lesen sie aus dem Spielstand.
  Eine eigene globale Variable für die Regeln ist ebenfalls vertretbar.
- Prüftabelle: Meldungen der Lösung entsprechen wörtlich der Tabelle auf
  der Seite (`./programm --text` mit der jeweiligen `regeln.txt`).
- Fenster: Reichweite 3 macht (12, 5) überwacht; Inventargröße 1 lässt
  jeden weiteren Gegenstand liegen; Zuglimit erscheint in der Statuszeile.
- Laden (Schritt 5): Ladeversuch mit `startzustand(&regeln)`; Prüfung der
  Inventaranzahl gegen `inventar_groesse` vor dem Aufbau der Liste;
  Meldung `spielstand.txt: 2 Gegenstaende im Inventar, laut regeln.txt sind
  hoechstens 1 erlaubt.` Die Formatgrenze von drei Namen bleibt als eigene
  Konstante (`INVENTAR_MAX`).
- Ausblick „Level“: bewusst ohne Lösung; Grundlage für Projektideen.
