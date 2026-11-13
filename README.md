# C-Lernwerkstatt

Statische Kurswebsite für den C-Programmierkurs im Wintersemester 2026/27.
Start: **19. Oktober 2026**. Abschluss: **bis 11. Februar 2027**.
Pro Woche finden zwei Termine mit jeweils 90 Minuten statt. Der Abschluss ist unbenotet.

## Einstieg und Materialstand

Die zentrale Übersicht liegt in **`uebersicht.html`**. Dort sind alle vereinbarten
Termine und Projektphasen mit ihren Themen und Lernzielen erreichbar.
Die Weihnachtspause ist als Planungsannahme markiert. Die beiden konkreten
Wochentage und endgültigen Kursdaten werden im Kurs bekanntgegeben.

Mehrere Kapitel enthalten ausgearbeitete Aufgaben und Materialien. Weitere
Themen sind als Kapitelgerüste oder vorbereitete Themenaufrufe angelegt.
Unfertige Inhalte sind sichtbar als „in Vorbereitung“ gekennzeichnet.

## Startkapitel

- `start/index.html`: Übersicht für Termin 1.1.
- `start/mein-erstes-programm.html`: vorhandene Aufgabe mit Hello World und Rechnen.
- `start/warum-c.html`: Gerüst für die Einführung.
- `start/code-lesen.html`: Gerüst für die Aufgabe zum Codeverständnis.

`index.html` bleibt als bestehender direkter Einstieg erhalten. Für die weitere
Entwicklung der Startaufgabe ist `start/mein-erstes-programm.html` maßgeblich.
Die Kursübersicht verlinkt diese Fassung.

## Themenordner

Die Kursübersicht in `uebersicht.html` führt durch die Kapitel. Die Reihenfolge
von Block II orientiert sich an den Voraussetzungen der Spielbausteine.
Makefiles werden im Kapitel „Header und Quelldateien“ eingeführt.

| Block | Ordner / Themenaufruf | Thema |
| --- | --- | --- |
| I | `start/` | Einführung |
| I | `variablen/` | Variablen, Datentypen und Berechnungen |
| I | `kontrollfluss/` | Bedingungen und Schleifen |
| I | `funktionen-debugging/` | Programmzerlegung, Funktionen und Debugging |
| I | `vom-problem-zum-programm/` | Vom Problem zum Programm I |
| I | `lernstand-1/` | Lernstandscheck I |
| II | `vom-problem-zum-programm-2/` | Vom Problem zum Programm II |
| II | `manpages/` | Manpages und Bibliotheksdokumentation |
| II | `header-quellen/` | Header und Quelldateien einschließlich Makefile-Grundlagen |
| II | `datenstrukturen/` | Arrays, Zeichenketten und Strukturen |
| II | `pointer/` | Pointer und Datenaustausch |
| II | `dynamischer-speicher/` | Dynamischer Speicher und Listen |
| II | `datei-ein-ausgabe/` | Datei-Ein- und -Ausgabe |
| II | `config/` | Config |
| II | `objektorientierung/` | Objektorientierung in C |
| II | `spielende/` | Spielende |
| II | `lernstand-2/` | Lernstandscheck II – Licht aus |
| III | `projektplanung/` | Projekt planen |
| III | `projekt-grundumfang/` | Technischen Projektgrundumfang umsetzen |
| III | `projekt-verbesserung/` | Projekt verbessern, erweitern und prüfen |
| III | `vortragsvorbereitung/` | Vortrag vorbereiten |
| III | `abschluss/` | Projektpräsentationen und Abschluss |

`kompilieren-make/index.html` bleibt als Weiterleitung auf die Makefile-Unterseite
erhalten. Es gibt kein separates Makefiles-Kapitel mehr. Weitere ältere
Themenordner bleiben vorerst bestehen, sind aber nicht Teil der Kapitelkette.

`kurs.css` enthält die gemeinsame Gestaltung. Links und Stylesheets sind relativ,
damit die Website auch unter dem GitHub-Pages-Projektpfad funktioniert.

## Gemeinsame Arbeit mit Branches

- `feature/kursstruktur`: zentrale Übersicht, Themenordner und gemeinsame Gestaltung.
- `feature/start`: Inhalte im Ordner `start/` bearbeiten.
- `feature/variablen`: Inhalte im Ordner `variablen/` bearbeiten.

Die beiden Themenbranches starten vom gleichen Kursstruktur-Commit. Jeder Branch
enthält die vollständige Website. Die Arbeit konzentriert sich auf den zugehörigen
Themenordner. Die gemeinsame Struktur wird zunächst über einen Pull Request nach
`main` übernommen. Danach können Kapiteländerungen ebenfalls über Pull Requests
nach `main` zusammengeführt werden.

Weitere Themenbranches bei Arbeitsbeginn aus dem dann aktuellen `main` erstellen.
Änderungen an `uebersicht.html` oder `kurs.css` miteinander abstimmen, da diese Dateien
von allen Kapiteln verwendet werden.

## Lokal ansehen und veröffentlichen

`uebersicht.html` im Browser öffnen. Die Website benötigt keinen Build-Schritt.
Für die vorhandene GitHub-Pages-Website bleibt die Veröffentlichung aus `main`
und `/(root)` vorgesehen. Ein Feature-Branch ist zunächst eine Arbeitsfassung.
Eine Übernahme in `main` erfolgt nach Prüfung der Änderungen.

## Was die Vorschau macht

Die Vorschau ist ausdrücklich **eine begrenzte Simulation, kein C-Compiler**.
Sie liest das kleine Gerüst, erzeugt dessen simulierte Ausgabe und vergleicht
die Ausgabe mit dem Ziel der Aufgabe. Unterschiedliche unterstützte
Schreibweisen können damit zum gleichen richtigen Ergebnis führen.

Unterstützt werden `#include <stdio.h>`, `int main(void)` bzw. `int main()`,
initialisierte `int`-Variablen, Zuweisungen, dezimale Ganzzahlen, Klammern,
`+`, `-`, `*`, `/`, `%`, `printf` mit einfachen Texten, `%d` und `%%`,
die Escape-Sequenzen `\n`, `\r`, `\t`, `\\`, `\"`, Kommentare und `return`.
Die Simulation begrenzt Zahlen auf vorzeichenbehaftete 32-Bit-Werte.
Ganzzahldivision schneidet Richtung null ab. Division durch null und
Überläufe ergeben Hinweise statt einer Ausgabe.

Gültiger C-Code außerhalb dieses kleinen Umfangs kann von der Vorschau
abgelehnt werden. Daher ist die Rückmeldung kein vollständiger Nachweis,
dass ein Programm in C korrekt ist. Ein echter Compiler kann später
eingebunden werden, wenn wir uns dafür entscheiden.

Der eingegebene Code bleibt im aktuellen Browserfenster. Er wird nicht
übertragen, dauerhaft gespeichert oder zwischen Personen synchronisiert.
Änderungen an der Website können gemeinsam im Repository bearbeitet und
über Issues oder Pull Requests besprochen werden.

