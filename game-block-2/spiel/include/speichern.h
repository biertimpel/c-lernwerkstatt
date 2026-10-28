#ifndef SPEICHERN_H
#define SPEICHERN_H
#include "spiel.h"
int spiel_speichern(const Spiel *spiel, const char *pfad);
/* Bei einem Fehler bleibt das vorhandene Spiel unveraendert. */
int spiel_laden(Spiel *spiel, const char *pfad);
#endif
