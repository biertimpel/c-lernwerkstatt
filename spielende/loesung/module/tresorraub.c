/* Anschluesse an das Programmgeruest: Textausgabe, Tasten, Statuszeile,
   Zeichnen. Die Spiellogik steht in den Modulen. */
#include "tresorraub.h"
#include "piktogramme/piktogramme.h"
#include "spiel.h"
#include "regeln.h"
#include "inventar.h"
#include "objekte.h"
#include "spiellogik.h"
#include "speicherstand.h"
#include <stdio.h>

#define SKALIERUNG 2
#define FELDGROESSE (16 * SKALIERUNG)
#define RAND_LINKS 80
#define RAND_OBEN 96

/* Das Inventar erscheint im Fenster unter dem Spielfeld. Der erste Platz
   beginnt bei INVENTAR_X, INVENTAR_Y; jeder weitere liegt FELDGROESSE
   Pixel weiter rechts. */
#define INVENTAR_X RAND_LINKS
#define INVENTAR_Y (RAND_OBEN + HOEHE * FELDGROESSE + 16)

int text_ausgeben(void) {
    /* Hier entstehen Ihre Versuche und Textausgaben.
       Starten mit: ./programm --text */
    Regeln regeln = regeln_standard();
    if (regeln_laden(REGELDATEI, &regeln) < 0) {
        return -1;
    }
    printf("Regeln: zuglimit %d, reichweite_waechter_1 %d, "
           "reichweite_waechter_2 %d, inventar_groesse %d\n",
           regeln.zuglimit, regeln.reichweite[0], regeln.reichweite[1],
           regeln.inventar_groesse);
    Spielstand s;
    startzustand(&s, &regeln);
    char zeile[96];
    spielfeld_ausgeben(&s);
    if (spielstand_lesen(&s, zeile, sizeof(zeile)) < 0) {
        return -1;
    }
    puts(zeile);
    inventar_leeren(&s.inventar);   /* s besitzt seine Liste */
    return 0;
}

/* Der laufende Spielstand. spiel_starten fuellt ihn mit dem Startzustand. */
static Spielstand spielstand;

/* Die folgenden Funktionen ruft das Geruest auf. */

int spiel_starten(void) {
    inventar_leeren(&spielstand.inventar);   /* falls schon Eintraege da sind */
    Regeln regeln = regeln_standard();
    if (regeln_laden(REGELDATEI, &regeln) < 0) {
        return -1;   /* fehlerhafte Datei: Spiel startet nicht */
    }
    startzustand(&spielstand, &regeln);
    return 0;
}

int taste_gedrueckt(SDL_Keycode taste) {
    if (taste == SDLK_n) {                       /* neues Spiel, jederzeit */
        if (spiel_starten() < 0) {
            return -1;
        }
        printf("Neues Spiel.\n");
        return 0;
    }
    if (spielstand.zustand != LAEUFT && taste != SDLK_F9) {
        if (taste == SDLK_LEFT || taste == SDLK_RIGHT || taste == SDLK_UP || taste == SDLK_DOWN
            || taste == SDLK_SPACE || taste == SDLK_l || taste == SDLK_F5) {
            printf("Das Spiel ist vorbei. N: neues Spiel, F9: Spielstand laden.\n");
        }
        return 0;
    }
    switch (taste) {
        case SDLK_LEFT:  figur_bewegen(&spielstand, -1, 0); break;
        case SDLK_RIGHT: figur_bewegen(&spielstand, 1, 0); break;
        case SDLK_UP:    figur_bewegen(&spielstand, 0, -1); break;
        case SDLK_DOWN:  figur_bewegen(&spielstand, 0, 1); break;
        case SDLK_SPACE:
            if (handeln(&spielstand) < 0) {
                return -1;   /* kein Speicher: Programmfehler */
            }
            inventar_ausgeben(&spielstand.inventar);
            break;
        case SDLK_l:
            ablegen(&spielstand);
            inventar_ausgeben(&spielstand.inventar);
            break;
        case SDLK_F5:
            printf(spielstand_speichern(&spielstand, SPIELSTANDDATEI) == 0
                   ? "Spielstand gespeichert.\n" : "Speichern fehlgeschlagen.\n");
            break;
        case SDLK_F9:
            printf(spielstand_laden(&spielstand, SPIELSTANDDATEI) == 0
                   ? "Spielstand geladen.\n" : "Laden fehlgeschlagen, bisheriger Stand bleibt erhalten.\n");
            break;
        default: break;   /* andere Tasten veraendern nichts */
    }
    return 0;
}

