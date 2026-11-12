# Musterlösung Datei-Ein- und -Ausgabe

Eine mögliche Umsetzung auf Basis von Dynamischer Speicher und Listen,
Commit b21b114. Eigene Funktionszuschnitte sind gleichwertig. programm.c,
tresorraub.h und piktogramme/ werden unverändert aus dem Startmaterial benutzt.
Die Lösung wird auf keiner Seite verlinkt und ist nicht im Startmaterial-ZIP.

## Ausführen

    make
    make test
    ./programm --text
    ./programm

F5 speichert und F9 lädt spielstand.txt im Arbeitsverzeichnis. L bleibt Ablegen.
F5 und F9 verbrauchen keinen Zug. Ein intern behandelter Dateifehler lässt den
Callback mit 0 zurückkehren, sodass das Gerüst das Spiel weiterführt.

## Erwartungshorizont

- Seite 1: FILE * zeigt auf ein Verwaltungsobjekt, nicht auf Dateiinhalt.
  fprintf und fclose werden geprüft. Ein neues Öffnen mit w ersetzt Inhalt.
- Seite 2: Inventarnamen in Listenreihenfolge. Keine Pointerwerte speichern.
- Seite 3: Schreiben verändert den Spielstand nicht. Ein Fehler kann bei w
  eine unvollständige Zieldatei hinterlassen. Kein transaktionales Dateisystem.
- Seite 4: eigener neuer Zustand, eigene Einträge, bei Fehler nur diese freigeben.
  Bei Erfolg altes Inventar freigeben und Besitz am neuen Inventar übernehmen.
  Zahlen werden zunächst als begrenzte Wörter gelesen und mit strtol geprüft,
  damit auch Werte außerhalb int kontrolliert abgelehnt werden.
- Seite 5: start.txt, inventar.txt, abgelegt.txt akzeptieren. Alle ungueltig-*
  ablehnen. inventar.txt: (5,3), 23 Züge, Tür offen, Beute vor Muenze.
  abgelegt.txt: (5,3), 24 Züge, Tür offen, Inventar leer, Beute (5,2), Münze (5,4).

## Prüfung

89 Prüfungen: gültige und ungültige Dateien, Grenzen, doppelte/ unbekannte
Namen, widersprüchliche Zustände, Listenreihenfolge, eigene Adressen, Freigaben,
kontrolliert fehlgeschlagenes malloc (vor und nach einem neuen Eintrag),
fehlgeschlagenes Öffnen und Schließen, /dev/full unter Linux,
F5/F9 über die echte Tastenfunktion, Statuszeile und Darstellung per SDL2-
Software-Renderer. Alle Dateiverbindungen und Listeneinträge sind am Ende frei.

Geprüft mit GCC und -std=c17 -Wall -Wextra -Wpedantic -Werror.
Zusätzlich AddressSanitizer/UndefinedBehaviorSanitizer bestanden.
LeakSanitizer ist in der Prüfumgebung wegen ptrace nicht ausführbar. Dafür
zählt tests.c zusätzlich die eigenen malloc/free- und fopen/fclose-Aufrufe.
Clang ist hier nicht verfügbar; der zweite Compilercheck steht noch aus:

    make clean
    make CC=clang
    make test CC=clang

Für AddressSanitizer in dieser Umgebung:

    make clean
    make CFLAGS="-std=c17 -Wall -Wextra -Wpedantic -Werror -g -fsanitize=address,undefined"
    ASAN_OPTIONS=detect_leaks=0 make test
