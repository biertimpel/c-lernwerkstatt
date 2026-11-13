# Musterlösung Lernstandscheck II „Licht aus“ – nur für Lehrende

Der Lernstandscheck ist eine freiwillige Selbstkontrolle ohne Abgabe und
Bewertung. Lösung und Erwartungshorizont dienen als Grundlage für
Rückmeldungen, wenn Studierende ihren Stand im Kurs zeigen oder Fragen haben.

Eine mögliche Lösung aller Aufgaben. Grundlage ist das Startmaterial (Ende
des Kapitels „Spielende“, Teil 2). Die Entwurfsentscheidungen stehen am
Anfang von `tresorraub.c`.

Dieser Ordner wird nicht in das Startmaterial gepackt und auf keiner
Kursseite verlinkt. Er ist aber im öffentlichen Repository und über GitHub
Pages abrufbar.

## Inhalt

- Alle Module und `tresorraub.c`; sie ersetzen die gleichnamigen Dateien
  des Startmaterials. Geändert gegenüber dem Start: `regeln` (neun
  Einträge), `spiel.h` (Licht und Zeit im Spielstand), `objekte` (Art
  Lichtschalter, Positionen aus den Regeln, `aufstellung_pruefen`),
  `spiellogik` (`zeit_fortschreiben`, Entdeckung nur bei Licht,
  Statuszeile), `speicherstand` (−3 im Dunkeln, Licht nach dem Laden),
  `tresorraub.c` (F5, `spiel_aktualisieren`, Dunkelheit zeichnen).
  `spielfeld` und `inventar` sind unverändert.
- `regeln.txt`: Anordnung der Musterlösung. Wächter 1 auf (1, 6),
  Schlüssel auf (1, 7) in seinem Bereich, Schalter auf (5, 7), 5 Sekunden.
- `tests_licht.c`: 46 Prüfungen der neuen Regeln mit erfundenen Zeiten
  (Namen der Musterlösung).
- `Makefile`: verwendet Gerüst, Piktogramme und `tests.c` aus
  `../materialien/tresorraub-licht`. `tests.c` wird dafür kopiert, weil es
  sonst die unveränderten Header neben sich einbinden würde.

## Bauen und prüfen

```sh
make
make test       # tests.c: 79 Pruefungen; tests_licht.c: 46 Pruefungen
./programm      # mit regeln.txt aus diesem Ordner
```

Geprüft mit GCC (`-Wall -Wextra -Wpedantic -Werror`,
`-fsanitize=address,undefined`) und Clang (`-Werror`).

Gewinnweg mit `regeln.txt` dieses Ordners (42 Züge, zügig gespielt):
rechts, Leertaste (Licht aus), dreimal links, Leertaste (Schlüssel), zweimal
rechts, hoch, rechts, dreimal hoch, rechts, Leertaste (Tür), zweimal rechts,
zweimal hoch, sechsmal rechts, Leertaste (Beute), sechsmal links, zweimal
runter, dreimal links, zweimal hoch, dreimal links. Wer nach dem Schlüssel zu
lange auf (1, 7) bleibt, wird beim Wiedereinschalten entdeckt.

## Erwartungshorizont

### Aufgabe 1: Plan

Erwartet sind konkrete Module und Funktionen, nicht nur „Lichtschalter
einbauen“. Gut ist ein Plan, der die betroffenen bisherigen Regeln nennt:
Entdeckung, Speichern, Laden, Statuszeile, Neustart, Zeichnen. Der Vergleich
am Ende benennt Abweichungen ehrlich.

### Aufgabe 2: Lichtschalter

- Neue Objektart mit eigener Methode für die Leertaste (Abstand 1),
  nicht betretbar. Keine Fallunterscheidung nach dem Namen in `handeln`
  oder `figur_bewegen`. Das ist der Kern der Aufgabe.
- Lichtzustand im Spielstand, nicht in einer globalen Variable eines
  anderen Moduls. Am Lichtschalter-Objekt (`offen`) ist vertretbar, wenn
  Laden und Neustart ihn zurücksetzen.
- Entdeckung nur bei Licht; die Prüfung bleibt an einer Stelle
  (`spielende_pruefen` o. ä.), nicht verteilt.
- Anzeige: Dunkelheit erkennbar, Figur sichtbar; Statuszeile mit
  `| Licht aus`.
- F5 im Dunkeln verweigert; nach Laden und N Licht an. Format: eine
  Objektzeile mehr, ohne Änderung am Speichern selbst.

### Aufgabe 3: Config

- Neun Einträge in der vorhandenen Tabelle der Regelangaben, Bereiche aus
  `BREITE` und `HOEHE`. Keine eigene zweite Leseschleife.
- Gesamtprüfung nach dem Lesen: Boden, keine Doppelbelegung, Startfeld
  nicht überwacht, mit verständlicher Meldung; das Spiel startet sonst
  nicht. Wo sie steht, ist eine Entwurfsfrage: In der Musterlösung in
  `objekte`, weil nur dort alle festen Positionen bekannt sind. Eine Prüfung
  in `regeln.c` mit eigenen Kopien der festen Positionen ist schwächer
  (doppeltes Wissen).
- Häufige Lücke: nur Wand geprüft, Doppelbelegung oder überwachtes
  Startfeld vergessen.
- Eigene Anordnung: Schlüssel nur im Dunkeln erreichbar, Schalter vorher
  erreichbar, Spiel lösbar, Weg und Zugzahl angegeben. `REGELN.md` ergänzt.

### Aufgabe 4: Zeit

- Prüfung in `spiel_aktualisieren` bzw. einer davon aufgerufenen Funktion,
  nicht in `taste_gedrueckt` und nicht in `zeichne`. Kein `SDL_Delay` und
  keine Warteschleife.
- Startzeitpunkt beim Betätigen; erneutes Betätigen startet neu.
- Beim Wiedereinschalten Entdeckung prüfen und Rückgabe 1, damit der Titel
  erneuert wird.
- Häufiger Fehler: Der Schalter kennt die Zeit nicht und ruft dort
  `SDL_GetTicks64()` direkt auf. Funktioniert, ist aber nicht mit
  erfundenen Zeiten testbar; im Gespräch darauf hinweisen.
- Umrechnung Sekunden → Millisekunden ohne Überlauf, `>=` beim Vergleich.

### Aufgabe 5: Prüfen

- `make test`: 0 fehlgeschlagen. Die einzige Prüfung, die die Zahl der
  Objekte betrifft, erlaubt bereits 8 bis `MAX_OBJEKTE`.
- Prüftabelle deckt die genannten Bereiche ab, mit beobachtetem Verhalten,
  nicht nur erwartetem. Gute Lösungen enthalten eigene Tests mit erfundenen
  Zeiten.
- Beschreibung ordnet die Änderungen Modulen zu und erklärt das
  Zusammenspiel Schalter → Spielstand → `spiel_aktualisieren` →
  Entdeckung.
