# Startmaterial: Vom Problem zum Programm

Dieses Material gibt keine fachliche Architektur und keine fertige Lösung vor.
Entwickeln Sie Ihre eigenen Funktionen in `programm.c`. Verwenden Sie diesen
Ordner und das Makefile über das gesamte Kapitel weiter.

## Übersetzen und starten

Voraussetzungen: ein C-Compiler und Make.

```sh
make
./programm
```

Das leere Gerüst beendet sich zunächst ohne Ausgabe. Ergänzen Sie Ihre eigene
Implementierung. Speichern und übersetzen Sie nach Änderungen erneut.

## Eingabebaustein

`lese_ganzzahl()` aus `eingabe.h` liefert eine Zahl aus der nächsten Zeile.
Jede Zahl muss allein auf ihrer Zeile stehen; umgebender Leerraum ist erlaubt.
Fachlich ungültige Temperaturen wie 99 werden unverändert geliefert.
Fehlende Zeilen, leere Zeilen, ungültige Schreibweisen, zu lange Zeilen und
Zahlen außerhalb des int-Bereichs führen zu einer Meldung auf stderr und
Programmende mit Status 1. Es gibt keine Aufforderung zur erneuten Eingabe.

`eingabe.c` ist fertige technische Infrastruktur. Seine interne Umsetzung mit
Arrays, Pointern und Headern ist hier kein vorausgesetzter Lernstoff. Das
Makefile verbindet den Baustein mit Ihrem eigenen Programm.

## Weiterentwicklung und Refactoring

Die Dateien begleiten Ihren eigenen Entwurf über das gesamte Kapitel:

- Version 1 verarbeitet eine einzelne Temperatur.
- Version 2 liest zuerst die Anzahl (0 bis 100) und danach die Messungen,
  jeweils eine ganze Zahl pro Zeile. Zusätzliche Zeilen nach der erwarteten
  Anzahl werden nicht ausgewertet.
- Nach der getesteten Version 2 verbessern Sie eine selbst gewählte Stelle
  durch Refactoring. Anforderungen, Eingaben, Ausgaben und Rückgabestatus
  bleiben dabei unverändert. Sichern Sie den Stand und wiederholen Sie Ihre
  bisherigen Tests mit denselben Erwartungen.
- Erst anschließend ergänzen Sie den gewünschten Temperaturbereich. Dieser
  verändert nicht die technische Plausibilitätsregel. Die Wunschgrenzen
  konfigurieren Sie in Ihrer Implementierung, nicht über zusätzliche
  Eingabezeilen.

Refactoring ist eine strukturelle Verbesserung, keine neue Funktionalität
und keine Fehlerkorrektur. Die Dateien geben dafür keine bestimmte
Funktionseinteilung oder Musterlösung vor.

## Freiwilliger Exkurs: Beispieleingaben aus Dateien

Dieser Exkurs hat einen höheren Schwierigkeitsgrad. Dateieingabe und
systematisches Testen werden später im Kurs ausführlicher behandelt.
Wenn Sie das jetzt noch stark herausfordert, ist das völlig in Ordnung:
Geben Sie Ihre Testwerte weiterhin von Hand ein und vergleichen Sie die
Ergebnisse. Die Prüfung des Programmverhaltens gehört zur Aufgabe;
Dateiumleitung und automatisierte Tests sind dafür nicht erforderlich.

Bei der folgenden Umleitung liefert die Datei dieselben Eingabezeilen wie
die Tastatur. Sie müssen dafür keine Dateizugriffe in C implementieren:

```sh
./programm < tests/v1-gueltig.txt
echo $?
```

Dateien mit `v1-` passen zur Einzelmessung auf Seite 4. Dateien mit `reihe-`
passen erst zur Messreihenauswertung auf Seite 5. Dort steht die Anzahl der
Messungen in der ersten Zeile. Die erwarteten Ergebnisse sind in
`TESTFAELLE.md` beschrieben. Ergänzen Sie eigene Tests und dokumentieren Sie
Ihre Ausgabeformatierung.
