/* Ihr Code fuer das Kapitel Objektorientierung in C.
   Ausgangspunkt ist der Stand am Ende des Kapitels Config: Bewegung,
   Inventar als verkettete Liste im Spielstand, Leertaste (aufnehmen, Tuer),
   L (ablegen) und Spielregeln aus regeln.txt.
   Grundriss: SPIELFELD.md. Inventar und Tasten: INVENTAR.md.
   Spielregeln: REGELN.md. Neue Objekte fuer Seite 5: OBJEKTE.md.
   Vor dem Umbau einmal ausfuehren: make ablauf && ./ablauf > vorher.txt */
#include "tresorraub.h"
#include "piktogramme/piktogramme.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define SKALIERUNG 2
#define FELDGROESSE (16 * SKALIERUNG)
#define RAND_LINKS 80
#define RAND_OBEN 96

#define REGELDATEI "regeln.txt"
#define ZEILE_MAX 80          /* laengste erlaubte Zeile in regeln.txt */

#define BREITE 15
#define HOEHE 9
#define ANZAHL_WAECHTER 2

/* Das Inventar erscheint im Fenster unter dem Spielfeld. Der erste Platz
   beginnt bei INVENTAR_X, INVENTAR_Y; jeder weitere liegt FELDGROESSE
   Pixel weiter rechts. */
#define INVENTAR_X RAND_LINKS
#define INVENTAR_Y (RAND_OBEN + HOEHE * FELDGROESSE + 16)

#define NAME_LAENGE 16

/* Ergebnisse von inventar_aufnehmen */
#define AUFGENOMMEN 0
#define SCHON_VORHANDEN 1
#define INVENTAR_VOLL 2
#define FEHLER_SPEICHER (-1)
#define FEHLER_NAME (-2)


typedef struct {
    int x;
    int y;
} Punkt;

typedef struct {
    Punkt position;
    int reichweite;
} Waechter;

/* ---------- Config: Spielregeln ---------- */

typedef struct {
    int zuglimit;
    int reichweite[ANZAHL_WAECHTER];
    int inventar_groesse;
} Regeln;

Regeln regeln_standard(void) {
    Regeln r = {.zuglimit = 60, .reichweite = {1, 2}, .inventar_groesse = 3};
    return r;
}

/* Alles, was das Einlesen ueber eine Regel wissen muss. */
typedef struct {
    const char *name;
    int min;
    int max;
    int *feld;      /* hierhin kommt der gelesene Wert */
    bool gelesen;   /* stand die Regel schon in der Datei? */
} Regelangabe;

static bool nur_leerraum(const char *text) {
    for (; *text != '\0'; text++) {
        if (!isspace((unsigned char)*text)) {
            return false;
        }
    }
    return true;
}

/* Wertet eine Zeile aus. Leerzeilen und Kommentare werden uebersprungen.
   Ein gueltiger Wert kommt in das Feld seiner Regelangabe.
   Ergebnis: 0 bei Erfolg, -1 bei einem Fehler; dann steht der Grund in
   grund. Veraendert zeile. */
static int zeile_auswerten(char *zeile, Regelangabe angaben[], size_t anzahl,
                           char grund[], size_t groesse) {
    const char *anfang = zeile;
    while (isspace((unsigned char)*anfang)) {
        anfang++;
    }
    if (*anfang == '\0' || *anfang == '#') {
        return 0;
    }
    char *gleich = strchr(zeile, '=');
    if (gleich == NULL) {
        snprintf(grund, groesse, "kein \"=\" gefunden");
        return -1;
    }
    *gleich = '\0';   /* trennt Name und Wert */
    char name[32];
    char rest;
    if (sscanf(zeile, "%31s %c", name, &rest) != 1) {
        snprintf(grund, groesse, "vor \"=\" muss genau ein Name stehen");
        return -1;
    }
    Regelangabe *angabe = NULL;
    for (size_t i = 0; i < anzahl && angabe == NULL; i++) {
        if (strcmp(name, angaben[i].name) == 0) {
            angabe = &angaben[i];
        }
    }
    if (angabe == NULL) {
        snprintf(grund, groesse, "unbekannte Regel \"%s\"", name);
        return -1;
    }
    if (angabe->gelesen) {
        snprintf(grund, groesse, "Regel \"%s\" steht doppelt", name);
        return -1;
    }
    char *ende;
    long wert = strtol(gleich + 1, &ende, 10);
    if (ende == gleich + 1 || !nur_leerraum(ende)) {
        snprintf(grund, groesse, "Wert von \"%s\" ist keine ganze Zahl", name);
        return -1;
    }
    if (wert < angabe->min || wert > angabe->max) {
        snprintf(grund, groesse, "Wert von \"%s\" muss zwischen %d und %d liegen",
                 name, angabe->min, angabe->max);
        return -1;
    }
    *angabe->feld = (int)wert;
    angabe->gelesen = true;
    return 0;
}

