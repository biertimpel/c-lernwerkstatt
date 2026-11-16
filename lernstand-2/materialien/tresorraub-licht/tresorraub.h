#ifndef TRESORRAUB_H
#define TRESORRAUB_H

#include <SDL.h>
#include <stddef.h>

/* Anschluss zwischen Programmgeruest (programm.c) und Ihrem Code
   (tresorraub.c). Das Geruest ruft diese Funktionen auf; Sie schreiben sie
   in tresorraub.c. Fuer Funktionen, die ein Kapitel noch nicht braucht,
   enthaelt das Startmaterial Platzhalter.
   Rueckgabe int: 0 bei Erfolg, negativer Wert bei einem Fehler, nach dem
   das Programm nicht sinnvoll weiterlaufen kann. Das Geruest beendet das
   Programm dann. */

/* Bei "./programm --text" genau einmal, ohne Fenster. */
int text_ausgeben(void);

/* Im Fenster genau einmal, bevor Tasten verarbeitet und Bilder gezeichnet
   werden. Legt den laufenden Spielstand an. */
int spiel_starten(void);

/* Im Fenster bei jedem Tastendruck einmal (ausser Escape, das beendet das
   Programm). Gedrueckt Halten zaehlt nicht als neuer Tastendruck. */
int taste_gedrueckt(SDL_Keycode taste);

/* Im Fenster fuer jedes Bild. Das Geruest loescht das Bild vorher und zeigt
   es danach an. Der Renderer wird nur geliehen. Bei einem SDL-Fehler bleibt
   die SDL-Fehlermeldung erhalten. */
int zeichne(SDL_Renderer *renderer);

/* Im Fenster bei jedem Durchlauf der Spielschleife, also etwa 60-mal pro
   Sekunde, auch ohne Tastendruck: nach den Tasten, vor dem Zeichnen.
   jetzt_ms ist die Zeit seit dem Start von SDL in Millisekunden
   (SDL_GetTicks64). Hier gehoert Spiellogik hin, die von der Zeit abhaengt.
   Rueckgabe: 0, wenn sich nichts geaendert hat; 1, wenn sich der Spielstand
   geaendert hat (das Geruest erneuert dann den Fenstertitel); negativ bei
   einem Fehler. Braucht SDL 2.0.18 oder neuer. */
int spiel_aktualisieren(Uint64 jetzt_ms);

/* Im Fenster nach dem Start, nach jedem Tastendruck und wenn
   spiel_aktualisieren eine Aenderung meldet. Schreibt die
   Statuszeile fuer den Fenstertitel als Zeichenkette nach ziel; groesse ist
   die Groesse von ziel in Bytes. */
int spiel_status(char ziel[], size_t groesse);

/* Genau einmal vor Programmende, in jedem Modus und auch nach einem
   Fehler. Gibt alles frei, was Ihr Programm dann noch besitzt. */
void aufraeumen(void);

#endif
