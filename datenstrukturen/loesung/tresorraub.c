/* MUSTERLOESUNG fuer das Kapitel Datenstrukturen - nur fuer Lehrende.
   Eine moegliche Loesung aller Aufgaben der Seiten 1 bis 5. Andere
   Datendarstellungen und Funktionszuschnitte sind ebenso vertretbar.

   Entwurfsentscheidungen:
   - Der Grundriss aendert sich nie und steht deshalb als Konstante
     ausserhalb des Spielstands. Objekte werden nicht in den Grundriss
     eingetragen, sondern mit eigener Position und eigenem Zustand
     gespeichert. Aufnehmen eines Gegenstands aendert so nur ein bool.
   - Das Inventar besteht aus drei bool-Werten, weil es hoechstens
     Schluessel, Beute und Muenze enthalten kann.
   - Das Zuglimit ist eine Regel und steht als Konstante im Programm. */
#include "tresorraub.h"
#include "piktogramme/piktogramme.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define SKALIERUNG 2
#define FELDGROESSE (16 * SKALIERUNG)
#define RAND_LINKS 80   /* (640 - 15 * 32) / 2: Spielfeld waagerecht mittig */
#define RAND_OBEN 96    /* (480 - 9 * 32) / 2: Spielfeld senkrecht mittig */

#define BREITE 15
#define HOEHE 9
#define ZUGLIMIT 60
#define ANZAHL_WAECHTER 2
#define VERSUCHE 5

/* ---------- Seite 1: Eindimensionale Arrays ---------- */

/* Anzahl der Versuche mit hoechstens ZUGLIMIT Zuegen. */
size_t versuche_im_limit(int zuege[], size_t anzahl) {
    size_t treffer = 0;
    for (size_t i = 0; i < anzahl; i++) {
        if (zuege[i] <= ZUGLIMIT) {
            treffer++;
        }
    }
    return treffer;
}

/* Kleinste Zugzahl. Vertrag: Bei anzahl == 0 gibt es kein Minimum,
   die Funktion liefert dann -1 (Zugzahlen sind nie negativ). */
int kuerzester_versuch(int zuege[], size_t anzahl) {
    if (anzahl == 0) {
        return -1;
    }
    int kuerzester = zuege[0];   /* Startwert ohne Annahme ueber die Daten */
    for (size_t i = 1; i < anzahl; i++) {
        if (zuege[i] < kuerzester) {
            kuerzester = zuege[i];
        }
    }
    return kuerzester;
}

/* Durchschnitt mit Nachkommastellen. Vertrag: Bei anzahl == 0 liefert die
   Funktion 0.0; Aufrufer sollten diesen Fall vorher selbst abfragen. */
double durchschnitt(int zuege[], size_t anzahl) {
    if (anzahl == 0) {
        return 0.0;
    }
    long summe = 0;
    for (size_t i = 0; i < anzahl; i++) {
        summe += zuege[i];
    }
    return (double)summe / (double)anzahl;
}

/* ---------- Seite 4: Strukturen (Typen werden ab Seite 2 gebraucht) ---------- */

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

bool ueberwacht(Punkt p, Waechter waechter[], size_t anzahl) {
    for (size_t i = 0; i < anzahl; i++) {
        if (rasterabstand(p, waechter[i].position) <= waechter[i].reichweite) {
            return true;
        }
    }
    return false;
}

/* ---------- Seite 2: Zeichenketten ---------- */

/* Schreibt die Statuszeile in ziel. Format: "Position (x, y) | Zuege z/60".
   Erfolg: 0. Gekuerzt, weil ziel zu klein: -1 (ziel enthaelt dann einen
   gekuerzten, aber abgeschlossenen Text). */
int statuszeile(char ziel[], size_t groesse, Punkt position, int zuege) {
    int laenge = snprintf(ziel, groesse, "Position (%d, %d) | Zuege %d/%d",
                          position.x, position.y, zuege, ZUGLIMIT);
    if (laenge < 0 || (size_t)laenge >= groesse) {
        return -1;
    }
    return 0;
}

/* ---------- Seite 3: Zweidimensionale Arrays ---------- */

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

int anzahl_zeichen(char zeichen) {
    int anzahl = 0;
    for (int y = 0; y < HOEHE; y++) {
        for (int x = 0; x < BREITE; x++) {
            if (GRUNDRISS[y][x] == zeichen) {
                anzahl++;
            }
        }
    }
    return anzahl;
}

/* ---------- Seite 5: Spielstand ---------- */

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

/* Rangfolge (hoechste zuerst): Spielfigur @, Waechter W, Tuer D/ /,
   Tresor T/t, Schluessel k, Muenze c, Ausgang E, Wand #, ueberwacht !,
   frei . */
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

