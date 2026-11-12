# SDL2: Mein erstes Fenster fuer den Tresorraub

Dieses Material gehoert zur Lektion „Manpages und Bibliotheksdokumentation“.
Es stellt das technische Geruest bereit, keine fertige Spielmechanik.
Sie koennen ein eigenes Motiv entwickeln oder die optionalen Beispiele nutzen.
Beide Wege sind gleichwertig. Die Bibliotheksrecherche bleibt in beiden Faellen
Ihre eigene Aufgabe.

## Voraussetzungen und Einrichtung

Benoetigt werden ein C17-Compiler, Make und ein SDL2-Entwicklungspaket mit
`sdl2-config`. Das Material verwendet ausschliesslich die SDL2-API, keine
SDL3-API, SDL_image oder SDL_ttf. Alle verwendeten Zeichenoperationen und
Fensterfunktionen gibt es seit SDL 2.0.0. Fuer einen Kursrechner soll eine
aktuelle verfuegbare SDL2-Version einheitlich eingerichtet und getestet sein.

Beispiele zur Einrichtung (nicht automatisch durch das Material ausgefuehrt):

- **macOS:** Apple-Compiler/Command Line Tools und Make muessen vorhanden sein.
  Wenn Homebrew bereits eingerichtet ist: `brew install sdl2`.
  Falls `sdl2-config` nicht gefunden wird, pruefen Sie den Suchpfad mit der
  Betreuung. Nicht SDL3 statt SDL2 installieren.
- **Debian/Ubuntu:** `sudo apt install build-essential libsdl2-dev`.
- **Windows:** Im **MSYS2-UCRT64-Terminal** mit
  `pacman -S --needed make mingw-w64-ucrt-x86_64-gcc mingw-w64-ucrt-x86_64-SDL2`
  die Werkzeuge bereitstellen. Danach dort `make CC=gcc` verwenden und
  `./programm.exe` starten. Die benoetigten DLLs muessen im Suchpfad bleiben.
  Nicht wahllos MSVC-, MinGW-, 32- und 64-Bit-Bibliotheken mischen.

Installationsschritte koennen Administratorrechte oder Netzwerkzugriff
benoetigen. Wenn die unveraenderte Fassung nicht startet: Betreuung fragen.
Die Einrichtung darf den eigentlichen Rechercheauftrag nicht ersetzen.

## Bauen und starten

Im entpackten Ordner:

```sh
make
./programm --version
./programm
```

Das leere Geruest zeigt nur einen hellen Hintergrund. Zeichnen Sie selbst in
`zeichnung.c`. Nach Aenderungen speichern und erneut `make` ausfuehren.
Schliesssymbol oder Escape beenden mit Status 0. Compilerwarnungen beachten.
Ein fehlgeschlagener SDL-Aufruf fuehrt zur Meldung auf stderr und Status 1.
Unbekannte Kommandozeilenoptionen ergeben Status 2.

Das Geruest gibt Header- und Laufzeitversion getrennt aus. Die Unterversionen
muessen nicht exakt gleich sein; die benoetigte API muss aber verfuegbar und
kompatibel sein. Die passende Dokumentation beginnt mit
https://wiki.libsdl.org/SDL2/ . SDL3-Dokumentation nicht ungeprueft uebernehmen.
SDL empfiehlt inzwischen SDL3 fuer neue Projekte. Dieser Kurs verwendet
bewusst die SDL2-API, auch bei einer kompatiblen Implementierung.

## Welche Datei bearbeite ich?

- `zeichnung.c`: Ihr Zeichenablauf. Anfangs leer; keine Motivkoordinaten
  vorgeschrieben. Der Renderer ist vorbereitet und wird nur weitergegeben.
- `zeichnung.h`: Legt fest, was `zeichne` zurueckgibt: 0 bei Erfolg,
  einen negativen Wert bei Fehler.
- `programm.c`: Fenster, Renderer, Hauptschleife, Diagnose und Aufraeumen.
  Das ist zunaechst Infrastruktur, keine Pflichtaufgabe zur Pointerverwaltung.
- `piktogramme/`: optionale Motive samt Schnittstellen und SVG-Vorschauen.
- `DOKUMENTATION.md`: leere Notizhilfe fuer Ihre eigene Recherche.

