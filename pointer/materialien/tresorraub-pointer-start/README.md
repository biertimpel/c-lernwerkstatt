# Tresorraub: Pointer und Datenaustausch

Voraussetzung ist SDL2 wie im Kapitel „Manpages“. In diesem Ordner:

    make
    ./programm --text    # Versuche ohne Fenster
    ./programm           # Spielfeld im Fenster; Escape beendet es

Unter Windows (MSYS2) heißt die ausführbare Datei programm.exe.

Sie arbeiten in tresorraub.c. programm.c, tresorraub.h, das Makefile
und die Piktogramme werden fertig mitgegeben. Das Gerüst ruft Ihre
Funktionen zum Spielstart, bei Tastendrücken und zum Zeichnen auf.
Die Statuszeile erscheint im Fenstertitel.

Wenn Sie mit Ihrem eigenen Stand aus „Datenstrukturen“ weiterarbeiten:

1. Sichern Sie Ihre tresorraub.c. Übernehmen Sie programm.c und
   tresorraub.h aus diesem Ordner.
2. Legen Sie unterhalb Ihres Strukturtyps eine Variable für den laufenden
   Spielstand an, wie static Spielstand spielstand im Startmaterial.
   Übernehmen Sie spiel_starten, taste_gedrueckt, spiel_status und
   aufraeumen aus der mitgelieferten tresorraub.c.
3. spiel_starten legt den laufenden Spielstand einmal an. spiel_status
   schreibt dessen Position und Zugzahl in das übergebene Zeichenarray.
   Verwenden Sie dafür Ihre vorhandene Statuszeile und passen Sie
   Funktions- und Feldnamen an Ihr Programm an. Falls Sie noch keine
   passende Funktion haben, nutzen Sie spielstand_lesen aus dem
   Startmaterial als Ausgangspunkt. aufraeumen bleibt vorerst leer.

Alternativ können Sie direkt mit der mitgelieferten tresorraub.c
weiterarbeiten. Sie zeigt das Ausgangsspielfeld und enthält noch keine
Bewegungslogik. Die Plätze für Ihre Ergänzungen stehen in der Aufgabe.
