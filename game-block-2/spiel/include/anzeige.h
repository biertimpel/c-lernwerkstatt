#ifndef ANZEIGE_H
#define ANZEIGE_H
#include "spiel.h"
int anzeige_spiel(const Spiel *spiel);
int anzeige_inventar(const Spiel *spiel);
int anzeige_text(const char *text);
int anzeige_hilfe(void);
#endif
