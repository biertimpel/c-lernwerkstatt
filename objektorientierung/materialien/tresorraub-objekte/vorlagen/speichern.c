/* VORLAGE zum Uebernehmen (Seite 4, Schritt 6): Speichern und Laden im
   Spielstandformat Version 2 (FORMAT.md).

   So uebernehmen Sie die Vorlage in tresorraub.c:
   - Ersetzen Sie den bisherigen Abschnitt "Spielstand speichern und laden"
     (Format Version 1) vollstaendig durch den Text unten, einschliesslich
     der Zeilen mit #include und #define.
   - F5 und F9 in taste_gedrueckt bleiben, wie sie sind.
   - Die Vorlage braucht diese Teile Ihres Umbaus, mit genau diesen Namen
     und Signaturen:
       void startzustand(Spielstand *s, const Regeln *regeln);
       size_t inventar_anzahl(const Inventar *inventar);
       ... inventar_aufnehmen(Inventar *inventar, Objekt *gegenstand,
                              size_t hoechstens);   liefert AUFGENOMMEN
       void inventar_leeren(Inventar *inventar);
     dazu begehbar, punkt_gleich und REGELDATEI aus dem Startmaterial und
     objekt_auf, waechter_auf und MAX_OBJEKTE aus der Vorlage objekte.c.

   Diese Datei wird nicht einzeln uebersetzt. */

/* ---------- Spielstand speichern und laden (Format Version 2) ---------- */

#include <errno.h>
#include <limits.h>

#define SPIELSTANDDATEI "spielstand.txt"
#define WORT_MAX 63            /* laengstes gelesenes Wort, ohne '\0' */
#define KEIN_OBJEKT (-1)

/* Nummer eines Objekts im Spielstand: Abstand zum Anfang des Arrays. Statt
   Pointern speichert die Datei diese Nummern. */
static int objekt_nummer(const Spielstand *s, const Objekt *o) {
    return o == NULL ? KEIN_OBJEKT : (int)(o - s->objekte);
}

/* Schreibt Figur, Zuege, den Zustand jedes Objekts und das Inventar als
   Objektnummern. Der Spielstand wird nur gelesen.
   Ergebnis: 0 bei Erfolg, -1 bei einem Dateifehler. */
int spielstand_speichern(const Spielstand *s, const char *pfad) {
    FILE *datei = fopen(pfad, "w");
    if (datei == NULL) {
        perror(pfad);
        return -1;
    }
    int ergebnis = 0;
    if (fprintf(datei, "TRESORRAUB 2\nFIGUR %d %d\nZUEGE %d\nOBJEKTE %zu\n",
                s->figur.x, s->figur.y, s->zuege, s->anzahl_objekte) < 0) {
        ergebnis = -1;
    }
    for (size_t i = 0; ergebnis == 0 && i < s->anzahl_objekte; i++) {
        const Objekt *o = &s->objekte[i];
        if (fprintf(datei, "%s %d %d %d %d %d\n", o->art->name, o->position.x,
                    o->position.y, (int)o->auf_feld, (int)o->offen,
                    objekt_nummer(s, o->inhalt)) < 0) {
            ergebnis = -1;
        }
    }
    if (ergebnis == 0 && fprintf(datei, "INVENTAR %zu\n", inventar_anzahl(&s->inventar)) < 0) {
        ergebnis = -1;
    }
    for (const Eintrag *e = s->inventar.erster; ergebnis == 0 && e != NULL; e = e->naechster) {
        if (fprintf(datei, "%d\n", objekt_nummer(s, e->gegenstand)) < 0) {
            ergebnis = -1;
        }
    }
    if (ergebnis == 0 && fprintf(datei, "ENDE\n") < 0) {
        ergebnis = -1;
    }
    if (fclose(datei) != 0) {
        ergebnis = -1;
    }
    return ergebnis;
}

/* Was die Datei ueber den Spielstand sagt, ohne Pointer. */
typedef struct {
    Punkt position;
    bool auf_feld;
    bool offen;
    int inhalt;               /* Objektnummer oder KEIN_OBJEKT */
} Objektzustand;

typedef struct {
    Punkt figur;
    int zuege;
    Objektzustand objekt[MAX_OBJEKTE];
    int anzahl_inventar;
    int inventar[MAX_OBJEKTE];   /* Objektnummern vom Listenkopf an */
} Gelesen;