int spiel_status(char ziel[], size_t groesse) {
    return spielstand_lesen(&spielstand, ziel, groesse);
}

void aufraeumen(void) {
    inventar_leeren(&spielstand.inventar);
}

static int feld_zeichnen(SDL_Renderer *renderer, const Spielstand *s, Punkt p) {
    int px = RAND_LINKS + p.x * FELDGROESSE;
    int py = RAND_OBEN + p.y * FELDGROESSE;
    if (ist_wand(p)) {
        return zeichne_wand(renderer, px, py, SKALIERUNG);
    }
    /* Untergrund: ueberwacht oder normaler Boden. Darueber steht hoechstens
       eines: Figur, Waechter oder ein Objekt. */
    int ergebnis = ueberwacht(p, s->waechter, ANZAHL_WAECHTER)
                       ? zeichne_ueberwachung(renderer, px, py, SKALIERUNG)
                       : zeichne_boden(renderer, px, py, SKALIERUNG);
    if (ergebnis < 0) {
        return ergebnis;
    }
    if (punkt_gleich(p, s->figur)) {
        return s->zustand == ENTDECKT
            ? zeichne_spielfigur_entdeckt(renderer, px, py, SKALIERUNG)
            : zeichne_spielfigur_froh(renderer, px, py, SKALIERUNG);
    }
    if (waechter_auf(s, p)) {
        return zeichne_waechter(renderer, px, py, SKALIERUNG);
    }
    const Objekt *o = objekt_auf(s, p);
    if (o != NULL) {
        return objekt_bild(o)(renderer, px, py, SKALIERUNG);
    }
    return 0;
}

/* Gewonnen: ein gruener Rahmen um das Spielfeld, vier Pixel breit. */
static int zeichne_siegrahmen(SDL_Renderer *renderer) {
    if (SDL_SetRenderDrawColor(renderer, 40, 200, 90, 255) < 0) {
        return -1;
    }
    for (int i = 0; i < 4; i++) {
        SDL_Rect rahmen = {RAND_LINKS - 4 + i, RAND_OBEN - 4 + i,
                           BREITE * FELDGROESSE + 8 - 2 * i, HOEHE * FELDGROESSE + 8 - 2 * i};
        if (SDL_RenderDrawRect(renderer, &rahmen) < 0) {
            return -1;
        }
    }
    return 0;
}

int zeichne(SDL_Renderer *renderer) {
    for (int y = 0; y < HOEHE; y++) {
        for (int x = 0; x < BREITE; x++) {
            int ergebnis = feld_zeichnen(renderer, &spielstand, (Punkt){x, y});
            if (ergebnis < 0) {
                return ergebnis;   /* Fehler sofort weitergeben */
            }
        }
    }
    /* das Inventar unter dem Spielfeld, in der Reihenfolge der Liste */
    int x = INVENTAR_X;
    for (const Eintrag *e = spielstand.inventar.erster; e != NULL; e = e->naechster) {
        int ergebnis = objekt_bild(e->gegenstand)(renderer, x, INVENTAR_Y, SKALIERUNG);
        if (ergebnis < 0) {
            return ergebnis;
        }
        x += FELDGROESSE;
    }
    /* gescheitert: WASTED quer ueber das Spielfeld stempeln */
    if (spielstand.zustand == ENTDECKT || spielstand.zustand == ZEIT_ABGELAUFEN) {
        return zeichne_wasted(renderer, RAND_LINKS, RAND_OBEN,
                              BREITE * FELDGROESSE, HOEHE * FELDGROESSE);
    }
    if (spielstand.zustand == GEWONNEN) {
        return zeichne_siegrahmen(renderer);
    }
    return 0;
}
