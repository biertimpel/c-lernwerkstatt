/* Spiellogik: Zuege, Spielende, Bewegung, Leertaste, Ablegen, Ausgabe. */
#include "spiellogik.h"
#include "objekte.h"
#include "inventar.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* Prueft nach einem Zug, ob das Spiel verloren ist. Ein Sieg wird schon
   beim Betreten des Ausgangs festgestellt und geht vor. */
void spielende_pruefen(Spielstand *s) {
    if (s->zustand != LAEUFT) {
        return;
    }
    if (ueberwacht(s->figur, s->waechter, ANZAHL_WAECHTER)) {
        s->zustand = ENTDECKT;
        printf("Entdeckt!\n");
    } else if (s->zuege >= s->regeln.zuglimit) {
        s->zustand = ZEIT_ABGELAUFEN;
        printf("Die Zeit ist abgelaufen.\n");
    }
}

/* Zaehlt einen Zug und prueft danach das Spielende. */
void zug_zaehlen(Spielstand *s) {
    s->zuege++;
    spielende_pruefen(s);
}

/* Formatiert die Position und die Zahl der ausgefuehrten Zuege. */
static int statuszeile(char ziel[], size_t groesse, Punkt position, int zuege,
                       int zuglimit, const char *hier) {
    int laenge = hier == NULL
        ? snprintf(ziel, groesse, "Position (%d, %d) | Zuege %d/%d",
                   position.x, position.y, zuege, zuglimit)
        : snprintf(ziel, groesse, "Position (%d, %d) | Zuege %d/%d | Hier liegt: %s",
                   position.x, position.y, zuege, zuglimit, hier);
    return laenge < 0 || (size_t)laenge >= groesse ? -1 : 0;
}

/* Text fuer das Spielende, NULL waehrend des Spiels. */
static const char *zustand_text(Spielzustand zustand) {
    switch (zustand) {
        case GEWONNEN:        return "Entkommen!";
        case ENTDECKT:        return "WASTED: entdeckt";
        case ZEIT_ABGELAUFEN: return "WASTED: Zeit abgelaufen";
        case LAEUFT:          break;
    }
    return NULL;
}

int spielstand_lesen(const Spielstand *s, char ziel[], size_t groesse) {
    if (s == NULL) {
        return -1;
    }
    const Objekt *hier = gegenstand_auf(s, s->figur);
    if (statuszeile(ziel, groesse, s->figur, s->zuege, s->regeln.zuglimit,
                    hier == NULL ? NULL : hier->art->name) < 0) {
        return -1;
    }
    const char *ende = zustand_text(s->zustand);
    if (ende == NULL) {
        return 0;
    }
    size_t laenge = strlen(ziel);
    int dazu = snprintf(ziel + laenge, groesse - laenge, " | %s", ende);
    return dazu < 0 || (size_t)dazu >= groesse - laenge ? -1 : 0;
}

char feldzeichen(const Spielstand *s, Punkt p) {
    if (!innerhalb(p)) {
        return ' ';
    }
    if (punkt_gleich(p, s->figur)) {
        return '@';
    }
    if (waechter_auf(s, p)) {
        return 'W';
    }
    const Objekt *o = objekt_auf(s, p);
    if (o != NULL) {
        return objekt_zeichen(o);
    }
    if (ist_wand(p)) {
        return '#';
    }
    if (ueberwacht(p, s->waechter, ANZAHL_WAECHTER)) {
        return '!';
    }
    return '.';
}

void spielfeld_ausgeben(const Spielstand *s) {
    for (int y = 0; y < HOEHE; y++) {
        for (int x = 0; x < BREITE; x++) {
            putchar(feldzeichen(s, (Punkt){x, y}));
        }
        putchar('\n');
    }
    printf("Legende: @ Figur, W Waechter, D/ / Tuer zu/offen, T/t Tresor voll/leer,\n"
           "         k Schluessel, c Muenze, b Beute, E Ausgang, # Wand, ! ueberwacht\n");
}

