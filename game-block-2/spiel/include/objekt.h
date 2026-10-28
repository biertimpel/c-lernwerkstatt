#ifndef OBJEKT_H
#define OBJEKT_H
#include "spiel.h"

typedef enum { AUFHEBBAR, TUER, TRESOR, WAECHTER, AUSGANG } Objekttyp;
enum { ID_SCHLUESSEL, ID_TUER, ID_TRESOR, ID_MUENZE,
       ID_WAECHTER_LINKS, ID_WAECHTER_RECHTS, ID_AUSGANG, OBJEKT_ANZAHL };

typedef struct Objekt Objekt;
typedef struct {
    char (*zeichen)(const Objekt *objekt);
    int (*betreten)(Objekt *objekt, Spiel *spiel);
    void (*freigeben)(Objekt *objekt);
} Objekttabelle;

struct Objekt {
    int id;
    Objekttyp typ;
    Punkt position;
    bool aktiv;
    const Objekttabelle *methoden;
};

/* Die eingebettete Basis steht immer zuerst. Die eigenen Felder folgen. */
typedef struct { Objekt basis; Gegenstand gegenstand; } Aufhebbar;
typedef struct { Objekt basis; bool offen; Gegenstand schluessel; } Tuer;
typedef struct { Objekt basis; bool gepluendert; } Tresor;
typedef struct { Objekt basis; int radius; } Waechter;
typedef struct { Objekt basis; } Ausgang;

Objekt *objekt_erzeugen(int id);
Objekt *objekt_finden(const Spiel *spiel, int id);
Objekt *objekt_am_punkt(const Spiel *spiel, Punkt p);
int objekte_anlegen(Spiel *spiel);
void objekte_freigeben(Spiel *spiel);
#endif