static bool wort_lesen(FILE *datei, char wort[WORT_MAX + 1]) {
    return fscanf(datei, "%63s", wort) == 1;
}

static bool kennwort_lesen(FILE *datei, const char *kennwort) {
    char wort[WORT_MAX + 1];
    return wort_lesen(datei, wort) && strcmp(wort, kennwort) == 0;
}

static bool zahl_lesen(FILE *datei, int *ziel) {
    char wort[WORT_MAX + 1];
    if (!wort_lesen(datei, wort)) {
        return false;
    }
    char *ende;
    errno = 0;
    long wert = strtol(wort, &ende, 10);
    if (ende == wort || *ende != '\0' || errno == ERANGE
        || wert < INT_MIN || wert > INT_MAX) {
        return false;
    }
    *ziel = (int)wert;
    return true;
}

static bool flag_lesen(FILE *datei, bool *ziel) {
    int wert;
    if (!zahl_lesen(datei, &wert) || (wert != 0 && wert != 1)) {
        return false;
    }
    *ziel = wert == 1;
    return true;
}

/* Liest die Datei bis ENDE. vorlage liefert die erwarteten Objektarten in
   ihrer Reihenfolge. Ergebnis: false bei einem Formatfehler. */
static bool datei_lesen(FILE *datei, const Spielstand *vorlage, Gelesen *g) {
    int version;
    int anzahl;
    if (!kennwort_lesen(datei, "TRESORRAUB") || !zahl_lesen(datei, &version) || version != 2
        || !kennwort_lesen(datei, "FIGUR") || !zahl_lesen(datei, &g->figur.x)
        || !zahl_lesen(datei, &g->figur.y)
        || !kennwort_lesen(datei, "ZUEGE") || !zahl_lesen(datei, &g->zuege)
        || !kennwort_lesen(datei, "OBJEKTE") || !zahl_lesen(datei, &anzahl)
        || anzahl != (int)vorlage->anzahl_objekte) {
        return false;
    }
    for (int i = 0; i < anzahl; i++) {
        Objektzustand *z = &g->objekt[i];
        if (!kennwort_lesen(datei, vorlage->objekte[i].art->name)
            || !zahl_lesen(datei, &z->position.x) || !zahl_lesen(datei, &z->position.y)
            || !flag_lesen(datei, &z->auf_feld) || !flag_lesen(datei, &z->offen)
            || !zahl_lesen(datei, &z->inhalt)) {
            return false;
        }
    }
    if (!kennwort_lesen(datei, "INVENTAR") || !zahl_lesen(datei, &g->anzahl_inventar)
        || g->anzahl_inventar < 0 || g->anzahl_inventar > anzahl) {
        return false;
    }
    for (int i = 0; i < g->anzahl_inventar; i++) {
        if (!zahl_lesen(datei, &g->inventar[i])) {
            return false;
        }
    }
    if (!kennwort_lesen(datei, "ENDE")) {
        return false;
    }
    char rest[WORT_MAX + 1];
    return fscanf(datei, "%63s", rest) == EOF && !ferror(datei);
}

/* Uebertraegt Figur, Zuege und Objektzustaende auf s (ohne Inventar). Die
   Objektnummern sind vorher geprueft. */
static void zustand_uebertragen(Spielstand *s, const Gelesen *g) {
    s->figur = g->figur;
    s->zuege = g->zuege;
    for (size_t i = 0; i < s->anzahl_objekte; i++) {
        Objekt *o = &s->objekte[i];
        const Objektzustand *z = &g->objekt[i];
        o->position = z->position;
        o->auf_feld = z->auf_feld;
        o->offen = z->offen;
        o->inhalt = z->inhalt == KEIN_OBJEKT ? NULL : &s->objekte[z->inhalt];
    }
}

/* Prueft die Regeln aus FORMAT.md. vorlage ist ein frischer Startzustand
   mit den geltenden Regeln; er wird dabei veraendert. */
