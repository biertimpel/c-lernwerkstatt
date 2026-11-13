/* Musterloesung "Pointer und Datenaustausch" - nur fuer Lehrende.

   Entwurfsentscheidungen:
   - Ein einziger laufender Spielstand als static-Variable (Seite 4).
     Das Geruest kennt den Typ Spielstand nicht; es erreicht ihn nur ueber
     spiel_starten, taste_gedrueckt, zeichne und spiel_status.
   - Veraendernde Funktionen erhalten Spielstand *, lesende
     const Spielstand * (Seite 3). Die Kopie-Variante zug_zaehlen bleibt
     fuer den Vergleich auf Seite 2 erhalten.
   - figur_bewegen prueft NULL, nur die vier Nachbarschritte, Rand, Wand und
     verschlossene Tuer, bevor sie etwas veraendert. Das Zuglimit wird noch
     nicht ausgewertet; Gegenstaende werden nur angezeigt. */
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


char feldzeichen(const Spielstand *s, Punkt p) {
    if (!innerhalb(p)) {
        return ' ';
    }
    if (punkt_gleich(p, s->figur)) {
        return '@';
    }
    for (size_t i = 0; i < ANZAHL_WAECHTER; i++) {
        if (punkt_gleich(p, s->waechter[i].position)) {
            return 'W';
        }
    }
    if (punkt_gleich(p, s->tuer)) {
        return s->tuer_offen ? '/' : 'D';
    }
    if (punkt_gleich(p, s->tresor)) {
        return s->beute_im_tresor ? 'T' : 't';
    }
    if (s->schluessel_liegt && punkt_gleich(p, s->schluessel)) {
        return 'k';
    }
    if (s->muenze_liegt && punkt_gleich(p, s->muenze)) {
        return 'c';
    }
    if (punkt_gleich(p, s->ausgang)) {
        return 'E';
    }
    if (GRUNDRISS[p.y][p.x] == '#') {
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
           "         k Schluessel, c Muenze, E Ausgang, # Wand, ! ueberwacht\n");
}

/* Seite 3: liest den Spielstand nur, deshalb const. */
static int spielstand_lesen(const Spielstand *s, char ziel[], size_t groesse) {
    if (s == NULL) {
        return -1;
    }
    return statuszeile(ziel, groesse, s->figur, s->zuege);
}

/* Seite 2: Hier wird nur die Kopie veraendert. */
void zug_zaehlen(Spielstand s) {
    s.zuege++;
    printf("Zuege innerhalb von zug_zaehlen: %d\n", s.zuege);
}

/* Seite 2: veraendert das Original ueber den Pointer.
   Gleichwertig: (*p).zuege++; */
void zug_zaehlen_original(Spielstand *p) {
    p->zuege++;
}

/* Seite 3: genau ein Schritt zu einem Nachbarfeld. */
bool figur_bewegen(Spielstand *s, int dx, int dy) {
    if (s == NULL) {
        return false;
    }
    bool waagerecht = (dx == -1 || dx == 1) && dy == 0;
    bool senkrecht = (dy == -1 || dy == 1) && dx == 0;
    if (!waagerecht && !senkrecht) {
        return false;
    }
    Punkt ziel = {s->figur.x + dx, s->figur.y + dy};
    if (!begehbar(ziel)) {      /* prueft zuerst die Grenzen, dann die Wand */
        return false;
    }
    if (punkt_gleich(ziel, s->tuer) && !s->tuer_offen) {
        return false;
    }
    s->figur = ziel;
    zug_zaehlen_original(s);
    return true;
}

static void ueberschrift(const char *text) {
    printf("\n=== %s ===\n", text);
}

/* Fuehrt einen Schritt aus und gibt Rueckgabe und Statuszeile aus. */
static int schritt_zeigen(const char *name, Spielstand *s, int dx, int dy) {
    char zeile[64];
    bool erfolg = figur_bewegen(s, dx, dy);
    if (s == NULL) {
        printf("%-28s %-5s (kein Spielstand)\n", name, erfolg ? "true" : "false");
        return 0;
    }
    if (spielstand_lesen(s, zeile, sizeof(zeile)) < 0) {
        return -1;
    }
    printf("%-28s %-5s %s\n", name, erfolg ? "true" : "false", zeile);
    return 0;
}

