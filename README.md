# C-Lernwerkstatt

Statische Kurswebsite für den C-Programmierkurs im Wintersemester 2026/27.
Start: **19. Oktober 2026**. Abschluss: **bis 11. Februar 2027**.
Pro Woche finden zwei Termine mit jeweils 90 Minuten statt. Der Abschluss ist unbenotet.

## Einstieg und Materialstand

Die zentrale Übersicht liegt in **`uebersicht.html`**. Dort sind alle vereinbarten
Termine und Projektphasen mit ihren Themen und Lernzielen erreichbar.
Die Weihnachtspause ist als Planungsannahme markiert. Die beiden konkreten
Wochentage und endgültigen Kursdaten werden im Kurs bekanntgegeben.

Die erste Programmieraufgabe ist vorhanden. Alle weiteren Seiten sind zunächst
Kapitelgerüste mit Lernzielen sowie Bereichen für Einführung, Aufgaben und Hinweise.
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

Jeder weitere Themenordner enthält zunächst eine `index.html`. Materialien und
zusätzliche Aufgaben können später direkt im jeweiligen Ordner ergänzt werden.

| Termin / Woche | Ordner | Thema |
| --- | --- | --- |
| 1.1 | `start/` | Start |
| 1.2 | `variablen/` | Variablen, Datentypen und Berechnungen |
| 2.1 | `kontrollfluss/` | Bedingungen und Schleifen |
| 2.2 | `randbedingungen/` | Komplexerer Kontrollfluss und Randbedingungen |
| 3.1 | `funktionen-debugging/` | Funktionen, Programmzerlegung und Debugging |
| 3.2 | `lernstand-1/` | Grundlagencheck |
| 4.1 | `arrays-zeichenketten/` | Manpages, Arrays, Zeichenketten und Listenmodelle |
| 4.2 | `pointer/` | Pointer und Datenaustausch |
| 5.1 | `verkettete-listen/` | Verkettete Listen mit Pointern umsetzen |
| 5.2 | `speicheraufbau/` | Speicheraufbau: Text, Data, Heap und Stack |
| 6.1 | `dynamischer-speicher/` | Dynamischer Speicher und Strukturen |
| 6.2 | `header-quellen/` | Header, Quelldateien und Schnittstellen |
| 7.1 | `kompilieren-make/` | Kompilieren, Linken und Makefiles |
| 7.2 | `modularisierung-praxis/` | Programmzerlegung in Dateien: praktische Aufgabe |
| 8.1 | `wiederholung/` | Wiederholung |
| 8.2 | `lernstand-2/` | Umfassender Lernstandscheck |
| 9 | `projektplanung/` | Projekt planen |
| 10 | `projekt-grundumfang/` | Technischen Projektgrundumfang umsetzen |
| 11–12 | `projekt-verbesserung/` | Projekt verbessern, erweitern und prüfen |
| 13 | `vortragsvorbereitung/` | Implementierung abschließen und Vortrag vorbereiten |
| 14–15 | `abschluss/` | Projektpräsentationen und Abschluss |

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