bool figur_bewegen(Spielstand *s, int dx, int dy) {
    if (s == NULL || s->zustand != LAEUFT) {
        return false;   /* nach dem Spielende bewegt sich nichts mehr */
    }
    bool waagerecht = (dx == -1 || dx == 1) && dy == 0;
    bool senkrecht = (dy == -1 || dy == 1) && dx == 0;
    if (!waagerecht && !senkrecht) {
        return false;
    }
    Punkt ziel = {s->figur.x + dx, s->figur.y + dy};
    if (!begehbar(ziel)) {
        return false;
    }
    const Objekt *o = objekt_auf(s, ziel);
    if (o != NULL && o->art->betretbar != NULL && !o->art->betretbar(o)) {
        return false;
    }
    s->figur = ziel;
    for (size_t i = 0; i < s->anzahl_objekte; i++) {   /* Methode betreten */
        Objekt *auf_ziel = &s->objekte[i];
        if (auf_ziel->auf_feld && punkt_gleich(auf_ziel->position, ziel) && auf_ziel->art->betreten != NULL) {
            auf_ziel->art->betreten(auf_ziel, s);
        }
    }
    zug_zaehlen(s);
    return true;
}

/* Leertaste. Zuerst sind die Objekte auf dem Feld der Figur an der Reihe,
   dann die auf den Nachbarfeldern. Das erste zustaendige Objekt handelt.
   Ergebnis: 0, auch bei Spielfaellen; negativ bei einem Programmfehler. */
int handeln(Spielstand *s) {
    if (s->zustand != LAEUFT) {
        return 0;
    }
    for (int abstand = 0; abstand <= 1; abstand++) {
        for (size_t i = 0; i < s->anzahl_objekte; i++) {
            Objekt *o = &s->objekte[i];
            if (!o->auf_feld || o->art->benutzen == NULL
                || rasterabstand(o->position, s->figur) != abstand) {
                continue;
            }
            Benutzung ergebnis = o->art->benutzen(o, s, abstand);
            if (ergebnis == BENUTZEN_FEHLER) {
                return -1;
            }
            if (ergebnis == ERLEDIGT) {
                return 0;
            }
        }
    }
    printf("Hier gibt es nichts zu tun.\n");
    return 0;
}

/* Ein Feld ist frei, wenn es begehbar ist und dort weder Figur, Waechter
   noch ein Objekt ist. */
static bool feld_frei(const Spielstand *s, Punkt p) {
    return begehbar(p) && !punkt_gleich(p, s->figur) && !waechter_auf(s, p)
        && objekt_auf(s, p) == NULL;
}

static bool freies_nachbarfeld(const Spielstand *s, Punkt *ziel) {
    static const int richtung[8][2] = {
        {0, -1}, {1, -1}, {1, 0}, {1, 1}, {0, 1}, {-1, 1}, {-1, 0}, {-1, -1}
    };
    for (size_t i = 0; i < 8; i++) {
        Punkt p = {s->figur.x + richtung[i][0], s->figur.y + richtung[i][1]};
        if (feld_frei(s, p)) {
            *ziel = p;
            return true;
        }
    }
    return false;
}

/* Taste L: legt alle Gegenstaende in der Reihenfolge der Liste auf freie
   Nachbarfelder. Zaehlt als ein Zug, wenn mindestens einer abgelegt wurde.
   Ergebnis: Zahl der abgelegten Gegenstaende. */
int ablegen(Spielstand *s) {
    if (s->zustand != LAEUFT) {
        return 0;
    }
    if (s->inventar.erster == NULL) {
        printf("Das Inventar ist leer.\n");
        return 0;
    }
    int abgelegt = 0;
    Eintrag *vorher = NULL;
    Eintrag *aktuell = s->inventar.erster;
    while (aktuell != NULL) {
        Eintrag *naechster = aktuell->naechster;
        Objekt *g = aktuell->gegenstand;
        Punkt ziel;
        if (freies_nachbarfeld(s, &ziel)) {
            g->position = ziel;
            g->auf_feld = true;
            printf("%s auf (%d, %d) abgelegt.\n", g->art->name, ziel.x, ziel.y);
            if (vorher == NULL) {
                s->inventar.erster = naechster;
            } else {
                vorher->naechster = naechster;
            }
            free(aktuell);
            abgelegt++;
        } else {
            printf("Kein Platz fuer %s, bleibt im Inventar.\n", g->art->name);
            vorher = aktuell;
        }
        aktuell = naechster;
    }
    if (abgelegt > 0) {
        zug_zaehlen(s);
    }
    return abgelegt;
}
