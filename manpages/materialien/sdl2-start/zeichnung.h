#ifndef ZEICHNUNG_H
#define ZEICHNUNG_H
#include <SDL.h>

/* Renderer wird nur geliehen. Nicht selbst zerstoeren.
   Erfolg: 0. Fehler: negativer Wert, SDL-Fehlermeldung erhalten. */
int zeichne(SDL_Renderer *renderer);

#endif
