#include "spiel.h"
#include "inventar.h"
#include "objekt.h"
#include <stdio.h>
#include <string.h>

static const char GRUNDRISS[HOEHE][BREITE + 1] = {
    "###############",
    "#.....#.......#",
    "#.##..#..###..#",
    "#.............#",
    "#.##..#.......#",
    "#.....#.......#",
    "#.....#.......#",
    "#.....#.......#",
    "###############"
};

bool punkt_gleich(Punkt a, Punkt b) { return a.x == b.x && a.y == b.y; }
bool punkt_gueltig(Punkt p) {
    return p.x >= 0 && p.x < BREITE && p.y >= 0 && p.y < HOEHE;
}
int manhattan_abstand(Punkt a, Punkt b) {
    int dx = a.x - b.x, dy = a.y - b.y;
    if (dx < 0) dx = -dx;
    if (dy < 0) dy = -dy;
    return dx + dy;
}
int spiel_neu(Spiel *spiel) {
    if (!spiel) return FEHLER_ARGUMENT;
    *spiel = (Spiel){0};
    memcpy(spiel->karte, GRUNDRISS, sizeof GRUNDRISS);
    spiel->spieler.position = (Punkt){3, 7};
    spiel->phase = LAEUFT;
    int ergebnis = objekte_anlegen(spiel);
    if (ergebnis < 0) { spiel_freigeben(spiel); return ergebnis; }
    snprintf(spiel->meldung, sizeof spiel->meldung,
             "Schluessel k holen, Tuer D oeffnen, Tresor T leeren, zu E zurueck.");
    return OK;
}
void spiel_freigeben(Spiel *spiel) {
    if (!spiel) return;
    inventar_freigeben(&spiel->spieler);
    objekte_freigeben(spiel);
}
bool spiel_betretbar(const Spiel *spiel, Punkt p) {
    return punkt_gueltig(p) && spiel->karte[p.y][p.x] != '#';
}
bool spiel_ueberwacht(const Spiel *spiel, Punkt p) {
    if (!punkt_gueltig(p)) return false;
    for (Objektknoten *k = spiel->objekte; k; k = k->naechster) {
        const Objekt *o = k->objekt;
        if (o->typ == WAECHTER && o->aktiv) {
            const Waechter *w = (const Waechter *)o;
            if (manhattan_abstand(o->position, p) <= w->radius) return true;
        }
    }
    return false;
}
char spiel_zeichen(const Spiel *spiel, Punkt p) {
    if (punkt_gleich(p, spiel->spieler.position)) return '@';
    if (!spiel_betretbar(spiel, p)) return '#';
    Objekt *objekt = objekt_am_punkt(spiel, p);
    if (objekt) return objekt->methoden->zeichen(objekt);
    return spiel_ueberwacht(spiel, p) ? '!' : '.';
}
static void zug_abschliessen(Spiel *spiel) {
    spiel->zuege++;
    if (spiel_ueberwacht(spiel, spiel->spieler.position)) {
        spiel->phase = ENTDECKT;
        snprintf(spiel->meldung, sizeof spiel->meldung, "Alarm! Sie wurden entdeckt.");
    } else if (punkt_gleich(spiel->spieler.position, (Punkt){1, 1})
               && inventar_hat(&spiel->spieler, BEUTE)) {
        spiel->phase = GEWONNEN;
        snprintf(spiel->meldung, sizeof spiel->meldung, "Entkommen! Der Tresorraub ist gelungen.");
    } else if (spiel->zuege >= ZUGLIMIT) {
        spiel->phase = ZEIT_ABGELAUFEN;
        snprintf(spiel->meldung, sizeof spiel->meldung, "Zeit abgelaufen. Die Polizei ist da.");
    }
}
int spiel_bewegen(Spiel *spiel, int dx, int dy) {
    if (!spiel || spiel->phase != LAEUFT) return FEHLER_ARGUMENT;
    if (!((dx == 0 && (dy == -1 || dy == 1))
          || (dy == 0 && (dx == -1 || dx == 1)))) return FEHLER_ARGUMENT;
    Punkt ziel = {spiel->spieler.position.x + dx, spiel->spieler.position.y + dy};
    if (!spiel_betretbar(spiel, ziel)) {
        snprintf(spiel->meldung, sizeof spiel->meldung, "Dort ist eine Wand. Kein Zug verbraucht.");
        return BLOCKIERT;
    }
    snprintf(spiel->meldung, sizeof spiel->meldung, "Leise weiter.");
    Objekt *objekt = objekt_am_punkt(spiel, ziel);
    if (objekt) {
        int ergebnis = objekt->methoden->betreten(objekt, spiel);
        if (ergebnis != OK) return ergebnis;
    }
    spiel->spieler.position = ziel;
    zug_abschliessen(spiel);
    return OK;
}
int spiel_warten(Spiel *spiel) {
    if (!spiel || spiel->phase != LAEUFT) return FEHLER_ARGUMENT;
    snprintf(spiel->meldung, sizeof spiel->meldung, "Ein Zug gewartet.");
    zug_abschliessen(spiel);
    return OK;
}
