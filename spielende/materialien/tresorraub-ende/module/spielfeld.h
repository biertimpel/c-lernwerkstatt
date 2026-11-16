/* Spielfeld des Tresorraubs: Groesse, Punkte, Waechter, Grundriss. */
#ifndef SPIELFELD_H
#define SPIELFELD_H

#include <stdbool.h>
#include <stddef.h>

#define BREITE 15
#define HOEHE 9
#define ANZAHL_WAECHTER 2

typedef struct {
    int x;
    int y;
} Punkt;

typedef struct {
    Punkt position;
    int reichweite;
} Waechter;

bool punkt_gleich(Punkt a, Punkt b);

/* Rasterabstand: waagerechter plus senkrechter Abstand. */
int rasterabstand(Punkt a, Punkt b);

/* Liegt p in der Reichweite eines Waechters? Das Feld des Waechters zaehlt mit. */
bool ueberwacht(Punkt p, const Waechter waechter[], size_t anzahl);

/* Liegt p im Spielfeld? */
bool innerhalb(Punkt p);

/* Ist p im Grundriss eine Wand? p muss innerhalb liegen. */
bool ist_wand(Punkt p);

/* Liegt p innerhalb und ist keine Wand? */
bool begehbar(Punkt p);

#endif
