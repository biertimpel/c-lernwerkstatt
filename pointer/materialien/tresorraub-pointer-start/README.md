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

Gerüst und Piktogramme sind dieselben wie im Kapitel „Datenstrukturen“.
Sie können deshalb direkt mit Ihrer eigenen tresorraub.c aus
„Datenstrukturen“ weiterarbeiten. Sie enthält bereits alle Funktionen,
die das Gerüst aufruft. Die Platzhalter für spiel_starten,
taste_gedrueckt und spiel_status ersetzen Sie in der Aufgabe auf Seite 4.

Alternativ arbeiten Sie mit der mitgelieferten tresorraub.c weiter. Sie
zeigt das Ausgangsspielfeld und enthält noch keine Bewegungslogik. Was
Sie ergänzen, steht in den Aufgaben.
