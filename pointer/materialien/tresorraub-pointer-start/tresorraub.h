#ifndef TRESORRAUB_H
#define TRESORRAUB_H

#include <SDL.h>
#include <stddef.h>

/* Aufrufe des mitgelieferten Geruests in tresorraub.c. */
int text_ausgeben(void);
int zeichne(SDL_Renderer *renderer);
void spiel_starten(void);
void taste_gedrueckt(SDL_Keycode taste);
int spiel_status(char ziel[], size_t groesse);
void aufraeumen(void);

#endif
