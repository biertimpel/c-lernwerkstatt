/* MUSTERLOESUNG fuer das Kapitel Objektorientierung in C - nur fuer Lehrende.
   Grundlage ist der Stand am Ende von "Config".

   Entwurfsentscheidungen:
   - Jede Objektart (Schluessel, Muenze, Beute, Tuer, Tresor, Ausgang,
     Geheimtuer) ist genau einmal als static const Objektart beschrieben:
     Name, Zeichen, Piktogramme und zwei Methoden als Funktionspointer
     (benutzen fuer die Leertaste, betretbar fuer die Bewegung).
   - Ein Objekt zeigt auf seine Art und hat eigene Daten: Position,
     auf_feld, offen und inhalt (Tresor). Eine gemeinsame Struktur fuer alle
     Arten genuegt; Struktureinbettung ist nicht noetig.
   - Der Spielstand haelt alle Objekte in einem Array. Weil Objekte und
     Inventar Pointer auf Objekte enthalten, wird der Spielstand nicht mehr
     als Wert kopiert: startzustand fuellt ihn ueber einen Pointer.
   - Das Inventar speichert Pointer auf Gegenstaende statt Namen. Es besitzt
     seine Eintraege, nicht die Gegenstaende.
   - Ergebniscodes als enum (Aufnahme, Benutzung) statt #define.
   - Seite 5: Geheimtuer auf (6, 6) (im Grundriss jetzt Boden) und eine
     zweite Muenze auf (12, 7). Beide kommen ohne Aenderung am uebrigen
     Code aus. */
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

/* (6, 6) ist Boden; dort steht die Geheimtuer.
   BREITE + 1: 15 sichtbare Zeichen plus abschliessendes '\0' je Zeile. */
