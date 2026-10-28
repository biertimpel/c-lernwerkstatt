# Tresorraub – Zielbild für Block 2

Planungsprototyp vom 28. Oktober 2026 für Jonathan und Larissa.

## Sofort ausprobieren

1. Die gesamte ZIP entpacken.
2. `START_HIER.html` per Doppelklick im Browser öffnen.
3. Mit den Schaltflächen, W/A/S/D oder den Pfeiltasten spielen.

Die Vorschau funktioniert offline und benötigt keine Installation. Es gibt keine externen Bilder, Schriftarten, Bibliotheken oder Netzwerkaufrufe. Rot und Grün sind nicht die einzigen Unterscheidungsmerkmale: Überwachte Felder sind rötlich hinterlegt und schraffiert. Die Browser-Vorschau verwendet eigene Piktogramme für Spielfigur, Wächter, Gegenstände, Tür, Tresor und Ausgang. Die Spielfigur lächelt und wird bei Entdeckung traurig. Die C-Terminalfassung verwendet weiterhin die in ihrer Hilfe erklärten Textzeichen.

## Die C-Referenz starten

Im Unterordner `spiel/` befinden sich sieben C-Dateien, sechs Header, ein Makefile und Tests. Voraussetzung ist ein C-Compiler, zum Beispiel GCC oder Clang. Es werden ausschließlich Funktionen der C-Standardbibliothek verwendet.

**macOS / Linux:** Im Terminal in den Ordner `spiel` wechseln und `sh starten.sh` ausführen. Das Skript übersetzt die Quellen und startet das Spiel. Alternativ `make` und anschließend `./tresorraub`.

**Windows mit GCC im Suchpfad:** `spiel/starten_windows.bat` doppelklicken. Das Skript übersetzt und startet das Programm. Bei einer IDE können alle C-Dateien aus `src/` in ein Konsolenprojekt aufgenommen werden. `include/` wird als Header-Suchpfad hinzugefügt. Nur `src/main.c` enthält den Programmeinstieg. `tests/test_spiel.c` gehört ausschließlich zum Testprogramm.

Die C-Fassung liest jeweils einen Buchstaben mit anschließendem Enter:

| Befehl | Bedeutung |
|---|---|
| W / A / S / D | Oben / links / unten / rechts |
| Z | Einen Zug warten |
| I | Inventar anzeigen |
| P | Spielstand speichern |
| L | Spielstand laden |
| H | Hilfe |
| Q | Beenden |

Standarddatei: `spielstand.sav` im aktuellen Arbeitsverzeichnis. Das erneute Speichern überschreibt diese Datei. Einen anderen Pfad wählt `./tresorraub --save-file datei.sav`. `./tresorraub --load datei.sav` startet aus einem gespeicherten Spiel. `--debug` aktiviert zusätzliche Diagnoseausgaben auf stderr.

Die Browser-Vorschau lädt und speichert dasselbe Textformat. Einen heruntergeladenen Spielstand kann man beispielsweise in den C-Spielordner kopieren und mit `./tresorraub --load spielstand.sav` öffnen. Der Browser lädt Dateien über den Dateiauswahldialog und bietet Speichern als Download an.

## Was enthalten ist

- `START_HIER.html` und `vorschau/spiel.js`: sofort spielbare Browser-Vorschau.
- `SPIELREGELN.md`: verbindliche Regeln dieses Prototyps und eine nachvollziehbare Gewinnroute.
- `KAPITELPLAN.md`: grobe Zuordnung aller Überkapitel, einschließlich der sechs Unterkapitel „Vom Problem zum Programm“.
- `TECHNIK.md`: Architektur, objektorientierte Umsetzung in C und Spielstandformat.
- `ABSTIMMUNG.md`: Erklärung der Unterrichtsidee, Gestaltungsspielraum und offene Entscheidungen.
- `PRUEFUNG.md`: tatsächlich ausgeführte Prüfungen und ihre Grenzen.
- `spiel/`: vollständige C-Referenz samt Makefile und Tests.

Alle Planungsunterlagen sind zusätzlich lesefreundlich in `LEITFADEN.html` zusammengefasst. Diese Datei lässt sich ebenfalls direkt im Browser öffnen.

## Einordnung

Dieses Paket ist ein unverbindliches Zielbild zur gemeinsamen Besprechung. Es enthält keine fertigen Kapitelaufgaben und keine studentische Projektvorgabe. Die C-Referenz zeigt einen möglichen Endstand nach Block 2. Die Lernenden würden die Bausteine schrittweise entwickeln und benötigen zu Beginn nicht die gesamte Architektur.

Der bereits besprochene **Lernstandscheck I** mit dem dreistelligen Codeschloss bleibt eine eigenständige Aufgabe aus Block 1. **Lernstandscheck II** würde später diese Spielgrundversion erweitern. Die beiden Vorhaben werden nicht vermischt.

Es wurden keine Änderungen an der Kurswebsite oder an GitHub-Branches vorgenommen.