Der logische Zeichenbereich ist 640 x 480. Links oben ist (0, 0), die letzte
sichtbare ganzzahlige Position (639, 479). Das Geruest loescht vor jedem Bild,
ruft Ihren Zeichencode auf und praesentiert danach den Hintergrundpuffer.
Nach dem Loeschen ist eine dunkle Standardzeichenfarbe vorbereitet; eigene
Farben koennen Sie innerhalb Ihres Zeichencodes setzen.
Zeichnen Sie nicht nur einmal vor der Schleife. Den Renderer nicht selbst
zerstoeren. Pointer und Strukturen innerhalb des Geruests werden spaeter
behandelt; fuer diese Lektion genuegt die bereitgestellte Schnittstelle.

## Optionale Piktogramme

```sh
./programm --galerie
```

Die 14 Motive erscheinen in vier Spalten, zeilenweise:

1. Spielfigur froh
2. Spielfigur entdeckt
3. Waechter
4. Schluessel
5. Tuer geschlossen
6. Tuer offen
7. Tresor geschlossen
8. Tresor leer
9. Ausgang
10. Muenze
11. Wand
12. Boden
13. Ueberwachungsfeld
14. Beute fuer das Inventar

Jede Funktion in `piktogramme/piktogramme.h` erhaelt Renderer, x, y und
Skalierung. x und y sind die linke obere Ecke, zulaessig jeweils -10000 bis
10000. Jedes Motiv hat 16 x 16 Zellen, Skalierung 1 bis 16 Pixel pro Zelle.
Die Hilfsfunktionen liefern 0 bei Erfolg und einen negativen Wert bei Fehler.
Ihre SVG-Vorschauen unter `piktogramme/vorschau/` lassen sich ohne Installation
im Browser oeffnen. In `piktogramme/index.html` finden Sie die ganze Sammlung.

In Ihrer Funktion `zeichne` duerfen Sie einen solchen Aufruf direkt
zurueckgeben. Bei mehreren Motiven jeden Rueckgabewert pruefen und Fehler
unmittelbar weitergeben. Fuer die eigene Recherche finden Sie in
`piktogramme.c` die echten SDL2-Aufrufe `SDL_SetRenderDrawColor` und
`SDL_RenderDrawLine`. Ein Hilfsfunktionsname ist kein SDL-Bibliotheksname.

Die internen Raster und Schleifen sind vorbereitete Infrastruktur. Sie
muessen dafuer noch keine Arrays implementieren. Uebernommene Motive sind
ausdruecklich erlaubt; erklaeren Sie eine verwendete SDL2-Funktion und
veraendern Sie selbst einen sichtbaren Parameter. Die Farbwerte stehen in
`farbe_setzen` in `piktogramme.c` und duerfen geaendert werden; eine
Aenderung gilt fuer alle Motive mit dieser Farbe. Die Motive sind eigens
fuer den Kurs erstellt und in Kursprojekten frei verwendbar. Sie enthalten
keine Spiellogik. Zustandsunterschiede sind nicht nur durch Farbe erkennbar.

## Diagnoseversuch

```sh
./programm --diagnose
echo $?
```

Nach erfolgreichem Fensteraufbau setzt `SDL_SetError` absichtlich einen
simulierten Fehler und liefert -1. Der gemeinsame Fehlerpfad meldet die
Diagnose, gibt Ressourcen frei und endet mit Status 1. Es wird kein echter
Grafikdefekt behauptet. Kann schon der Fensteraufbau nicht gelingen, erscheint
stattdessen dessen Fehlermeldung; dann zuerst die Umgebung klaeren.

Bei der Fehleranalyse den Rueckgabewert gemaess Dokumentation pruefen und
erst danach `SDL_GetError` auswerten. Eine alte Fehlermeldung kann nach
erfolgreichen Aufrufen bestehen bleiben. Beispielsweise liefert
`SDL_RenderPresent` in SDL2 void, also keinen vergleichbaren Fehlercode.

## Fuer Betreuung und automatische Pruefungen

`./programm --testlauf` erzeugt ein verstecktes Fenster, zeichnet genau ein
Bild und endet selbststaendig. Kombinierbar mit `--galerie` oder `--diagnose`.
Mit einem SDL2-Bau, der den Dummy-Videotreiber enthaelt, ist beispielsweise
`SDL_VIDEODRIVER=dummy ./programm --galerie --testlauf` moeglich. Das ersetzt
keine Sichtpruefung mit einem echten Fenster und keine Tastaturpruefung.

Bei abweichendem Installationsort laesst sich `SDL2_CONFIG` anpassen:
`make SDL2_CONFIG=/pfad/zu/sdl2-config`. Das Makefile soll vorgegeben bleiben.
Wie ein Makefile aufgebaut ist, lernen Sie im Kapitel „Header und
Quelldateien“ auf der Seite „Den Build mit einem Makefile ausfuehren“.
