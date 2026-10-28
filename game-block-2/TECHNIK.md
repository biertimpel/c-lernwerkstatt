# Architektur und C-Referenz

## Dateien und Zuständigkeiten

| Datei | Verantwortung |
|---|---|
| `spiel.h` / `spiel.c` | Gemeinsame Grundtypen, Karte, Spielphase, Bewegung, Überwachungsprüfung und Zugabschluss |
| `inventar.h` / `inventar.c` | Besitz und Lebensdauer der dynamischen Inventarknoten |
| `objekt.h` / `objekt.c` | Basistyp, konkrete Objekttypen, Methodentabellen, Erzeugung und Freigabe der Weltobjekte |
| `eingabe.h` / `eingabe.c` | Ganze Eingabezeilen, Befehlserkennung und streng geprüfte Ganzzahlen |
| `speichern.h` / `speichern.c` | Versioniertes Textformat, Schreiben und geprüftes Laden |
| `anzeige.h` / `anzeige.c` | Terminaldarstellung und Ausgabefehler |
| `main.c` | Startoptionen, Befehlsschleife, Aufruf der Bausteine und Aufräumen |

Die Header liegen unter `spiel/include/`, die Implementierungen unter `spiel/src/`.

## Datenstrukturen und Besitz

`Spiel` enthält das Kartenarray, den Spieler, den Kopf der Weltobjektliste, den Zugzähler und die Spielphase. `Spieler` enthält seine Position und den Kopf der Inventarliste.

Die Weltobjektliste besitzt ihre Knoten und deren konkrete Objekte. Die Inventarliste besitzt ihre Knoten. Objektabfragen liefern nur geliehene Pointer; diese werden vom Aufrufer nicht freigegeben. Nach `spiel_freigeben` sind die Listen leer. Beim Laden wird ein separater Kandidat angelegt. Auf Fehlerpfaden wird dieser freigegeben. Bei Erfolg wird zuerst das alte Spiel freigegeben und anschließend der neue Zustand übernommen.

Das Museum ist in dieser Referenz fest vorgegeben. Die zur Laufzeit erzeugten und entfernten Inventareinträge demonstrieren veränderliche Sammlungen. Die Weltobjekte werden separat angelegt, weil konkrete Typen unterschiedliche Größen und Lebenszyklen haben. Ein späteres Level-Dateiformat könnte die Anzahl und Typen zusätzlich zur Laufzeit bestimmen; das ist keine Funktion der Grundversion.

## Objektorientierung in C

Der gemeinsame Basistyp `Objekt` enthält ID, Typ, Position, Aktivitätszustand und einen Pointer auf eine konstante Methodentabelle. Die konkrete Struktur bettet diese Basis als erstes Feld ein und ergänzt ihre eigenen Daten:

- `Aufhebbar`: ID des aufzunehmenden Gegenstands.
- `Tuer`: Schlüsseltyp und Zustand offen/geschlossen.
- `Tresor`: Zustand geleert/nicht geleert.
- `Waechter`: Überwachungsradius.
- `Ausgang`: gemeinsames Objekt mit eigenem Verhalten beim Betreten.

Die Methodentabelle enthält Funktionspointer für Darstellung, Betreten und Freigabe. `spiel_bewegen` ruft dieselbe Schnittstelle auf; die konkrete Funktion wird über die Methodentabelle gewählt. Schlüsselaufnahme, Türöffnung und Tresorentnahme sind damit verschiedene Implementierungen derselben Operation. Die Typumwandlungen in den konkreten Methoden sind durch die Zuordnung von erzeugtem Objekttyp und passender Tabelle begründet. Es werden keine beliebigen fremden Pointer umgedeutet.

Die C-Referenz zeigt Struktureinbettung und dynamische Methodenaufrufe. Sie bildet keine vollständige C++-Laufzeit nach. Die Überwachungsprüfung und das feste Spielstandformat kennen einzelne konkrete Typen. Die Datenstrukturen sind in diesem Lehrprototyp öffentlich sichtbar. Stärkere Kapselung oder eine zusätzliche virtuelle Überwachungsoperation wären mögliche spätere Entwurfsdiskussionen.