static const char GRUNDRISS[HOEHE][BREITE + 1] = {
    "###############",
    "#.....#.......#",
    "#.##..#..###..#",
    "#.............#",
    "#.##..#.......#",
    "#.....#.......#",
    "#.............#",
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

/* ---------- Objekte und Objektarten ---------- */

typedef struct Spielstand Spielstand;
typedef struct Objekt Objekt;

/* Alle Zeichenfunktionen der Piktogramme haben diese Signatur. */
typedef int (*Piktogramm)(SDL_Renderer *renderer, int x, int y, int skalierung);

/* Ergebnisse der Methode benutzen. Negativ: Programmfehler. */
typedef enum {
    BENUTZEN_FEHLER = -1,
    ERLEDIGT,          /* etwas ist geschehen oder wurde gemeldet */
    NICHTS_ZU_TUN      /* dieses Objekt ist nicht zustaendig */
} Benutzung;

/* Eine Objektart beschreibt, was alle Objekte dieser Art gemeinsam haben:
   Aussehen und Verhalten. Jede Art gibt es genau einmal (static const);
   jedes Objekt zeigt auf seine Art. */
typedef struct {
    const char *name;
    bool aufnehmbar;              /* kann ins Inventar */
    char zeichen;                 /* Textausgabe */
    char zeichen_offen;           /* Textausgabe, wenn offen */
    Piktogramm bild;
    Piktogramm bild_offen;
    /* Leertaste. selbst ist das Objekt, abstand sein Rasterabstand zur
       Figur. NULL: Objekt reagiert nicht. */
    Benutzung (*benutzen)(Objekt *selbst, Spielstand *s, int abstand);
    /* Darf die Figur auf das Feld? NULL: immer. */
    bool (*betretbar)(const Objekt *selbst);
} Objektart;

/* Ein Objekt ist eine Instanz seiner Art mit eigenen Daten. */
struct Objekt {
    const Objektart *art;
    Punkt position;
    bool auf_feld;     /* liegt oder steht auf dem Spielfeld */
    bool offen;        /* Tuer: geoeffnet; Tresor: aufgebrochen */
    Objekt *inhalt;    /* Tresor: enthaltener Gegenstand, sonst NULL */
};

/* ---------- Inventar: Liste von Pointern auf Gegenstaende ---------- */

/* Das Inventar besitzt seine Eintraege, aber nicht die Gegenstaende:
   Die Objekte gehoeren dem Spielstand. */
typedef struct Eintrag {
    Objekt *gegenstand;
    struct Eintrag *naechster;
} Eintrag;

typedef struct {
    Eintrag *erster;   /* NULL: Inventar ist leer */
} Inventar;

/* Ergebnisse von inventar_aufnehmen */
typedef enum {
    FEHLER_SPEICHER = -1,
    AUFGENOMMEN,
    INVENTAR_VOLL
} Aufnahme;

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

/* ---------- Der Spielstand ---------- */

#define MAX_OBJEKTE 10

/* Achtung: Objekte zeigen aufeinander (Tresor -> Beute) und das Inventar
   zeigt auf Objekte. Ein kopierter Spielstand wuerde auf die Objekte des
   Originals zeigen. Deshalb wird er nie als Wert kopiert, sondern ueber
   einen Pointer gefuellt und benutzt. */
struct Spielstand {
    Punkt figur;
    int zuege;
    Inventar inventar;
    Regeln regeln;
    Objekt objekte[MAX_OBJEKTE];
    size_t anzahl_objekte;
    Waechter waechter[ANZAHL_WAECHTER];
};

void zug_zaehlen_original(Spielstand *p) {
    p->zuege++;
}

/* ---------- Verhalten der Objektarten ---------- */

/* Nimmt einen Gegenstand ins Inventar auf und meldet das Ergebnis. Bei
   AUFGENOMMEN liegt er nicht mehr auf dem Feld, und ein Zug ist gezaehlt. */
static Aufnahme gegenstand_aufnehmen(Objekt *gegenstand, Spielstand *s) {
    const char *name = gegenstand->art->name;
    Aufnahme ergebnis = inventar_aufnehmen(&s->inventar, gegenstand,
                                           (size_t)s->regeln.inventar_groesse);
    if (ergebnis == FEHLER_SPEICHER) {
        printf("Fehler: %s konnte nicht aufgenommen werden.\n", name);
    } else if (ergebnis == INVENTAR_VOLL) {
        printf("Inventar ist voll, %s bleibt liegen.\n", name);
    } else {
        gegenstand->auf_feld = false;
        zug_zaehlen_original(s);
        printf("%s aufgenommen.\n", name);
    }
    return ergebnis;
}

/* Gegenstaende: aufnehmen, wenn die Figur auf ihrem Feld steht. */
static Benutzung gegenstand_benutzen(Objekt *selbst, Spielstand *s, int abstand) {
    if (abstand != 0) {
        return NICHTS_ZU_TUN;
    }
    return gegenstand_aufnehmen(selbst, s) == FEHLER_SPEICHER ? BENUTZEN_FEHLER : ERLEDIGT;
}

static const Objektart ART_SCHLUESSEL = {
    "Schluessel", true, 'k', 'k', zeichne_schluessel, zeichne_schluessel,
    gegenstand_benutzen, NULL
};
static const Objektart ART_MUENZE = {
    "Muenze", true, 'c', 'c', zeichne_muenze, zeichne_muenze,
    gegenstand_benutzen, NULL
};
static const Objektart ART_BEUTE = {
    "Beute", true, 'b', 'b', zeichne_beute, zeichne_beute,
    gegenstand_benutzen, NULL
};

/* Tuer: vom Nachbarfeld aus mit dem Schluessel oeffnen; der Schluessel
   wird verbraucht. */
static Benutzung tuer_benutzen(Objekt *selbst, Spielstand *s, int abstand) {
    if (abstand != 1 || selbst->offen) {
        return NICHTS_ZU_TUN;
    }
    Objekt *schluessel = inventar_finden(&s->inventar, &ART_SCHLUESSEL);
    if (schluessel == NULL) {
        printf("Ohne Schluessel bleibt die Tuer zu.\n");
        return ERLEDIGT;
    }
    inventar_entfernen(&s->inventar, schluessel);
    selbst->offen = true;
    zug_zaehlen_original(s);
    printf("Tuer geoeffnet, Schluessel verbraucht.\n");
    return ERLEDIGT;
}

/* Tueren aller Arten: nur offen betretbar. */
static bool tuer_betretbar(const Objekt *selbst) {
    return selbst->offen;
}

/* Tresor: Steht die Figur darauf, wird der Inhalt aufgenommen. */
static Benutzung tresor_benutzen(Objekt *selbst, Spielstand *s, int abstand) {
    if (abstand != 0 || selbst->inhalt == NULL) {
        return NICHTS_ZU_TUN;
    }
    Aufnahme ergebnis = gegenstand_aufnehmen(selbst->inhalt, s);
    if (ergebnis == FEHLER_SPEICHER) {
        return BENUTZEN_FEHLER;
    }
    if (ergebnis == AUFGENOMMEN) {
        selbst->inhalt = NULL;
        selbst->offen = true;
    }
    return ERLEDIGT;
}

/* Geheimtuer: sieht geschlossen wie eine Wand aus und geht vom
   Nachbarfeld aus ohne Schluessel auf. */
static Benutzung geheimtuer_benutzen(Objekt *selbst, Spielstand *s, int abstand) {
    if (abstand != 1 || selbst->offen) {
        return NICHTS_ZU_TUN;
    }
    selbst->offen = true;
    zug_zaehlen_original(s);
    printf("Geheimtuer geoeffnet.\n");
    return ERLEDIGT;
}

static const Objektart ART_TUER = {
    "Tuer", false, 'D', '/', zeichne_tuer_geschlossen, zeichne_tuer_offen,
    tuer_benutzen, tuer_betretbar
};
static const Objektart ART_TRESOR = {
    "Tresor", false, 'T', 't', zeichne_tresor_geschlossen, zeichne_tresor_leer,
    tresor_benutzen, NULL
};
static const Objektart ART_AUSGANG = {
    "Ausgang", false, 'E', 'E', zeichne_ausgang, zeichne_ausgang, NULL, NULL
};
static const Objektart ART_GEHEIMTUER = {
    "Geheimtuer", false, '#', '/', zeichne_wand, zeichne_tuer_offen,
    geheimtuer_benutzen, tuer_betretbar
};

/* ---------- Spielstand anlegen und befragen ---------- */

/* Haengt ein Objekt an und liefert einen Pointer darauf. */
static Objekt *objekt_hinzufuegen(Spielstand *s, const Objektart *art,
                                  int x, int y, bool auf_feld) {
    Objekt *o = &s->objekte[s->anzahl_objekte++];
    *o = (Objekt){art, {x, y}, auf_feld, false, NULL};
    return o;
}

/* Fuellt *s mit dem Startzustand. Die Reihenfolge der Objekte legt fest,
   welches Zeichen gewinnt, wenn mehrere auf einem Feld stehen. */
void startzustand(Spielstand *s, const Regeln *regeln) {
    *s = (Spielstand){
        .figur = {3, 7},
        .regeln = *regeln,
        .waechter = {{{1, 5}, regeln->reichweite[0]}, {{9, 5}, regeln->reichweite[1]}}
    };
    objekt_hinzufuegen(s, &ART_TUER, 6, 3, true);
    Objekt *tresor = objekt_hinzufuegen(s, &ART_TRESOR, 13, 1, true);
    objekt_hinzufuegen(s, &ART_SCHLUESSEL, 3, 5, true);
    objekt_hinzufuegen(s, &ART_MUENZE, 11, 7, true);
    tresor->inhalt = objekt_hinzufuegen(s, &ART_BEUTE, 13, 1, false);
    objekt_hinzufuegen(s, &ART_AUSGANG, 1, 1, true);
    objekt_hinzufuegen(s, &ART_GEHEIMTUER, 6, 6, true);
    objekt_hinzufuegen(s, &ART_MUENZE, 12, 7, true);
}

/* Erstes Objekt auf dem Feld p, sonst NULL. */
static const Objekt *objekt_auf(const Spielstand *s, Punkt p) {
    for (size_t i = 0; i < s->anzahl_objekte; i++) {
        const Objekt *o = &s->objekte[i];
        if (o->auf_feld && punkt_gleich(o->position, p)) {
            return o;
        }
    }
    return NULL;
}

/* Gegenstand, den die Figur auf p aufnehmen koennte, sonst NULL. Der
   Inhalt eines Tresors zaehlt dazu. */
static const Objekt *gegenstand_auf(const Spielstand *s, Punkt p) {
    const Objekt *o = objekt_auf(s, p);
    if (o == NULL) {
        return NULL;
    }
    if (o->art->aufnehmbar) {
        return o;
    }
    return o->inhalt;
}

static char objekt_zeichen(const Objekt *o) {
    return o->offen ? o->art->zeichen_offen : o->art->zeichen;
}

static Piktogramm objekt_bild(const Objekt *o) {
    return o->offen ? o->art->bild_offen : o->art->bild;
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

static bool waechter_auf(const Spielstand *s, Punkt p) {
    for (size_t i = 0; i < ANZAHL_WAECHTER; i++) {
        if (punkt_gleich(p, s->waechter[i].position)) {
            return true;
        }
    }
    return false;
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

static int spielstand_lesen(const Spielstand *s, char ziel[], size_t groesse) {
    if (s == NULL) {
        return -1;
    }
    const Objekt *hier = gegenstand_auf(s, s->figur);
    return statuszeile(ziel, groesse, s->figur, s->zuege, s->regeln.zuglimit,
                       hier == NULL ? NULL : hier->art->name);
}

/* ---------- Bewegen, Leertaste, Ablegen ---------- */

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
    if (!begehbar(ziel)) {
        return false;
    }
    const Objekt *o = objekt_auf(s, ziel);
    if (o != NULL && o->art->betretbar != NULL && !o->art->betretbar(o)) {
        return false;
    }
    s->figur = ziel;
    zug_zaehlen_original(s);
    return true;
}

/* Leertaste. Zuerst sind die Objekte auf dem Feld der Figur an der Reihe,
   dann die auf den Nachbarfeldern. Das erste zustaendige Objekt handelt.
   Ergebnis: 0, auch bei Spielfaellen; negativ bei einem Programmfehler. */
int handeln(Spielstand *s) {
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
    if (GRUNDRISS[p.y][p.x] == '#') {
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
        return zeichne_spielfigur_froh(renderer, px, py, SKALIERUNG);
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
    return 0;
}
