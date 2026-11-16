/* Tests zur Musterloesung. Prueft die erwarteten Werte, die auf den
   Seiten des Kapitels in den Selbstkontrollen genannt werden. */
#include "tresorraub.c"

static int fehler = 0;
#define PRUEFE(bedingung) do { \
        if (!(bedingung)) { \
            printf("FEHLGESCHLAGEN (Zeile %d): %s\n", __LINE__, #bedingung); \
            fehler++; \
        } \
    } while (0)

static bool status_ist(const char *erwartet) {
    char zeile[64];
    return spiel_status(zeile, sizeof(zeile)) == 0 && strcmp(zeile, erwartet) == 0;
}

static bool position_ist(const Spielstand *s, int x, int y, int zuege) {
    return s->figur.x == x && s->figur.y == y && s->zuege == zuege;
}

int main(void) {
    /* Seite 4, Abschnitt 1: vor spiel_starten ist der Spielstand mit
       Nullen gefuellt (statische Speicherdauer). Muss vor allen anderen
       Aufrufen stehen. */
    PRUEFE(status_ist("Position (0, 0) | Zuege 0/60"));

    /* Seite 2: Kopie und Original */
    Spielstand s = startzustand();
    zug_zaehlen(s);
    PRUEFE(s.zuege == 0);
    zug_zaehlen_original(&s);
    PRUEFE(s.zuege == 1);
    Spielstand *p = &s;
    (*p).zuege++;
    PRUEFE(p->zuege == 2 && s.zuege == 2);

    /* Seite 3: vorgegebene Folge */
    s = startzustand();
    PRUEFE(position_ist(&s, 3, 7, 0));
    PRUEFE(figur_bewegen(&s, 1, 0) && position_ist(&s, 4, 7, 1));
    PRUEFE(figur_bewegen(&s, 1, 0) && position_ist(&s, 5, 7, 2));
    PRUEFE(!figur_bewegen(&s, 0, 1) && position_ist(&s, 5, 7, 2));

    /* Seite 3: Tuer verschlossen und offen */
    s.figur = (Punkt){5, 3};
    s.tuer_offen = false;
    PRUEFE(!figur_bewegen(&s, 1, 0) && position_ist(&s, 5, 3, 2));
    s.tuer_offen = true;
    PRUEFE(figur_bewegen(&s, 1, 0) && position_ist(&s, 6, 3, 3));

    /* Seite 3: ungueltige Schritte und NULL */
    PRUEFE(!figur_bewegen(&s, 2, 0) && position_ist(&s, 6, 3, 3));
    PRUEFE(!figur_bewegen(&s, 1, 1) && position_ist(&s, 6, 3, 3));
    PRUEFE(!figur_bewegen(&s, 0, 0) && position_ist(&s, 6, 3, 3));
    PRUEFE(!figur_bewegen(NULL, 1, 0));

    /* Seite 3: Rand und Wand; Ecke (1, 1) hat links und oben Wand */
    s = startzustand();
    s.figur = (Punkt){1, 1};
    PRUEFE(!figur_bewegen(&s, -1, 0) && !figur_bewegen(&s, 0, -1));
    PRUEFE(position_ist(&s, 1, 1, 0));

    /* Seite 3: lesende Funktion */
    char zeile[64];
    s = startzustand();
    PRUEFE(spielstand_lesen(&s, zeile, sizeof(zeile)) == 0);
    PRUEFE(strcmp(zeile, "Position (3, 7) | Zuege 0/60") == 0);
    PRUEFE(spielstand_lesen(NULL, zeile, sizeof(zeile)) == -1);
    char klein[8];
    PRUEFE(spielstand_lesen(&s, klein, sizeof(klein)) == -1);
    PRUEFE(position_ist(&s, 3, 7, 0));   /* Lesen veraendert nichts */

    /* Seite 4: Tastatur, Bild und Statuszeile mit demselben Spielstand */
    PRUEFE(spiel_starten() == 0);
    PRUEFE(status_ist("Position (3, 7) | Zuege 0/60"));
    PRUEFE(taste_gedrueckt(SDLK_RIGHT) == 0);
    PRUEFE(taste_gedrueckt(SDLK_RIGHT) == 0);
    PRUEFE(status_ist("Position (5, 7) | Zuege 2/60"));
    PRUEFE(taste_gedrueckt(SDLK_DOWN) == 0);            /* Wand */
    PRUEFE(status_ist("Position (5, 7) | Zuege 2/60"));
    for (int i = 0; i < 4; i++) {
        taste_gedrueckt(SDLK_UP);
    }
    PRUEFE(status_ist("Position (5, 3) | Zuege 6/60"));
    PRUEFE(taste_gedrueckt(SDLK_RIGHT) == 0);           /* Tuer verschlossen */
    PRUEFE(status_ist("Position (5, 3) | Zuege 6/60"));
    PRUEFE(taste_gedrueckt(SDLK_a) == 0);               /* andere Taste */
    PRUEFE(taste_gedrueckt(SDLK_SPACE) == 0);
    PRUEFE(status_ist("Position (5, 3) | Zuege 6/60"));
    taste_gedrueckt(SDLK_LEFT);
    PRUEFE(status_ist("Position (4, 3) | Zuege 7/60"));
    PRUEFE(feldzeichen(&spielstand, (Punkt){4, 3}) == '@');
    PRUEFE(feldzeichen(&spielstand, (Punkt){3, 7}) == '.');
    char zu_klein[16];
    PRUEFE(spiel_status(zu_klein, sizeof(zu_klein)) == -1);

    /* Seite 4: zeichne liest den laufenden Spielstand und meldet keinen
       Fehler (Software-Renderer, braucht kein Fenster) */
    SDL_Surface *flaeche = SDL_CreateRGBSurfaceWithFormat(0, 640, 480, 32, SDL_PIXELFORMAT_RGBA32);
    SDL_Renderer *renderer = flaeche != NULL ? SDL_CreateSoftwareRenderer(flaeche) : NULL;
    PRUEFE(renderer != NULL);
    if (renderer != NULL) {
        PRUEFE(zeichne(renderer) == 0);
        SDL_DestroyRenderer(renderer);
    }
    SDL_FreeSurface(flaeche);

    /* Zuglimit wird noch nicht ausgewertet: nach 60 Zuegen geht es weiter */
    for (int i = 0; i < 27; i++) {
        taste_gedrueckt(SDLK_RIGHT);
        taste_gedrueckt(SDLK_LEFT);
    }
    PRUEFE(status_ist("Position (4, 3) | Zuege 61/60"));

    /* spiel_starten setzt den laufenden Spielstand zurueck */
    PRUEFE(spiel_starten() == 0);
    PRUEFE(status_ist("Position (3, 7) | Zuege 0/60"));
    aufraeumen();

    if (fehler == 0) {
        printf("Alle Tests bestanden.\n");
    }
    return fehler == 0 ? 0 : 1;
}
