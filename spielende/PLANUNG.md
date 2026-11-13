# Planung: Kapitel „Spielende“

Stand 13.11.2026. Verantwortung: Larissa.
Stellung im Kurs: Block 2, Kapitel 10, nach „Objektorientierung in C“, vor
„Lernstandscheck II“. Ersetzt die frühere Planung „Die Spielbausteine
zusammenführen & Refactoring“: Die Bausteine sind seit Datenstrukturen in
einer durchgehenden `tresorraub.c` zusammengeführt; was fehlte, waren das
Spielende und die Aufteilung in Module.
Grundlage ist der Endstand von „Objektorientierung in C“
(`objektorientierung/loesung/tresorraub.c`).
Umfang: etwa zwei Termine (Teil 1 ein Termin, Teil 2 und Rückblick ein
Termin).

## Voraussetzungen

- „Vom Problem zum Programm II“: Zielbild, eigene Notizen (Seite 4)
- „Header und Quelldateien“: Header, `static`, Linker, Makefile
- „Pointer und Datenaustausch“: `zug_zaehlen_original`, `static`-Spielstand
- „Dynamischer Speicher und Listen“: Inventar freigeben (N)
- „Datei-Ein- und -Ausgabe“: Laden prüft Gültigkeit
- „Objektorientierung in C“: `enum`, Methoden, Vergleichslauf

## Seiten

| Seite | Inhalt | Ergebnis | Auswahlfragen |
|---|---|---|---|
| 1 Wann ist das Spiel zu Ende? | Was fehlt (Versuch); `enum Spielzustand` statt mehrerer `bool`; Sieg als Methode `betreten` des Ausgangs, Verlieren nach jedem Zug, Reihenfolge (Sieg im 60. Zug); beendetes Spiel schützen (Tasten, Funktionen, Speichern, Laden, N); Aufgabe in sechs Schritten mit Hinweisen; Prüftabelle; erwarteter `diff` | Spiel mit Sieg und Niederlage | Zustand; Reihenfolge; nach dem Ende |
| 2 WASTED | Stempel `zeichne_wasted` (Piktogramme), Zeichenreihenfolge; Erklärung: Alpha/Mischmodus, Rasterschrift, `SDL_RenderGeometry` mit Drehung; Aufgabe: Niederlage zeigen, Sieg selbst gestalten | Spielende sichtbar, neues `vorher.txt` | Stempel |
| 3 Module und Unit Tests | Module laut vorgegebenen Headern; Abhängigkeiten; Unit Test vs. Vergleichslauf (Charakterisierungstest); Aufgabe Aufteilen in vier Schritten; Hinweise zu Compiler- und Linkermeldungen; Zuordnungstabelle | sechs Module, `make test` 0 Fehler, `diff` leer | Module/static; Unit Tests |
| 4 Rückblick | Plan aus „Vom Problem zum Programm II“ neben das Programm legen: Anforderungen, Programmplan, Kapitelzuordnung, Zielbild-Abweichungen; eigene Arbeitsweise | schriftlicher Rückblick (eine Seite) | keine (Reflexion) |
| 5 Geschafft! | Feier: Konfetti (ohne Bewegung bei `prefers-reduced-motion`), Zahlen, Tabelle aller Kapitel von Block 2 mit Gelerntem und Beitrag zum Spiel, Ausblick | – | – |

## Entwurfsentscheidungen

- **Zwei Teile, ein Material.** Teil 1 in der bekannten einen Datei,
  Teil 2 im Unterordner `module/` mit eigenem Makefile, das Gerüst,
  Piktogramme und `ablauf.c` aus dem Elternordner verwendet. So bleibt der
  Stand von Teil 1 als Vergleich erhalten.
- **Namen in `ENDE.md`:** `Spielzustand` mit `LAEUFT`, `GEWONNEN`,
  `ENTDECKT`, `ZEIT_ABGELAUFEN` und das Feld `zustand`, weil die Header in
  Teil 2 sie verwenden. Wie die Methode heißt, entscheiden die
  Studierenden in Teil 1; der Header in Teil 2 nennt sie `betreten`.
- **Header und Test-Suite vorgegeben (Teil 2).** Die Studierenden schreiben
  die Quelldateien selbst. Die Suite (79 Prüfungen) setzt den Vermerk um:
  Unit Tests zeigen beim Umbau, dass die Funktionalität erhalten bleibt;
  Abgrenzung zum Vergleichslauf auf Seite 3.
- **Nach dem Ende** ist der Schutz doppelt: in `taste_gedrueckt` (Meldung,
  N und F9) und in den Funktionen (für Tests und weitere Aufrufer).
- **Laden** lehnt Spielstände ab, in denen das Spiel zu Ende wäre (Figur
  überwacht, auf einem Objekt mit `betreten`, Zuglimit erreicht).
- **WASTED-Stempel** als neues Piktogramm; braucht SDL ≥ 2.0.18. Ohne
  Markennamen auf den Seiten. Sieg wird frei gestaltet (Musterlösung:
  grüner Rahmen).
- **Umbenennung** `zug_zaehlen_original` → `zug_zaehlen` in Teil 2,
  `ist_wand` neu, Grundriss privat.

## Abhängigkeiten und offene Punkte

- **Lernstandscheck II** muss auf dem Endstand von Teil 2 aufbauen
  (Module, `Spielzustand`). Das Gerüst hat keinen Aufruf pro Bild für
  Spiellogik; ein Timer nach `SDL_GetTicks64` braucht entweder einen neuen
  Haken im Gerüst oder eine Prüfung in `zeichne`/`taste_gedrueckt`.
- **SDL-Version** auf den Pool-Rechnern prüfen (≥ 2.0.18).
- **Umfang:** Seite 3 ist der größte Brocken. Kürzbar, indem zusätzlich
  `spielfeld.c` und `regeln.c` vorgegeben werden.