int spielstand_ausgeben(Spielstand s) {
    char zeile[64];
    if (statuszeile(zeile, sizeof(zeile), s.figur, s.zuege) < 0) {
        return -1;
    }
    printf("%s\n", zeile);
    printf("Inventar:%s%s%s%s\n",
           s.hat_schluessel ? " Schluessel" : "",
           s.hat_beute ? " Beute" : "",
           s.hat_muenze ? " Muenze" : "",
           (s.hat_schluessel || s.hat_beute || s.hat_muenze) ? "" : " leer");
    printf("Tuer: %s | Tresor: %s\n", s.tuer_offen ? "offen" : "verschlossen",
           s.beute_im_tresor ? "Beute liegt im Tresor" : "leer");
    printf("Schluessel: %s | Muenze: %s\n",
           s.schluessel_liegt ? "liegt auf dem Feld" : "aufgenommen",
           s.muenze_liegt ? "liegt auf dem Feld" : "aufgenommen");
    return 0;
}

/* Seite 5, Abschnitt 3: Die Funktion arbeitet mit einer Kopie. */
void zug_zaehlen(Spielstand s) {
    s.zuege++;
    printf("  in zug_zaehlen: %d Zuege\n", s.zuege);
}

/* ---------- Hilfsfunktionen fuer die Versuche der Seiten 2 bis 4 ---------- */

/* Seite 2, Abschnitt 1: Text Zeichen fuer Zeichen bis zum Nullzeichen. */
void zeichenweise_ausgeben(char text[]) {
    for (size_t i = 0; text[i] != '\0'; i++) {
        printf("[%c]", text[i]);
    }
    printf("\n");
}

/* Seite 3, Abschnitt 2: Grundriss Feld fuer Feld. Aussen die Zeilen (y),
   innen die Spalten (x). Die Spaltenschleife endet bei BREITE, nicht bei
   BREITE + 1: Die letzte Spalte des Arrays enthaelt nur '\0'. */
void grundriss_feldweise_ausgeben(void) {
    for (int y = 0; y < HOEHE; y++) {
        for (int x = 0; x < BREITE; x++) {
            putchar(GRUNDRISS[y][x]);
        }
        putchar('\n');
    }
}

/* Seite 4, Abschnitt 1: Die Funktion aendert nur ihre Kopie. */
void position_verschieben(Punkt p) {
    p.x = p.x + 1;
}

/* Seite 4, Abschnitt 2 */
void waechter_ausgeben(Waechter waechter[], size_t anzahl) {
    for (size_t i = 0; i < anzahl; i++) {
        printf("Waechter %zu: Position (%d, %d), Reichweite %d\n", i + 1,
               waechter[i].position.x, waechter[i].position.y,
               waechter[i].reichweite);
    }
}

/* ---------- Anschluss an das Geruest ---------- */

