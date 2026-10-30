# Referenzfälle

Dies sind fachliche Vergleichswerte, keine vorgeschriebenen Ausgabezeilen.
Das leere Startgerüst implementiert die Aufgaben noch nicht. Die Fälle gelten
erst nach Ihrer jeweiligen Erweiterung.

Technisch plausible Messwerte sind ganzzahlige Temperaturen von -40 bis 80
einschließlich. Eine lesbare ganze Zahl außerhalb dieses Bereichs ist eine
fachlich ungültige Messung, kein technischer Eingabefehler.
Nach dem zusätzlichen Änderungsauftrag liegt der gewünschte Bereich bei
18 bis 24 einschließlich.
Ungültige Temperaturen gehen weder in die Statistik noch in die beiden
Wunschbereich-Zähler ein.

## Einzelmessung (Version 1)

| Datei | Erwartung | Status |
| --- | --- | --- |
| `tests/v1-gueltig.txt` | 21, gültig | 0 |
| `tests/v1-ungueltig.txt` | 81, ungültige Messung | 0 |
| `tests/v1-text.txt` | Technischer Eingabefehler | 1 |

## Messreihe (Version 2, später zusätzlich Wunschbereich)

| Datei | Gültig / ungültig | Min / Max / Mittel | Im / außerhalb Wunschbereich | Status |
| --- | --- | --- | --- | --- |
| `tests/reihe-gemischt.txt` | 5 / 1 | -5 / 80 / 27,6 | 3 / 2 | 0 |
| `tests/reihe-grenzen.txt` | 4 / 0 | -40 / 80 / 20,5 | 2 / 2 | 0 |
| `tests/reihe-negative.txt` | 3 / 0 | -30 / -10 / -20 | 0 / 3 | 0 |
| `tests/reihe-einzelwert.txt` | 1 / 0 | -5 / -5 / -5 | 0 / 1 | 0 |
| `tests/reihe-wunschgrenzen.txt` | 4 / 1 | 17 / 25 / 21 | 2 / 2 | 0 |
| `tests/reihe-leer.txt` | 0 / 0 | Nicht verfügbar | 0 / 0 | 0 |
| `tests/reihe-keine-gueltigen.txt` | 0 / 3 | Nicht verfügbar | 0 / 0 | 0 |
| `tests/reihe-fehlende-messung.txt` | Abbruch wegen fehlender zweiter Zeile mit Messwert | Kein vollständiger Bericht verlangt | — | 1 |
| `tests/reihe-anzahl-ungueltig.txt` | Konfigurationsfehler vor dem Lesen von Messwerten | Kein Bericht verlangt | — | 1 |

## Refactoring und Änderungsauftrag getrennt prüfen

Beim Refactoring der Version 2 bleiben alle bisherigen Erwartungen
unverändert. Vergleichen Sie auch Ihren bisherigen Meldungstext und
Rückgabestatus, nicht nur die Zahlen im Bericht. Die Wunschbereich-Spalten
werden erst beim anschließenden Änderungsauftrag benötigt. Danach müssen
die bisherigen Statistikwerte und Gültigkeitszähler weiterhin gleich sein.

Wählen Sie die Wunschgrenzen an der vorgesehenen Stelle in Ihrem eigenen
Code; sie stehen nicht in den Eingabedateien. Prüfen Sie zusätzlich einen
geänderten Wunschbereich und ungültige Konfigurationen (vertauschte Grenzen
oder eine Grenze außerhalb von -40 bis 80). Bei ungültiger Konfiguration
wird Status 1 vor der Auswertung erwartet.

## Verwendung der Testdaten ist freiwillig

Der Dateieingabe-Exkurs hat einen höheren Schwierigkeitsgrad. Sie können
dieselben Werte von Hand eingeben. Für die Messreihe geben Sie zuerst die
Anzahl und danach jeden Messwert in einer eigenen Zeile ein. Dateieingabe
und systematisches Testen werden später vertieft; automatisierte Tests
sind hier keine Voraussetzung.

Prüfen Sie zusätzlich eigene Grenzen und dieselbe Reihe in anderer
Reihenfolge. Diese kleine Sammlung ersetzt keinen eigenen Testplan.