/* Liest die Regeln aus der Datei. Nicht genannte Regeln behalten ihren
   bisherigen Wert.
   Ergebnis: 0, wenn die Datei fehlerfrei gelesen wurde; 1, wenn sie fehlt
   (Regeln unveraendert, Hinweis im Terminal); -1 bei einem Fehler (Regeln
   unveraendert, Meldung mit Zeilennummer). */
int regeln_laden(const char *dateiname, Regeln *regeln) {
    FILE *datei = fopen(dateiname, "r");
    if (datei == NULL) {
        printf("%s nicht gefunden, es gelten die Standardregeln.\n", dateiname);
        return 1;
    }
    Regeln neu = *regeln;
    Regelangabe angaben[] = {
        {"zuglimit",              1, 999, &neu.zuglimit,         false},
        {"reichweite_waechter_1", 0, 5,   &neu.reichweite[0],    false},
        {"reichweite_waechter_2", 0, 5,   &neu.reichweite[1],    false},
        {"inventar_groesse",      1, 3,   &neu.inventar_groesse, false},
    };
    size_t anzahl = sizeof(angaben) / sizeof(angaben[0]);
    char zeile[ZEILE_MAX + 2];   /* Platz fuer '\n' und '\0' */
    char grund[96] = "";
    int nummer = 0;
    int ergebnis = 0;
    while (fgets(zeile, sizeof(zeile), datei) != NULL) {
        nummer++;
        if (strchr(zeile, '\n') == NULL && !feof(datei)) {
            snprintf(grund, sizeof(grund), "Zeile ist laenger als %d Zeichen", ZEILE_MAX);
            ergebnis = -1;
            break;
        }
        if (zeile_auswerten(zeile, angaben, anzahl, grund, sizeof(grund)) < 0) {
            ergebnis = -1;
            break;
        }
    }
    if (ergebnis == 0 && ferror(datei)) {
        snprintf(grund, sizeof(grund), "Lesefehler");
        ergebnis = -1;
    }
    fclose(datei);   /* nur gelesen: kein gepufferter Schreibfehler moeglich */
    if (ergebnis < 0) {
        fprintf(stderr, "%s, Zeile %d: %s\n", dateiname, nummer, grund);
        return -1;
    }
    *regeln = neu;
    return 0;
}

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

/* ---------- Einen Eintrag anlegen ---------- */

/* Der Strukturname "Eintrag" hinter struct ist noetig, weil der Typ sich
   selbst enthaelt: naechster zeigt auf einen Eintrag desselben Typs. */
typedef struct Eintrag {
    char name[NAME_LAENGE];
    struct Eintrag *naechster;
} Eintrag;

/* Legt einen Eintrag auf dem Heap an. naechster ist NULL.
   Ergebnis: Pointer auf den neuen Eintrag; der Aufrufer besitzt ihn und muss
   ihn freigeben (oder an eine Liste uebergeben). NULL, wenn kein Speicher zu
   bekommen war oder der Name nicht in NAME_LAENGE passt; dann ist nichts
   freizugeben. */
Eintrag *eintrag_anlegen(const char *name) {
    Eintrag *neu = malloc(sizeof(*neu));
    if (neu == NULL) {
        return NULL;
    }
    int laenge = snprintf(neu->name, sizeof(neu->name), "%s", name);
    if (laenge < 0 || (size_t)laenge >= sizeof(neu->name)) {
        free(neu);   /* Fehlerpfad: eigenen Speicher selbst freigeben */
        return NULL;
    }
    neu->naechster = NULL;
    return neu;
}

/* ---------- Lesen und Aufnehmen ---------- */

typedef struct {
    Eintrag *erster;   /* NULL: Inventar ist leer */
} Inventar;

size_t inventar_anzahl(const Inventar *inventar) {
    size_t anzahl = 0;
    for (const Eintrag *e = inventar->erster; e != NULL; e = e->naechster) {
        anzahl++;
    }
    return anzahl;
}

