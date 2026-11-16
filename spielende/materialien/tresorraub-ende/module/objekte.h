/* Objekte des Tresorraubs: Startzustand und Hilfsfunktionen. Die
   Objektarten und ihre Methoden sind in objekte.c gekapselt. */
#ifndef OBJEKTE_H
#define OBJEKTE_H

#include "spiel.h"

/* Fuellt *s mit dem Startzustand nach den Regeln. */
void startzustand(Spielstand *s, const Regeln *regeln);

/* Erstes Objekt auf dem Feld p, sonst NULL. */
const Objekt *objekt_auf(const Spielstand *s, Punkt p);

/* Steht auf p ein Waechter? */
bool waechter_auf(const Spielstand *s, Punkt p);

/* Gegenstand, den die Figur auf p aufnehmen koennte (auch im Tresor), sonst NULL. */
const Objekt *gegenstand_auf(const Spielstand *s, Punkt p);

/* Zeichen und Piktogramm eines Objekts, je nach Zustand. */
char objekt_zeichen(const Objekt *o);
Piktogramm objekt_bild(const Objekt *o);

#endif