## Fehlercodes und Diagnose

| Ergebnis | Bedeutung |
|---|---|
| 0 | Aktion erfolgreich |
| 1 | Regulär blockierte Bewegung, kein Programmfehler |
| 2 | Ende des Eingabestroms |
| -1 | Speicher konnte nicht angelegt werden |
| -2 | Unbekannter oder ungültiger Befehl |
| -3 | Ein-/Ausgabefehler |
| -4 | Ungültiges Spielstandformat |
| -5 | Ungültige Funktionsargumente oder unpassende Spielphase |

Ein ungültiger Befehl oder eine blockierte Bewegung beendet das Spiel nicht. Ein fehlerhafter Ladevorgang lässt das alte Spiel bestehen. Ein Speicherfehler wird bis zur Hauptfunktion weitergegeben; das Programm räumt auf und endet mit Status 1. Ein regulärer Sieg oder eine reguläre Niederlage ist kein Programmfehler und endet mit Status 0.

`--debug` gibt Befehl, Ergebniscode, Position, Zugzähler und Phase auf stderr aus. Diese Ausgaben sind optional und gehören nicht zur regulären Spielanzeige.

## Spielstandformat, Version 1

Es wird ein Textformat geschrieben:

```text
TRESORRAUB 1
3 7 0
1 0 0 1
0
```

1. Kennung und Version.
2. Spieler-x, Spieler-y, verbrauchte Züge.
3. Schlüssel noch auf Karte, Tür offen, Tresor geleert, Münze noch auf Karte. Jedes Feld ist 0 oder 1.
4. Anzahl der Inventareinträge.
5. Anschließend eine Gegenstands-ID pro Zeile: 1 Schlüssel, 2 Beute, 3 Münze.

Die feste Karte und die unveränderlichen Wächter werden aus der Programmdefinition rekonstruiert. Gespeichert werden alle veränderlichen spielrelevanten Werte der Grundversion. Die flüchtige Meldungszeile wird beim Laden durch eine Lademeldung ersetzt. Gespeichert werden nur laufende Spiele.

Pointer, Listenknoten, Funktionsadressen und Methodentabellen werden **nicht** als Speicherabbild geschrieben. Beim Laden entstehen neue Instanzen und neue Listenknoten; die Methodentabellen werden durch die Erzeugungsfunktionen wieder zugeordnet. Das ist eine wichtige Verbindung zwischen Datenmodell, Speicherlebensdauer und Persistenz.

Der Loader prüft Version, Feldanzahl, Wertebereiche, Position, Überwachungszone, Inventar-IDs, Duplikate, zusätzliche Daten und die Konsistenz der Gegenstandszustände. Er ist kein kryptografischer Manipulationsschutz und beweist nicht, dass ein Spielstand durch eine ehrliche Zugfolge entstanden ist.

Schreiben prüft Öffnen, einzelne Schreiboperationen und Schließen. Das Speichern überschreibt die gewählte Zieldatei. Es gibt in diesem Prototyp keine atomare Sicherung mit Backup; ein Datenträgerfehler beim Schreiben kann die bisherige Datei beschädigen. Laden übernimmt einen Zustand erst nach vollständiger erfolgreicher Prüfung.

## Nachschlagbare Grundlagen

- GNU C Language Manual, Function Pointers: https://www.gnu.org/software/c-intro-and-ref/manual/html_node/Function-Pointers.html
- GNU C Language Manual, Pointers and Arrays: https://www.gnu.org/software/c-intro-and-ref/manual/html_node/Pointers-and-Arrays.html
- Die lokal installierten Manpages zu `malloc`, `free`, `fgets`, `strtol`, `fopen`, `fprintf`, `fclose`.

Die Quellen dienen als Nachschlagepunkte. Der Kapitelplan ist ein eigener didaktischer Vorschlag, keine aus diesen Quellen abgeleitete Zeit- oder Erfolgszusage.
