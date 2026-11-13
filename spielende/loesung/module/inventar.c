/* Inventar: einfach verkettete Liste von Pointern auf Gegenstaende. Das
   Inventar besitzt seine Eintraege, aber nicht die Gegenstaende. */
#include "inventar.h"
#include <stdio.h>
#include <stdlib.h>

size_t inventar_anzahl(const Inventar *inventar) {
    size_t anzahl = 0;
    for (const Eintrag *e = inventar->erster; e != NULL; e = e->naechster) {
        anzahl++;
    }
    return anzahl;
}

/* Erster Gegenstand dieser Art im Inventar, sonst NULL. */
Objekt *inventar_finden(const Inventar *inventar, const Objektart *art) {
    for (const Eintrag *e = inventar->erster; e != NULL; e = e->naechster) {
        if (e->gegenstand->art == art) {
            return e->gegenstand;
        }
    }
    return NULL;
}

/* Nimmt einen Gegenstand vorne auf. Ergebnis: AUFGENOMMEN, INVENTAR_VOLL
   (Inventar unveraendert) oder FEHLER_SPEICHER (ebenfalls unveraendert). */
Aufnahme inventar_aufnehmen(Inventar *inventar, Objekt *gegenstand, size_t hoechstens) {
    if (inventar_anzahl(inventar) >= hoechstens) {
        return INVENTAR_VOLL;
    }
    Eintrag *neu = malloc(sizeof(*neu));
    if (neu == NULL) {
        return FEHLER_SPEICHER;
    }
    neu->gegenstand = gegenstand;
    neu->naechster = inventar->erster;
    inventar->erster = neu;
    return AUFGENOMMEN;
}

void inventar_ausgeben(const Inventar *inventar) {
    printf("Inventar (%zu):", inventar_anzahl(inventar));
    if (inventar->erster == NULL) {
        printf(" leer");
    }
    for (const Eintrag *e = inventar->erster; e != NULL; e = e->naechster) {
        printf(" %s", e->gegenstand->art->name);
    }
    printf("\n");
}

/* Entfernt den Eintrag dieses Gegenstands und gibt den Eintrag frei.
   Ergebnis: true, wenn er vorhanden war. */
bool inventar_entfernen(Inventar *inventar, const Objekt *gegenstand) {
    Eintrag *vorher = NULL;
    Eintrag *aktuell = inventar->erster;
    while (aktuell != NULL && aktuell->gegenstand != gegenstand) {
        vorher = aktuell;
        aktuell = aktuell->naechster;
    }
    if (aktuell == NULL) {
        return false;
    }
    if (vorher == NULL) {
        inventar->erster = aktuell->naechster;
    } else {
        vorher->naechster = aktuell->naechster;
    }
    free(aktuell);
    return true;
}

void inventar_leeren(Inventar *inventar) {
    Eintrag *aktuell = inventar->erster;
    while (aktuell != NULL) {
        Eintrag *naechster = aktuell->naechster;   /* merken, bevor free */
        free(aktuell);
        aktuell = naechster;
    }
    inventar->erster = NULL;
}
