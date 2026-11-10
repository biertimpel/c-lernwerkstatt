#ifndef TRESORRAUB_H
#define TRESORRAUB_H
#include <SDL.h>

/* Anschluss zwischen Programmgeruest (programm.c) und Ihrem Code
   (tresorraub.c). Beide Funktionen schreiben Sie in tresorraub.c. */

/* Wird bei "./programm --text" genau einmal aufgerufen, ohne Fenster.
   Erfolg: 0. Fehler: negativer Wert. */
int text_ausgeben(void);

/* Wird bei "./programm" fuer jedes Bild aufgerufen. Das Geruest loescht das
   Bild vorher und zeigt es danach an. Renderer wird nur geliehen.
   Erfolg: 0. Fehler: negativer Wert, SDL-Fehlermeldung erhalten. */
int zeichne(SDL_Renderer *renderer);

#endif