int text_ausgeben(void) {
    printf("== Seite 1, Abschnitt 1: Vorhersagen und ausprobieren ==\n");
    int zuege[VERSUCHE] = {41, 60, 37, 52, 61};
    printf("zuege[1] = %d, zuege[4] = %d\n", zuege[1], zuege[4]);
    zuege[2] = 45;
    for (int i = 0; i < VERSUCHE; i++) {
        printf("%d ", zuege[i]);
    }
    printf("\n");
    zuege[2] = 37;   /* Beispieldaten wiederherstellen */
    int liste[6] = {1, 2};
    printf("liste: %d %d %d %d %d %d\n", liste[0], liste[1], liste[2],
           liste[3], liste[4], liste[5]);
    int kurz[] = {4, 8, 15};
    printf("int kurz[] = {4, 8, 15} hat %zu Elemente\n", sizeof(kurz) / sizeof(kurz[0]));
    /* Ein Array ohne Initialisierung wird hier bewusst nicht ausgegeben:
       Lesen unbestimmter Werte ist fuer die Musterloesung nicht sinnvoll. */

    printf("\n== Seite 1, Abschnitt 3: Groessen ==\n");
    printf("sizeof(char) = %zu, sizeof(int) = %zu, sizeof(double) = %zu\n",
           sizeof(char), sizeof(int), sizeof(double));
    size_t anzahl = sizeof(zuege) / sizeof(zuege[0]);
    printf("sizeof(zuege) = %zu, sizeof(zuege[0]) = %zu, Elemente = %zu\n",
           sizeof(zuege), sizeof(zuege[0]), anzahl);

    printf("\n== Seite 1, Abschnitt 4: Versuche auswerten ==\n");
    printf("Im Limit: %zu\n", versuche_im_limit(zuege, anzahl));
    printf("Kuerzester Versuch: %d Zuege\n", kuerzester_versuch(zuege, anzahl));
    printf("Durchschnitt: %.1f Zuege\n", durchschnitt(zuege, anzahl));
    int einer[] = {60};
    int alle_zu_lang[] = {61, 70, 99};
    printf("Ein Versuch {60}: im Limit %zu, kuerzester %d, Durchschnitt %.1f\n",
           versuche_im_limit(einer, 1), kuerzester_versuch(einer, 1), durchschnitt(einer, 1));
    printf("Alle zu lang {61, 70, 99}: im Limit %zu\n", versuche_im_limit(alle_zu_lang, 3));
    printf("Keine Versuche: kuerzester %d, Durchschnitt %.1f (vereinbarte Sonderwerte)\n",
           kuerzester_versuch(zuege, 0), durchschnitt(zuege, 0));

    printf("\n== Seite 2, Abschnitt 1: Text als Array von Zeichen ==\n");
    char gegenstand[16] = "Beute";
    printf("sizeof = %zu, strlen = %zu, [4] = '%c', [5] ist Nullzeichen: %s\n",
           sizeof(gegenstand), strlen(gegenstand), gegenstand[4],
           gegenstand[5] == '\0' ? "ja" : "nein");
    zeichenweise_ausgeben(gegenstand);
    char name[] = "Beute";
    printf("char name[] = \"Beute\": sizeof = %zu\n", sizeof(name));
    printf("strlen(\"T\xc3\xbcr\") = %zu, strlen(\"Tuer\") = %zu\n",
           strlen("T\xc3\xbcr"), strlen("Tuer"));

    printf("\n== Seite 2, Abschnitt 2: Texte vergleichen ==\n");
    char option[] = "--text";
    /* option == "--text" waere immer falsch und erzeugt eine Warnung. */
    printf("strcmp(option, \"--text\") = %d\n", strcmp(option, "--text"));
    printf("strcmp(option, \"--galerie\") ist ungleich 0: %s\n",
           strcmp(option, "--galerie") != 0 ? "ja" : "nein");

    printf("\n== Seite 2, Abschnitt 3: Statuszeile ==\n");
    char zeile[64];
    if (statuszeile(zeile, sizeof(zeile), (Punkt){14, 8}, 60) < 0) {
        return -1;
    }
    printf("Laengster Fall: \"%s\" (%zu Zeichen)\n", zeile, strlen(zeile));
    char zu_klein[12];
    if (statuszeile(zu_klein, sizeof(zu_klein), (Punkt){3, 7}, 0) < 0) {
        printf("Zu kleines Array erkannt, Inhalt gekuerzt: \"%s\"\n", zu_klein);
    }

    printf("\n== Seite 3, Abschnitt 2: Grundriss ==\n");
    for (int y = 0; y < HOEHE; y++) {
        printf("%s\n", GRUNDRISS[y]);   /* zeilenweise */
    }
    grundriss_feldweise_ausgeben();
    printf("Waende: %d, freie Felder: %d\n", anzahl_zeichen('#'), anzahl_zeichen('.'));

    printf("\n== Seite 3, Abschnitt 3: Felder abfragen ==\n");
    Punkt proben[] = {{3, 7}, {0, 0}, {14, 8}, {15, 3}, {-1, 3}, {6, 3}};
    for (size_t i = 0; i < sizeof(proben) / sizeof(proben[0]); i++) {
        printf("(%d, %d): innerhalb %s, begehbar %s\n", proben[i].x, proben[i].y,
               innerhalb(proben[i]) ? "ja" : "nein", begehbar(proben[i]) ? "ja" : "nein");
    }

    printf("\n== Seite 4, Abschnitt 1: Strukturen kopieren ==\n");
    Punkt start = {3, 7};
    Punkt zweite = start;
    zweite.x = 10;
    printf("start = (%d, %d), zweite = (%d, %d)\n", start.x, start.y, zweite.x, zweite.y);
    position_verschieben(start);
    printf("Nach position_verschieben(start): (%d, %d)\n", start.x, start.y);
    printf("punkt_gleich(start, zweite) = %d\n", punkt_gleich(start, zweite));

    printf("\n== Seite 4, Abschnitte 2 und 3: Waechter ==\n");
    Spielstand s = startzustand();
    waechter_ausgeben(s.waechter, ANZAHL_WAECHTER);
    printf("Zweiter Waechter, x = %d\n", s.waechter[1].position.x);
    printf("Rasterabstand (1, 5) -> (3, 6) = %d\n", rasterabstand((Punkt){1, 5}, (Punkt){3, 6}));

    printf("\n== Seite 5: Startzustand ==\n");
    spielfeld_ausgeben(s);
    if (spielstand_ausgeben(s) < 0) {
        return -1;
    }
    printf("sizeof(Spielstand) = %zu Bytes\n", sizeof(Spielstand));
    zug_zaehlen(s);
    printf("  nach dem Aufruf: %d Zuege\n", s.zuege);
    return 0;
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

/* Diese Funktionen ruft das Geruest im Fenster ebenfalls auf. Sie brauchen
   sie erst ab dem Kapitel "Pointer und Datenaustausch". Bis dahin lassen
   Sie die Platzhalter unveraendert. */

int spiel_starten(void) {
    return 0;
}

int taste_gedrueckt(SDL_Keycode taste) {
    (void)taste; /* Platzhalter: entfernen, sobald Sie taste verwenden. */
    return 0;
}

int spiel_status(char ziel[], size_t groesse) {
    int laenge = snprintf(ziel, groesse, "Tresorraub");
    return laenge < 0 || (size_t)laenge >= groesse ? -1 : 0;
}

void aufraeumen(void) {
}
