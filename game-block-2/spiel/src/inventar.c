#include "inventar.h"
#include <stdio.h>
#include <stdlib.h>

const char *gegenstand_name(Gegenstand typ) {
    switch (typ) {
        case SCHLUESSEL: return "Museumsschluessel";
        case BEUTE: return "Tresorbeute";
        case MUENZE: return "Sammlermuenze";
        default: return NULL;
    }
}

bool inventar_hat(const Spieler *spieler, Gegenstand typ) {
    for (const Inventarknoten *k = spieler->inventar; k; k = k->naechster) {
        if (k->typ == typ) return true;
    }
    return false;
}

size_t inventar_anzahl(const Spieler *spieler) {
    size_t n = 0;
    for (const Inventarknoten *k = spieler->inventar; k; k = k->naechster) n++;
    return n;
}

int inventar_hinzufuegen(Spieler *spieler, Gegenstand typ) {
    const char *name = gegenstand_name(typ);
    if (!name || inventar_hat(spieler, typ)
        || inventar_anzahl(spieler) >= INVENTAR_LIMIT) return FEHLER_ARGUMENT;
    Inventarknoten *neu = malloc(sizeof *neu);
    if (!neu) return FEHLER_SPEICHER;
    neu->typ = typ;
    int n = snprintf(neu->name, sizeof neu->name, "%s", name);
    if (n < 0 || (size_t)n >= sizeof neu->name) {
        free(neu);
        return FEHLER_FORMAT;
    }
    neu->naechster = spieler->inventar;
    spieler->inventar = neu;
    return OK;
}

bool inventar_entfernen(Spieler *spieler, Gegenstand typ) {
    Inventarknoten **stelle = &spieler->inventar;
    while (*stelle) {
        Inventarknoten *k = *stelle;
        if (k->typ == typ) {
            *stelle = k->naechster;
            free(k);
            return true;
        }
        stelle = &k->naechster;
    }
    return false;
}

void inventar_freigeben(Spieler *spieler) {
    while (spieler->inventar) {
        Inventarknoten *alt = spieler->inventar;
        spieler->inventar = alt->naechster;
        free(alt);
    }
}