static bool gelesen_gueltig(Spielstand *vorlage, const Gelesen *g) {
    int n = (int)vorlage->anzahl_objekte;
    if (g->zuege < 0 || g->anzahl_inventar > vorlage->regeln.inventar_groesse) {
        return false;
    }
    /* Wo ist jeder Gegenstand? Hoechstens an einer Stelle. */
    int stellen[MAX_OBJEKTE] = {0};
    for (int i = 0; i < n; i++) {
        const Objektzustand *z = &g->objekt[i];
        const Objekt *start = &vorlage->objekte[i];
        if (!begehbar(z->position)) {
            return false;
        }
        if (!start->art->aufnehmbar
            && (!z->auf_feld || !punkt_gleich(z->position, start->position))) {
            return false;   /* Tueren, Tresor, Ausgang bleiben, wo sie sind */
        }
        if (z->inhalt != KEIN_OBJEKT) {
            if (z->inhalt < 0 || z->inhalt >= n || z->inhalt == i
                || !vorlage->objekte[z->inhalt].art->aufnehmbar
                || g->objekt[z->inhalt].auf_feld) {
                return false;
            }
            stellen[z->inhalt]++;
        }
        if (z->auf_feld) {
            stellen[i]++;
        }
    }
    for (int i = 0; i < g->anzahl_inventar; i++) {
        int nr = g->inventar[i];
        if (nr < 0 || nr >= n || !vorlage->objekte[nr].art->aufnehmbar) {
            return false;
        }
        stellen[nr]++;
    }
    for (int i = 0; i < n; i++) {
        if (stellen[i] > 1) {
            return false;
        }
    }
    /* Auf jedem Feld hoechstens ein Objekt, keines auf einem Waechter. */
    for (int i = 0; i < n; i++) {
        if (!g->objekt[i].auf_feld) {
            continue;
        }
        if (waechter_auf(vorlage, g->objekt[i].position)) {
            return false;
        }
        for (int j = 0; j < i; j++) {
            if (g->objekt[j].auf_feld
                && punkt_gleich(g->objekt[i].position, g->objekt[j].position)) {
                return false;
            }
        }
    }
    /* Die Figur steht auf einem Feld, das sie betreten darf. */
    zustand_uebertragen(vorlage, g);
    if (!begehbar(g->figur)) {
        return false;
    }
    const Objekt *o = objekt_auf(vorlage, g->figur);
    return o == NULL || o->art->betretbar == NULL || o->art->betretbar(o);
}

/* Laedt einen Spielstand. Alles oder nichts: Bei einem Fehler bleibt *ziel
   samt Inventar unveraendert. Gelesen wird in Daten ohne Pointer; erst
   nach allen Pruefungen werden sie auf *ziel uebertragen.
   Ergebnis: 0 bei Erfolg; -1 Dateifehler; -2 Format oder Regeln verletzt;
   -3 kein Speicher. */
int spielstand_laden(Spielstand *ziel, const char *pfad) {
    FILE *datei = fopen(pfad, "r");
    if (datei == NULL) {
        perror(pfad);
        return -1;
    }
    Spielstand vorlage;
    startzustand(&vorlage, &ziel->regeln);
    Gelesen g;
    int ergebnis = datei_lesen(datei, &vorlage, &g) ? 0 : -2;
    if (ferror(datei)) {
        ergebnis = -1;
    }
    if (fclose(datei) != 0) {
        ergebnis = -1;
    }
    if (ergebnis == 0 && g.anzahl_inventar > ziel->regeln.inventar_groesse) {
        fprintf(stderr, "%s: %d Gegenstaende im Inventar, laut %s sind hoechstens %d erlaubt.\n",
                pfad, g.anzahl_inventar, REGELDATEI, ziel->regeln.inventar_groesse);
        ergebnis = -2;
    }
    if (ergebnis == 0 && !gelesen_gueltig(&vorlage, &g)) {
        ergebnis = -2;
    }
    /* Neues Inventar zuerst vollstaendig aufbauen; es zeigt auf die Objekte
       von *ziel, die ihre Adressen behalten. */
    Inventar neu = {NULL};
    for (int i = g.anzahl_inventar - 1; ergebnis == 0 && i >= 0; i--) {
        if (inventar_aufnehmen(&neu, &ziel->objekte[g.inventar[i]],
                               (size_t)ziel->regeln.inventar_groesse) != AUFGENOMMEN) {
            ergebnis = -3;
        }
    }
    if (ergebnis != 0) {
        inventar_leeren(&neu);
        return ergebnis;
    }
    zustand_uebertragen(ziel, &g);
    inventar_leeren(&ziel->inventar);
    ziel->inventar = neu;
    return 0;
}