int text_ausgeben(void) {
    ueberschrift("Seite 1: Eine Adresse speichern und benutzen");
    int var = 10;
    int *p = &var;
    int **pp = &p;
    printf("var = %d, *p = %d\n", var, *p);
    printf("p    = %p\n&var = %p\n", (void *)p, (void *)&var);
    *p = 20;
    printf("nach *p = 20: var = %d, **pp = %d\n", var, **pp);

    ueberschrift("Seite 1: Zeichenketten vergleichen");
    /* a == b ist bewusst nicht enthalten: GCC und Clang warnen
       (siehe README). */
    char a[] = "Beute";
    char b[] = "Beute";
    printf("strcmp(a, b) = %d\n", strcmp(a, b));
    printf("a bei %p, b bei %p\n", (void *)a, (void *)b);
    char c[] = "Muenze";   /* der Versuch mit geaendertem Inhalt von b */
    printf("strcmp(\"Beute\", \"Muenze\") < 0: %d\n", strcmp(a, c) < 0);

    ueberschrift("Seite 2: Kopie und Original");
    Spielstand s = startzustand();
    zug_zaehlen(s);
    printf("nach zug_zaehlen(s):           %d\n", s.zuege);
    zug_zaehlen_original(&s);
    printf("nach zug_zaehlen_original(&s): %d\n", s.zuege);

    ueberschrift("Seite 3: Einen Schritt ausfuehren");
    s = startzustand();
    char zeile[64];
    if (spielstand_lesen(&s, zeile, sizeof(zeile)) < 0) {
        return -1;
    }
    printf("%-28s %-5s %s\n", "Start", "-", zeile);
    if (schritt_zeigen("Schritt nach rechts", &s, 1, 0) < 0
        || schritt_zeigen("Schritt nach rechts", &s, 1, 0) < 0
        || schritt_zeigen("Schritt nach unten (Wand)", &s, 0, 1) < 0) {
        return -1;
    }
    s.figur = (Punkt){5, 3};
    s.tuer_offen = false;
    if (schritt_zeigen("Tuer verschlossen, rechts", &s, 1, 0) < 0) {
        return -1;
    }
    s.tuer_offen = true;
    if (schritt_zeigen("Tuer offen, rechts", &s, 1, 0) < 0
        || schritt_zeigen("ungueltig (2, 0)", &s, 2, 0) < 0
        || schritt_zeigen("ungueltig (1, 1)", &s, 1, 1) < 0
        || schritt_zeigen("NULL", NULL, 1, 0) < 0) {
        return -1;
    }
    spielfeld_ausgeben(&s);
    return 0;
}

/* Seite 4: der laufende Spielstand. Ohne Anfangswert ist er bis zu
   spiel_starten mit Nullen gefuellt. Ein Funktionsaufruf als Anfangswert
   ist hier nicht erlaubt. */
static Spielstand spielstand;

/* Die folgenden Funktionen ruft das Geruest auf. */

int spiel_starten(void) {
    spielstand = startzustand();
    return 0;
}

int taste_gedrueckt(SDL_Keycode taste) {
    switch (taste) {
        case SDLK_LEFT:  figur_bewegen(&spielstand, -1, 0); break;
        case SDLK_RIGHT: figur_bewegen(&spielstand, 1, 0); break;
        case SDLK_UP:    figur_bewegen(&spielstand, 0, -1); break;
        case SDLK_DOWN:  figur_bewegen(&spielstand, 0, 1); break;
        default: break;   /* andere Tasten veraendern nichts */
    }
    return 0;   /* hier kann noch kein Fehler auftreten */
}

int spiel_status(char ziel[], size_t groesse) {
    return spielstand_lesen(&spielstand, ziel, groesse);
}

void aufraeumen(void) {
    /* Fuer spaetere Kapitel vorgesehen. */
}


static int feld_zeichnen(SDL_Renderer *renderer, const Spielstand *s, Punkt p) {
    int px = RAND_LINKS + p.x * FELDGROESSE;
    int py = RAND_OBEN + p.y * FELDGROESSE;
    char z = feldzeichen(s, p);
    if (z == '#') {
        return zeichne_wand(renderer, px, py, SKALIERUNG);
    }
    /* Untergrund: ueberwacht oder normaler Boden. Objekte haben transparente
       Zellen und werden darueber gezeichnet. */
    int ergebnis = ueberwacht(p, s->waechter, ANZAHL_WAECHTER)
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
    for (int y = 0; y < HOEHE; y++) {
        for (int x = 0; x < BREITE; x++) {
            int ergebnis = feld_zeichnen(renderer, &spielstand, (Punkt){x, y});
            if (ergebnis < 0) {
                return ergebnis;   /* Fehler sofort weitergeben */
            }
        }
    }
    return 0;
}
