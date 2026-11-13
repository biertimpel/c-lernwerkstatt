/* VORLAGE zum Uebernehmen (Seite 4, Schritt 1): Typen fuer Objekte,
   Objektarten, Inventar und Spielstand und einige Hilfsfunktionen.

   So uebernehmen Sie die Vorlage in tresorraub.c:
   - Fuegen Sie den Text unten direkt vor dem Abschnitt
     "---------- Einen Eintrag anlegen ----------" ein.
   - Loeschen Sie dann die bisherigen Typen Eintrag, Inventar und
     Spielstand. Ihre Funktionen fuer das Inventar passen Sie in Schritt 4
     an: Ein Eintrag speichert jetzt einen Pointer auf sein Objekt statt
     eines Namens.
   - Voraussetzungen, die tresorraub.c schon enthaelt: Punkt, Waechter,
     ANZAHL_WAECHTER, Regeln, punkt_gleich und piktogramme.h.

   Diese Datei wird nicht einzeln uebersetzt. */

/* ---------- Objekte, Objektarten und Spielstand ---------- */

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

/* Das Inventar besitzt seine Eintraege, aber nicht die Gegenstaende:
   Die Objekte gehoeren dem Spielstand. */
typedef struct Eintrag {
    Objekt *gegenstand;
    struct Eintrag *naechster;
} Eintrag;

typedef struct {
    Eintrag *erster;   /* NULL: Inventar ist leer */
} Inventar;

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

/* ---------- Hilfsfunktionen fuer Objekte ---------- */

/* Haengt ein Objekt an und liefert einen Pointer darauf. */
static Objekt *objekt_hinzufuegen(Spielstand *s, const Objektart *art,
                                  int x, int y, bool auf_feld) {
    Objekt *o = &s->objekte[s->anzahl_objekte++];
    *o = (Objekt){art, {x, y}, auf_feld, false, NULL};
    return o;
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

/* Steht auf p ein Waechter? */
static bool waechter_auf(const Spielstand *s, Punkt p) {
    for (size_t i = 0; i < ANZAHL_WAECHTER; i++) {
        if (punkt_gleich(p, s->waechter[i].position)) {
            return true;
        }
    }
    return false;
}

/* Zeichen und Piktogramm eines Objekts, je nach Zustand. */
static char objekt_zeichen(const Objekt *o) {
    return o->offen ? o->art->zeichen_offen : o->art->zeichen;
}

static Piktogramm objekt_bild(const Objekt *o) {
    return o->offen ? o->art->bild_offen : o->art->bild;
}

