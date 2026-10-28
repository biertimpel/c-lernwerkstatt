#include "objekt.h"
#include "inventar.h"
#include <stdio.h>
#include <stdlib.h>

static void normal_freigeben(Objekt *objekt) { free(objekt); }
static char aufhebbar_zeichen(const Objekt *objekt) {
    const Aufhebbar *ding = (const Aufhebbar *)objekt;
    return ding->gegenstand == SCHLUESSEL ? 'k' : 'c';
}
static int aufhebbar_betreten(Objekt *objekt, Spiel *spiel) {
    Aufhebbar *ding = (Aufhebbar *)objekt;
    int ergebnis = inventar_hinzufuegen(&spiel->spieler, ding->gegenstand);
    if (ergebnis < 0) return ergebnis;
    objekt->aktiv = false;
    snprintf(spiel->meldung, sizeof spiel->meldung, "%s aufgenommen.",
             gegenstand_name(ding->gegenstand));
    return OK;
}
static char tuer_zeichen(const Objekt *objekt) {
    return ((const Tuer *)objekt)->offen ? '/' : 'D';
}
static int tuer_betreten(Objekt *objekt, Spiel *spiel) {
    Tuer *tuer = (Tuer *)objekt;
    if (tuer->offen) return OK;
    if (!inventar_entfernen(&spiel->spieler, tuer->schluessel)) {
        snprintf(spiel->meldung, sizeof spiel->meldung,
                 "Die Tuer ist zu. Finden Sie den Schluessel k.");
        return BLOCKIERT;
    }
    tuer->offen = true;
    snprintf(spiel->meldung, sizeof spiel->meldung,
             "Tuer geoeffnet. Der Schluessel wurde verwendet.");
    return OK;
}
static char tresor_zeichen(const Objekt *objekt) {
    return ((const Tresor *)objekt)->gepluendert ? 't' : 'T';
}
static int tresor_betreten(Objekt *objekt, Spiel *spiel) {
    Tresor *tresor = (Tresor *)objekt;
    if (tresor->gepluendert) return OK;
    int ergebnis = inventar_hinzufuegen(&spiel->spieler, BEUTE);
    if (ergebnis < 0) return ergebnis;
    tresor->gepluendert = true;
    snprintf(spiel->meldung, sizeof spiel->meldung,
             "Beute gesichert! Jetzt zurueck zum Ausgang E.");
    return OK;
}
static char waechter_zeichen(const Objekt *objekt) { (void)objekt; return 'W'; }
static int waechter_betreten(Objekt *objekt, Spiel *spiel) {
    (void)objekt; (void)spiel; return OK;
}
static char ausgang_zeichen(const Objekt *objekt) { (void)objekt; return 'E'; }
static int ausgang_betreten(Objekt *objekt, Spiel *spiel) {
    (void)objekt;
    if (!inventar_hat(&spiel->spieler, BEUTE)) {
        snprintf(spiel->meldung, sizeof spiel->meldung,
                 "Hier ist der Ausgang. Die Beute fehlt noch.");
    }
    return OK;
}

static const Objekttabelle AUFHEBBAR_METHODEN = {
    aufhebbar_zeichen, aufhebbar_betreten, normal_freigeben };
static const Objekttabelle TUER_METHODEN = {
    tuer_zeichen, tuer_betreten, normal_freigeben };
static const Objekttabelle TRESOR_METHODEN = {
    tresor_zeichen, tresor_betreten, normal_freigeben };
static const Objekttabelle WAECHTER_METHODEN = {
    waechter_zeichen, waechter_betreten, normal_freigeben };
static const Objekttabelle AUSGANG_METHODEN = {
    ausgang_zeichen, ausgang_betreten, normal_freigeben };

Objekt *objekt_erzeugen(int id) {
    Objekt *basis = NULL;
    if (id == ID_SCHLUESSEL || id == ID_MUENZE) {
        Aufhebbar *o = calloc(1, sizeof *o);
        if (!o) return NULL;
        o->gegenstand = id == ID_SCHLUESSEL ? SCHLUESSEL : MUENZE;
        basis = &o->basis;
        basis->typ = AUFHEBBAR;
        basis->position = id == ID_SCHLUESSEL ? (Punkt){3, 5} : (Punkt){11, 7};
        basis->methoden = &AUFHEBBAR_METHODEN;
    } else if (id == ID_TUER) {
        Tuer *o = calloc(1, sizeof *o);
        if (!o) return NULL;
        o->schluessel = SCHLUESSEL;
        basis = &o->basis;
        basis->typ = TUER; basis->position = (Punkt){6, 3};
        basis->methoden = &TUER_METHODEN;
    } else if (id == ID_TRESOR) {
        Tresor *o = calloc(1, sizeof *o);
        if (!o) return NULL;
        basis = &o->basis;
        basis->typ = TRESOR; basis->position = (Punkt){13, 1};
        basis->methoden = &TRESOR_METHODEN;
    } else if (id == ID_WAECHTER_LINKS || id == ID_WAECHTER_RECHTS) {
        Waechter *o = calloc(1, sizeof *o);
        if (!o) return NULL;
        o->radius = id == ID_WAECHTER_LINKS ? 1 : 2;
        basis = &o->basis;
        basis->typ = WAECHTER;
        basis->position = id == ID_WAECHTER_LINKS ? (Punkt){1, 5} : (Punkt){9, 5};
        basis->methoden = &WAECHTER_METHODEN;
    } else if (id == ID_AUSGANG) {
        Ausgang *o = calloc(1, sizeof *o);
        if (!o) return NULL;
        basis = &o->basis;
        basis->typ = AUSGANG; basis->position = (Punkt){1, 1};
        basis->methoden = &AUSGANG_METHODEN;
    }
    if (basis) { basis->id = id; basis->aktiv = true; }
    return basis;
}

Objekt *objekt_finden(const Spiel *spiel, int id) {
    for (Objektknoten *k = spiel->objekte; k; k = k->naechster) {
        if (k->objekt->id == id) return k->objekt;
    }
    return NULL;
}
Objekt *objekt_am_punkt(const Spiel *spiel, Punkt p) {
    for (Objektknoten *k = spiel->objekte; k; k = k->naechster) {
        if (k->objekt->aktiv && punkt_gleich(k->objekt->position, p)) return k->objekt;
    }
    return NULL;
}
int objekte_anlegen(Spiel *spiel) {
    for (int id = 0; id < OBJEKT_ANZAHL; id++) {
        Objekt *o = objekt_erzeugen(id);
        if (!o) return FEHLER_SPEICHER;
        Objektknoten *k = malloc(sizeof *k);
        if (!k) { o->methoden->freigeben(o); return FEHLER_SPEICHER; }
        k->objekt = o; k->naechster = spiel->objekte; spiel->objekte = k;
    }
    return OK;
}
void objekte_freigeben(Spiel *spiel) {
    while (spiel->objekte) {
        Objektknoten *alt = spiel->objekte;
        spiel->objekte = alt->naechster;
        alt->objekt->methoden->freigeben(alt->objekt);
        free(alt);
    }
}