bool inventar_enthaelt(const Inventar *inventar, const char *name) {
    for (const Eintrag *e = inventar->erster; e != NULL; e = e->naechster) {
        if (strcmp(e->name, name) == 0) {
            return true;
        }
    }
    return false;
}

/* Nimmt einen Gegenstand vorne auf. Ergebnis: AUFGENOMMEN, SCHON_VORHANDEN
   oder INVENTAR_VOLL (Spielfaelle, Inventar unveraendert) oder ein negativer
   Fehler (FEHLER_SPEICHER, FEHLER_NAME; Inventar ebenfalls unveraendert).
   Besitz: Bei AUFGENOMMEN besitzt das Inventar den neuen Eintrag. */
int inventar_aufnehmen(Inventar *inventar, const char *name, size_t hoechstens) {
    if (inventar_enthaelt(inventar, name)) {
        return SCHON_VORHANDEN;
    }
    if (inventar_anzahl(inventar) >= hoechstens) {
        return INVENTAR_VOLL;
    }
    Eintrag *neu = eintrag_anlegen(name);
    if (neu == NULL) {
        return strlen(name) >= NAME_LAENGE ? FEHLER_NAME : FEHLER_SPEICHER;
    }
    /* Erst den neuen Eintrag vollstaendig vorbereiten, dann den Kopf aendern. */
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
        printf(" %s", e->name);
    }
    printf("\n");
}

/* ---------- Entfernen und aufraeumen ---------- */

/* Entfernt den Eintrag mit diesem Namen und gibt ihn frei.
   Ergebnis: true, wenn er vorhanden war, sonst false. */
bool inventar_entfernen(Inventar *inventar, const char *name) {
    Eintrag *vorher = NULL;
    Eintrag *aktuell = inventar->erster;
    while (aktuell != NULL && strcmp(aktuell->name, name) != 0) {
        vorher = aktuell;
        aktuell = aktuell->naechster;
    }
    if (aktuell == NULL) {
        return false;
    }
    /* Erst aushaengen, dann freigeben. */
    if (vorher == NULL) {
        inventar->erster = aktuell->naechster;      /* Kopf entfernen */
    } else {
        vorher->naechster = aktuell->naechster;     /* Mitte oder Ende */
    }
    free(aktuell);
    return true;
}

/* Gibt alle Eintraege frei. Danach ist das Inventar leer. Auch fuer ein
   leeres Inventar und mehrfach hintereinander sicher. */
void inventar_leeren(Inventar *inventar) {
    Eintrag *aktuell = inventar->erster;
    while (aktuell != NULL) {
        Eintrag *naechster = aktuell->naechster;   /* merken, bevor free */
        free(aktuell);
        aktuell = naechster;
    }
    inventar->erster = NULL;
}

/* ---------- Der Spielstand besitzt das Inventar ---------- */

typedef struct {
    Punkt figur;
    int zuege;
    Inventar inventar;
    Regeln regeln;       /* Config: Zuglimit und Inventargroesse */
    Punkt ausgang;
    Punkt schluessel;
    bool schluessel_liegt;
    Punkt tuer;
    bool tuer_offen;
    Punkt tresor;
    bool beute_im_tresor;
    Punkt beute;          /* nur gueltig, wenn beute_liegt */
    bool beute_liegt;
    Punkt muenze;
    bool muenze_liegt;
    Waechter waechter[ANZAHL_WAECHTER];
} Spielstand;

