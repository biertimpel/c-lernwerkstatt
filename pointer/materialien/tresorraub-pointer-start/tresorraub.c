/* Weiterarbeit am Spielstand aus Datenstrukturen. */
#include "tresorraub.h"
#include "piktogramme/piktogramme.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define SKALIERUNG 2
#define FELDGROESSE (16 * SKALIERUNG)
#define RAND_LINKS 80
#define RAND_OBEN 96

#define BREITE 15
#define HOEHE 9
#define ANZAHL_WAECHTER 2
#define ZUGLIMIT 60


typedef struct {
    int x;
    int y;
} Punkt;

typedef struct {
    Punkt position;
    int reichweite;
} Waechter;

bool punkt_gleich(Punkt a, Punkt b) {
    return a.x == b.x && a.y == b.y;
}

/* Rasterabstand: waagerechter plus senkrechter Abstand (abs aus stdlib.h). */
int rasterabstand(Punkt a, Punkt b) {
    return abs(a.x - b.x) + abs(a.y - b.y);
}

bool ueberwacht(Punkt p, const Waechter waechter[], size_t anzahl) {
    for (size_t i = 0; i < anzahl; i++) {
        if (rasterabstand(p, waechter[i].position) <= waechter[i].reichweite) {
            return true;
        }
    }
    return false;
}

/* BREITE + 1: 15 sichtbare Zeichen plus abschliessendes '\0' je Zeile. */
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

bool innerhalb(Punkt p) {
    return p.x >= 0 && p.x < BREITE && p.y >= 0 && p.y < HOEHE;
}

/* Bereichspruefung VOR dem Arrayzugriff; && wertet von links aus und
   bricht beim ersten false ab. Zeile = y, Spalte = x. */
bool begehbar(Punkt p) {
    return innerhalb(p) && GRUNDRISS[p.y][p.x] != '#';
}

typedef struct {
    Punkt figur;
    int zuege;
    bool hat_schluessel;
    bool hat_beute;
    bool hat_muenze;
    Punkt ausgang;
    Punkt schluessel;
    bool schluessel_liegt;
    Punkt tuer;
    bool tuer_offen;
    Punkt tresor;
    bool beute_im_tresor;
    Punkt muenze;
    bool muenze_liegt;
    Waechter waechter[ANZAHL_WAECHTER];
} Spielstand;

Spielstand startzustand(void) {
    Spielstand s = {
        .figur = {3, 7},
        .zuege = 0,
        .hat_schluessel = false,
        .hat_beute = false,
        .hat_muenze = false,
        .ausgang = {1, 1},
        .schluessel = {3, 5},
        .schluessel_liegt = true,
        .tuer = {6, 3},
        .tuer_offen = false,
        .tresor = {13, 1},
        .beute_im_tresor = true,
        .muenze = {11, 7},
        .muenze_liegt = true,
        .waechter = {{{1, 5}, 1}, {{9, 5}, 2}}
    };
    return s;
}

/* Formatiert die Position und die Zahl der ausgefuehrten Zuege. */
static int statuszeile(char ziel[], size_t groesse, Punkt position, int zuege) {
    int laenge = snprintf(ziel, groesse, "Position (%d, %d) | Zuege %d/%d",
                          position.x, position.y, zuege, ZUGLIMIT);
    return laenge < 0 || (size_t)laenge >= groesse ? -1 : 0;
}


char feldzeichen(Spielstand s, Punkt p) {
    if (!innerhalb(p)) {
        return ' ';
    }
    if (punkt_gleich(p, s.figur)) {
        return '@';
    }
    for (size_t i = 0; i < ANZAHL_WAECHTER; i++) {
        if (punkt_gleich(p, s.waechter[i].position)) {
            return 'W';
        }
    }
    if (punkt_gleich(p, s.tuer)) {
        return s.tuer_offen ? '/' : 'D';
    }
    if (punkt_gleich(p, s.tresor)) {
        return s.beute_im_tresor ? 'T' : 't';
    }
    if (s.schluessel_liegt && punkt_gleich(p, s.schluessel)) {
        return 'k';
    }
    if (s.muenze_liegt && punkt_gleich(p, s.muenze)) {
        return 'c';
    }
    if (punkt_gleich(p, s.ausgang)) {
        return 'E';
    }
    if (GRUNDRISS[p.y][p.x] == '#') {
        return '#';
    }
    if (ueberwacht(p, s.waechter, ANZAHL_WAECHTER)) {
        return '!';
    }
    return '.';
}

