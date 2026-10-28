#ifndef INVENTAR_H
#define INVENTAR_H
#include "spiel.h"
const char *gegenstand_name(Gegenstand typ);
bool inventar_hat(const Spieler *spieler, Gegenstand typ);
size_t inventar_anzahl(const Spieler *spieler);
int inventar_hinzufuegen(Spieler *spieler, Gegenstand typ);
bool inventar_entfernen(Spieler *spieler, Gegenstand typ);
void inventar_freigeben(Spieler *spieler);
#endif