Spielstand startzustand(const Regeln *regeln) {
    Spielstand s = {
        .figur = {3, 7},
        .zuege = 0,
        .inventar = {NULL},
        .ausgang = {1, 1},
        .schluessel = {3, 5},
        .schluessel_liegt = true,
        .tuer = {6, 3},
        .tuer_offen = false,
        .tresor = {13, 1},
        .beute_im_tresor = true,
        .beute = {13, 1},
        .beute_liegt = false,
        .muenze = {11, 7},
        .muenze_liegt = true,
        .regeln = *regeln,
        .waechter = {{{1, 5}, regeln->reichweite[0]}, {{9, 5}, regeln->reichweite[1]}}
    };
    return s;
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

/* Name des Gegenstands, der auf dem Feld p liegt, sonst NULL.
   Die Beute im Tresor zaehlt dazu. */
static const char *gegenstand_auf(const Spielstand *s, Punkt p) {
    if (s->schluessel_liegt && punkt_gleich(p, s->schluessel)) {
        return "Schluessel";
    }
    if (s->muenze_liegt && punkt_gleich(p, s->muenze)) {
        return "Muenze";
    }
    if (s->beute_liegt && punkt_gleich(p, s->beute)) {
        return "Beute";
    }
    if (s->beute_im_tresor && punkt_gleich(p, s->tresor)) {
        return "Beute";
    }
    return NULL;
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
    if (s->beute_liegt && punkt_gleich(p, s->beute)) {
        return 'b';
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
           "         k Schluessel, c Muenze, b Beute, E Ausgang, # Wand, ! ueberwacht\n");
}

/* Liest den Spielstand nur, deshalb const. */
static int spielstand_lesen(const Spielstand *s, char ziel[], size_t groesse) {
    if (s == NULL) {
        return -1;
    }
    return statuszeile(ziel, groesse, s->figur, s->zuege, s->regeln.zuglimit,
                       gegenstand_auf(s, s->figur));
}

/* Zaehlt einen Zug im Spielstand des Aufrufers. */
void zug_zaehlen_original(Spielstand *p) {
    p->zuege++;
}

/* Genau ein Schritt zu einem Nachbarfeld. Ergebnis: true, wenn die Figur
   gezogen ist; sonst bleibt der Spielstand unveraendert. */
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

/* Merkt nach dem Aufnehmen, dass der Gegenstand nicht mehr auf
   dem Feld liegt. */
static void vom_feld_nehmen(Spielstand *s, const char *name) {
    if (strcmp(name, "Schluessel") == 0) {
        s->schluessel_liegt = false;
    } else if (strcmp(name, "Muenze") == 0) {
        s->muenze_liegt = false;
    } else if (s->beute_liegt && punkt_gleich(s->figur, s->beute)) {
        s->beute_liegt = false;
    } else {
        s->beute_im_tresor = false;
    }
}

/* Leertaste. Liegt auf dem Feld der Figur ein Gegenstand, wird er
   aufgenommen. Sonst wird, falls die Figur neben der verschlossenen Tuer
   steht, die Tuer mit dem Schluessel geoeffnet. Nur eine erfolgreiche
   Handlung zaehlt als Zug.
   Ergebnis: 0, auch bei Spielfaellen; negativ bei einem Programmfehler
   (kein Speicher). Der Spielstand ist dann unveraendert. */
int handeln(Spielstand *s) {
    const char *name = gegenstand_auf(s, s->figur);
    if (name != NULL) {
        int ergebnis = inventar_aufnehmen(&s->inventar, name,
                                          (size_t)s->regeln.inventar_groesse);
        if (ergebnis < 0) {
            printf("Fehler: %s konnte nicht aufgenommen werden.\n", name);
            return -1;
        }
        if (ergebnis == SCHON_VORHANDEN) {
            printf("%s ist schon im Inventar.\n", name);
        } else if (ergebnis == INVENTAR_VOLL) {
            printf("Inventar ist voll, %s bleibt liegen.\n", name);
        } else {
            vom_feld_nehmen(s, name);
            zug_zaehlen_original(s);
            printf("%s aufgenommen.\n", name);
        }
        return 0;
    }
    if (!s->tuer_offen && rasterabstand(s->figur, s->tuer) == 1) {
        if (inventar_entfernen(&s->inventar, "Schluessel")) {
            s->tuer_offen = true;
            zug_zaehlen_original(s);
            printf("Tuer geoeffnet, Schluessel verbraucht.\n");
        } else {
            printf("Ohne Schluessel bleibt die Tuer zu.\n");
        }
        return 0;
    }
    printf("Hier gibt es nichts zu tun.\n");
    return 0;
}

/* ---------- Gegenstaende ablegen ---------- */

/* Ein Feld ist frei, wenn es begehbar ist und dort weder Tuer, Tresor,
   Ausgang, Waechter, Figur noch ein Gegenstand ist. */
static bool feld_frei(const Spielstand *s, Punkt p) {
    if (!begehbar(p) || punkt_gleich(p, s->figur) || punkt_gleich(p, s->tuer)
        || punkt_gleich(p, s->tresor) || punkt_gleich(p, s->ausgang)
        || gegenstand_auf(s, p) != NULL) {
        return false;
    }
    for (size_t i = 0; i < ANZAHL_WAECHTER; i++) {
        if (punkt_gleich(p, s->waechter[i].position)) {
            return false;
        }
    }
    return true;
}

/* Erstes freies Nachbarfeld im Uhrzeigersinn ab oben. Ergebnis: true und
   das Feld in *ziel, sonst false. */
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

/* Legt den Gegenstand mit diesem Namen auf das Feld p. Ergebnis: false fuer
   einen unbekannten Namen. */
static bool auf_feld_legen(Spielstand *s, const char *name, Punkt p) {
    if (strcmp(name, "Schluessel") == 0) {
        s->schluessel = p;
        s->schluessel_liegt = true;
    } else if (strcmp(name, "Muenze") == 0) {
        s->muenze = p;
        s->muenze_liegt = true;
    } else if (strcmp(name, "Beute") == 0) {
        s->beute = p;
        s->beute_liegt = true;
    } else {
        return false;
    }
    return true;
}

/* Taste L: legt alle Gegenstaende in der Reihenfolge der Liste auf freie
   Nachbarfelder (Uhrzeigersinn ab oben) und gibt ihre Eintraege frei. Was
   keinen Platz findet, bleibt im Inventar. Zaehlt als ein Zug, wenn
   mindestens ein Gegenstand abgelegt wurde. Ergebnis: Zahl der abgelegten
   Gegenstaende. */
int ablegen(Spielstand *s) {
    if (s->inventar.erster == NULL) {
        printf("Das Inventar ist leer.\n");
        return 0;
    }
    int abgelegt = 0;
    Eintrag *vorher = NULL;
    Eintrag *aktuell = s->inventar.erster;
    while (aktuell != NULL) {
        Eintrag *naechster = aktuell->naechster;   /* merken, bevor free */
        Punkt ziel;
        if (freies_nachbarfeld(s, &ziel) && auf_feld_legen(s, aktuell->name, ziel)) {
            printf("%s auf (%d, %d) abgelegt.\n", aktuell->name, ziel.x, ziel.y);
            if (vorher == NULL) {
                s->inventar.erster = naechster;     /* aushaengen ... */
            } else {
                vorher->naechster = naechster;
            }
            free(aktuell);                          /* ... dann freigeben */
            abgelegt++;
        } else {
            printf("Kein Platz fuer %s, bleibt im Inventar.\n", aktuell->name);
            vorher = aktuell;
        }
        aktuell = naechster;
    }
    if (abgelegt > 0) {
        zug_zaehlen_original(s);
    }
    return abgelegt;
}

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
    Spielstand s = startzustand(&regeln);
    char zeile[96];
    spielfeld_ausgeben(&s);
    if (spielstand_lesen(&s, zeile, sizeof(zeile)) < 0) {
        return -1;
    }
    puts(zeile);
    inventar_leeren(&s.inventar);   /* s besitzt seine Liste */
    return 0;
}

