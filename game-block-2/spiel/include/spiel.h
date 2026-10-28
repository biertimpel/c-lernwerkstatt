#ifndef SPIEL_H
#define SPIEL_H

#include <stdbool.h>
#include <stddef.h>

enum { BREITE = 15, HOEHE = 9, ZUGLIMIT = 60, INVENTAR_LIMIT = 3 };
typedef enum { OK = 0, BLOCKIERT = 1, ENDE_EINGABE = 2,
    FEHLER_SPEICHER = -1, FEHLER_EINGABE = -2, FEHLER_DATEI = -3,
    FEHLER_FORMAT = -4, FEHLER_ARGUMENT = -5 } Ergebnis;
typedef enum { LAEUFT, GEWONNEN, ENTDECKT, ZEIT_ABGELAUFEN } Phase;
typedef struct { int x, y; } Punkt;
typedef enum { SCHLUESSEL = 1, BEUTE = 2, MUENZE = 3 } Gegenstand;

typedef struct Inventarknoten {
    Gegenstand typ;
    char name[32];
    struct Inventarknoten *naechster;
} Inventarknoten;

typedef struct {
    Punkt position;
    Inventarknoten *inventar;
} Spieler;

struct Objekt;
typedef struct Objektknoten {
    struct Objekt *objekt;
    struct Objektknoten *naechster;
} Objektknoten;

typedef struct Spiel {
    char karte[HOEHE][BREITE + 1];
    Spieler spieler;
    Objektknoten *objekte;
    int zuege;
    Phase phase;
    char meldung[192];
} Spiel;

bool punkt_gleich(Punkt a, Punkt b);
bool punkt_gueltig(Punkt p);
int manhattan_abstand(Punkt a, Punkt b);
int spiel_neu(Spiel *spiel);
void spiel_freigeben(Spiel *spiel);
int spiel_bewegen(Spiel *spiel, int dx, int dy);
int spiel_warten(Spiel *spiel);
bool spiel_ueberwacht(const Spiel *spiel, Punkt p);
bool spiel_betretbar(const Spiel *spiel, Punkt p);
char spiel_zeichen(const Spiel *spiel, Punkt p);

#endif
