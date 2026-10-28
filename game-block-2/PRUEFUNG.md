# Prüfung des Prototyps

Stand: 28. Oktober 2026. Die folgenden Angaben beziehen sich auf die gelieferte Grundversion.

## Tatsächlich ausgeführt

- Alle sieben C-Dateien mit `-std=c17 -Wall -Wextra -Wpedantic -Werror` übersetzt und gelinkt: ohne Warnungen und Fehler.
- Standardsuite `make test`: **643 Prüfungen bestanden**.
- Instrumentierte Suite `make test-alloc`: **746 Prüfungen bestanden**, einschließlich der Standardsuite. Speicherreservierungen wurden gezielt zum Scheitern gebracht. Erzeugen, Aufnehmen und Laden geben Fehler weiter und räumen bereits angelegte Daten auf. Nach den geprüften Lebenszyklen sind keine erfassten Anwendungs-Speicherblöcke offen.
- AddressSanitizer und UndefinedBehaviorSanitizer: Standardsuite bestanden. Die zusätzliche automatische LeakSanitizer-Suche musste wegen eingeschränkter Prozessinformationen der Prüfungsumgebung deaktiviert werden. Die gesonderte Reservierungs-/Freigabeinstrumentierung wurde dafür ebenfalls ausgeführt.
- Acht Prüfungen des tatsächlichen Konsolenprogramms: Sieg, Entdeckung, Zeitablauf, ungültige und überlange Eingaben, fehlerhafte Startoptionen, fehlender Spielstand, Speichern und anschließender Neustart aus der Datei, Diagnoseausgaben.
- **207 Zustandsvergleiche** zwischen dem JavaScript-Spielmodell und dem nativen C-Modell: sieben gezielte Abläufe und 200 reproduzierbare zufällige Befehlsfolgen.
- Austausch des gemeinsamen Spielstandformats zwischen Browser-Modell und C-Fassung geprüft.

Die gezielten Tests umfassen unter anderem:

- Gewinnroute in 32 Zügen und Bonusroute in 48 Zügen.
- Erkennung genau auf der Radiusgrenze und Sicherheit direkt außerhalb.
- Verschlossene Tür ohne Schlüssel: kein Zug und keine Positionsänderung.
- Aufnahme, Verwendung und Entfernung des Schlüssels.
- Beute und Münze nach Speichern und Laden.
- Sieg exakt im 60. Zug.
- Keine weiteren Bewegungen nach einem Spielende.
- Fehlende, abgeschnittene, widersprüchliche und unbekannt versionierte Spielstände.
- Sehr große Zahlen, ungültige Koordinaten, zusätzliche Daten und doppelte Gegenstände.
- Vorhandener Spielzustand bleibt bei einem Ladefehler erhalten.

## Aktualisierung: Piktogramme

Die Browser-Vorschau verwendet eigene SVG-Piktogramme. Glücklicher und trauriger Spielerzustand, böse Wächter, Türzustände und Gefahrenfelder ohne Textzeichen wurden im JavaScript-Modell geprüft. Die SVGs wurden zusätzlich in einer Spielfeld-Kontaktansicht gerendert und visuell kontrolliert. Die 207 Vergleiche mit dem C-Modell wurden nach der Änderung erneut bestanden. Die C-Quellen und Spielregeln wurden nicht geändert.

Diese Prüfung ersetzt keinen vollständigen Test der Browseroberfläche; die unten genannte Einschränkung bleibt bestehen.

## Selbst nachprüfen

Im Ordner `spiel/`:

```sh
make
make test
make test-alloc
```

Optional mit einem Compiler, der die entsprechenden Sanitizer unterstützt:

```sh
make sanitize
```

In der eingeschränkten Erstellungsumgebung wurde dafür `ASAN_OPTIONS=detect_leaks=0 make sanitize` verwendet. Auf einem eigenen geeigneten System kann die zusätzliche Leakerkennung aktiv bleiben.

## Grenzen der Prüfung

Die C-Fassung wurde unter Linux mit dem vorhandenen Compiler ausgeführt. Ein Test auf einem tatsächlichen macOS- oder Windows-Rechner wurde nicht durchgeführt. Die Startdateien sind vorbereitet; auf diesen Systemen muss ein passender C-Compiler vorhanden sein.

Das JavaScript-Modell wurde unter Node.js geprüft. Eine echte Browser-Ausführung samt visueller Desktop-/Mobilprüfung konnte hier mangels installiertem Browser nicht durchgeführt werden. Die HTML-Struktur und lokalen Verweise wurden geprüft. Der erste praktische Sichtcheck von Anzeige, Tastaturbedienung und Dateidialogen bleibt deshalb Teil eurer gemeinsamen Durchsicht.

Die Tests prüfen Softwareverhalten. Es gab noch keinen Pilotversuch mit Studierenden und keine Messung der benötigten Bearbeitungszeit. Der Kapitelplan und die möglichen Erweiterungen sind keine verbindliche Zeitplanung oder fertige Lernstandscheck-II-Aufgabe.