/* Der laufende Spielstand. spiel_starten weist ihm den Startzustand zu. */
static Spielstand spielstand;

/* Die folgenden Funktionen ruft das Geruest auf. */

int spiel_starten(void) {
    inventar_leeren(&spielstand.inventar);   /* falls schon Eintraege da sind */
    Regeln regeln = regeln_standard();
    if (regeln_laden(REGELDATEI, &regeln) < 0) {
        return -1;   /* fehlerhafte Datei: Spiel startet nicht */
    }
    spielstand = startzustand(&regeln);
    return 0;
}

int taste_gedrueckt(SDL_Keycode taste) {
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
        case 'b': return zeichne_beute(renderer, px, py, SKALIERUNG);
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
    /* das Inventar unter dem Spielfeld, in der Reihenfolge der Liste */
    int x = INVENTAR_X;
    for (const Eintrag *e = spielstand.inventar.erster; e != NULL; e = e->naechster) {
        int ergebnis = 0;
        if (strcmp(e->name, "Schluessel") == 0) {
            ergebnis = zeichne_schluessel(renderer, x, INVENTAR_Y, SKALIERUNG);
        } else if (strcmp(e->name, "Beute") == 0) {
            ergebnis = zeichne_beute(renderer, x, INVENTAR_Y, SKALIERUNG);
        } else if (strcmp(e->name, "Muenze") == 0) {
            ergebnis = zeichne_muenze(renderer, x, INVENTAR_Y, SKALIERUNG);
        }
        if (ergebnis < 0) {
            return ergebnis;
        }
        x += FELDGROESSE;
    }
    return 0;
}
