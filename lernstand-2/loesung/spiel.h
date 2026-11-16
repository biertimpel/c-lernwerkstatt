/* Typen des Tresorraubs: Objektarten, Objekte, Inventar, Spielstand. */
#ifndef SPIEL_H
#define SPIEL_H

#include "spielfeld.h"
#include "regeln.h"
#include <SDL.h>

#define MAX_OBJEKTE 10

typedef struct Spielstand Spielstand;
typedef struct Objekt Objekt;

/* Alle Zeichenfunktionen der Piktogramme haben diese Signatur. */
typedef int (*Piktogramm)(SDL_Renderer *renderer, int x, int y, int skalierung);

/* Wie steht das Spiel? */
typedef enum {
    LAEUFT,
    GEWONNEN,          /* mit der Beute am Ausgang */
    ENTDECKT,          /* in einem ueberwachten Feld */
    ZEIT_ABGELAUFEN    /* Zuglimit erreicht, ohne zu gewinnen */
} Spielzustand;

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
    /* Die Figur hat das Feld gerade betreten. NULL: nichts geschieht. */
    void (*betreten)(Objekt *selbst, Spielstand *s);
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

/* Ergebnisse von inventar_aufnehmen */
typedef enum {
    FEHLER_SPEICHER = -1,
    AUFGENOMMEN,
    INVENTAR_VOLL
} Aufnahme;

/* Achtung: Objekte zeigen aufeinander (Tresor -> Beute) und das Inventar
   zeigt auf Objekte. Ein kopierter Spielstand wuerde auf die Objekte des
   Originals zeigen. Deshalb wird er nie als Wert kopiert, sondern ueber
   einen Pointer gefuellt und benutzt. */
struct Spielstand {
    Punkt figur;
    int zuege;
    Spielzustand zustand;
    bool licht_an;
    Uint64 zeit_ms;             /* zuletzt gemeldete Zeit (spiel_aktualisieren) */
    Uint64 licht_aus_seit_ms;   /* gilt, solange licht_an falsch ist */
    Inventar inventar;
    Regeln regeln;
    Objekt objekte[MAX_OBJEKTE];
    size_t anzahl_objekte;
    Waechter waechter[ANZAHL_WAECHTER];
};

#endif