void spielfeld_ausgeben(Spielstand s) {
    for (int y = 0; y < HOEHE; y++) {
        for (int x = 0; x < BREITE; x++) {
            putchar(feldzeichen(s, (Punkt){x, y}));
        }
        putchar('\n');
    }
    printf("Legende: @ Figur, W Waechter, D/ / Tuer zu/offen, T/t Tresor voll/leer,\n"
           "         k Schluessel, c Muenze, E Ausgang, # Wand, ! ueberwacht\n");
}

/* Seite 3: Auf einen lesenden Pointer umstellen. */
static int spielstand_lesen(Spielstand s, char ziel[], size_t groesse) {
    return statuszeile(ziel, groesse, s.figur, s.zuege);
}

/* Aus Datenstrukturen: Hier wird nur die Kopie veraendert. */
void zug_zaehlen(Spielstand s) {
    s.zuege++;
    printf("Zuege innerhalb von zug_zaehlen: %d\n", s.zuege);
}

int text_ausgeben(void) {
    Spielstand s = startzustand();
    char zeile[64];
    spielfeld_ausgeben(s);
    if (spielstand_lesen(s, zeile, sizeof(zeile)) < 0) {
        return -1;
    }
    puts(zeile);
    return 0;
}

/* Die folgenden Funktionen ruft das Geruest auf. Die Ruempfe sind
   Platzhalter; Sie schreiben sie in der Aufgabe auf Seite 4. */

/* Seite 4: Den laufenden Spielstand einmal aus dem Startzustand anlegen. */
int spiel_starten(void) {
    return 0;
}

/* Seite 4: Bei einer Pfeiltaste die Figur im laufenden Spielstand bewegen. */
int taste_gedrueckt(SDL_Keycode taste) {
    (void)taste; /* Platzhalter: entfernen, sobald Sie taste verwenden. */
    return 0;
}

/* Seite 4: Die Statuszeile des laufenden Spielstands in ziel schreiben.
   Bis dahin steht ein fester Text im Fenstertitel. */
int spiel_status(char ziel[], size_t groesse) {
    int laenge = snprintf(ziel, groesse, "Tresorraub");
    return laenge < 0 || (size_t)laenge >= groesse ? -1 : 0;
}

/* Fuer das Kapitel "Dynamischer Speicher und Listen" vorgesehen. */
void aufraeumen(void) {
}


static int feld_zeichnen(SDL_Renderer *renderer, Spielstand s, Punkt p) {
    int px = RAND_LINKS + p.x * FELDGROESSE;
    int py = RAND_OBEN + p.y * FELDGROESSE;
    char z = feldzeichen(s, p);
    if (z == '#') {
        return zeichne_wand(renderer, px, py, SKALIERUNG);
    }
    /* Untergrund: ueberwacht oder normaler Boden. Objekte haben transparente
       Zellen und werden darueber gezeichnet. */
    int ergebnis = ueberwacht(p, s.waechter, ANZAHL_WAECHTER)
                       ? zeichne_ueberwachung(renderer, px, py, SKALIERUNG)
                       : zeichne_boden(renderer, px, py, SKALIERUNG);
    if (ergebnis < 0) {
        return ergebnis;
    }
    switch (z) {
        case '@': return zeichne_spielfigur_froh(renderer, px, py, SKALIERUNG);
        case 'W': return zeichne_waechter(renderer, px, py, SKALIERUNG);
        case 'D': return zeichne_tuer_geschlossen(renderer, px, py, SKALIERUNG);
        case '/': return zeichne_tuer_offen(renderer, px, py, SKALIERUNG);
        case 'T': return zeichne_tresor_geschlossen(renderer, px, py, SKALIERUNG);
        case 't': return zeichne_tresor_leer(renderer, px, py, SKALIERUNG);
        case 'k': return zeichne_schluessel(renderer, px, py, SKALIERUNG);
        case 'c': return zeichne_muenze(renderer, px, py, SKALIERUNG);
        case 'E': return zeichne_ausgang(renderer, px, py, SKALIERUNG);
        default: return 0;
    }
}

int zeichne(SDL_Renderer *renderer) {
    Spielstand s = startzustand();
    for (int y = 0; y < HOEHE; y++) {
        for (int x = 0; x < BREITE; x++) {
            int ergebnis = feld_zeichnen(renderer, s, (Punkt){x, y});
            if (ergebnis < 0) {
                return ergebnis;   /* Fehler sofort weitergeben */
            }
        }
    }
    return 0;
}
