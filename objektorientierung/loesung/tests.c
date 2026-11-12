/* Tests zur Musterloesung "Objektorientierung in C". Prueft Objektarten,
   Startzustand, alle Methoden ueber taste_gedrueckt, Geheimtuer und zweite
   Muenze, Speicherfehler und Zeichnen. Den Vergleich mit dem Stand vor dem
   Umbau leistet ./ablauf (siehe README.md). */
#include <SDL.h>   /* vor dem Ersetzen von malloc einbinden */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* malloc ersetzen, um Speicherfehler kontrolliert auszuloesen. */
static int fehlschlag_ab = -1;
static void *test_malloc(size_t groesse) {
    if (fehlschlag_ab == 0) {
        fehlschlag_ab = -1;
        return NULL;
    }
    if (fehlschlag_ab > 0) {
        fehlschlag_ab--;
    }
    return malloc(groesse);
}
#define malloc test_malloc
#include "tresorraub.c"
#undef malloc

static int fehler = 0;
#define PRUEFE(bedingung) do { \
        if (!(bedingung)) { \
            printf("FEHLGESCHLAGEN (Zeile %d): %s\n", __LINE__, #bedingung); \
            fehler++; \
        } \
    } while (0)

static void als_text(const Inventar *inv, char ziel[], size_t groesse) {
    ziel[0] = '\0';
    for (const Eintrag *e = inv->erster; e != NULL; e = e->naechster) {
        strncat(ziel, e->gegenstand->art->name, groesse - strlen(ziel) - 1);
        if (e->naechster != NULL) {
            strncat(ziel, " ", groesse - strlen(ziel) - 1);
        }
    }
}

static bool zeile_ist(int y, const char *erwartet) {
    for (int x = 0; x < BREITE; x++) {
        if (feldzeichen(&spielstand, (Punkt){x, y}) != erwartet[x]) {
            printf("  Zeile %d: erwartet %s\n", y, erwartet);
            return false;
        }
    }
    return true;
}

static void druecke(const char *tasten) {
    for (; *tasten != '\0'; tasten++) {
        SDL_Keycode t = *tasten == 'r' ? SDLK_RIGHT : *tasten == 'l' ? SDLK_LEFT
                      : *tasten == 'h' ? SDLK_UP : *tasten == 'u' ? SDLK_DOWN
                      : *tasten == ' ' ? SDLK_SPACE : SDLK_l;   /* 'L' */
        PRUEFE(taste_gedrueckt(t) == 0);
    }
}

static void starte_mit_regeln(const char *inhalt) {
    FILE *f = fopen(REGELDATEI, "w");
    if (f == NULL) {
        exit(1);
    }
    fputs(inhalt, f);
    fclose(f);
    PRUEFE(spiel_starten() == 0);
}

#define SICHERUNG "regeln.txt.testsicherung"

int main(void) {
    bool gesichert = rename(REGELDATEI, SICHERUNG) == 0;
    char text[160];

    /* Objektarten: vollstaendig beschrieben */
    const Objektart *arten[] = {&ART_SCHLUESSEL, &ART_MUENZE, &ART_BEUTE, &ART_TUER,
                                &ART_TRESOR, &ART_AUSGANG, &ART_GEHEIMTUER};
    for (size_t i = 0; i < sizeof(arten) / sizeof(arten[0]); i++) {
        PRUEFE(arten[i]->name != NULL && arten[i]->bild != NULL && arten[i]->bild_offen != NULL);
        PRUEFE(!arten[i]->aufnehmbar || arten[i]->benutzen == gegenstand_benutzen);
    }

    /* Startzustand (ohne regeln.txt: Standardregeln) */
    remove(REGELDATEI);
    PRUEFE(spiel_starten() == 0);
    PRUEFE(spielstand.anzahl_objekte == 8);
    PRUEFE(zeile_ist(1, "#E....#......T#"));
    PRUEFE(zeile_ist(3, "#.....D..!....#"));
    PRUEFE(zeile_ist(5, "#W!k..#!!W!!..#"));
    PRUEFE(zeile_ist(6, "#!....#.!!!...#"));   /* Geheimtuer sieht aus wie Wand */
    PRUEFE(zeile_ist(7, "#..@..#..!.cc.#"));
    const Objekt *tresor = &spielstand.objekte[1];
    PRUEFE(tresor->art == &ART_TRESOR && tresor->inhalt != NULL
           && tresor->inhalt->art == &ART_BEUTE && !tresor->inhalt->auf_feld);
    PRUEFE(spiel_status(text, sizeof(text)) == 0
           && strcmp(text, "Position (3, 7) | Zuege 0/60") == 0);

    /* Tuer ohne Schluessel, Wand, Schluessel aufnehmen */
    druecke(" u");                                   /* nichts; Wand */
    PRUEFE(spielstand.zuege == 0);
    druecke("rrhhhh");                               /* (5, 3) */
    druecke("r");                                    /* Tuer zu */
    PRUEFE(punkt_gleich(spielstand.figur, (Punkt){5, 3}) && spielstand.zuege == 6);
    druecke(" ");                                    /* ohne Schluessel */
    PRUEFE(!spielstand.objekte[0].offen && spielstand.zuege == 6);
    druecke("uull");                                 /* (3, 5) */
    PRUEFE(spiel_status(text, sizeof(text)) == 0 && strstr(text, "Hier liegt: Schluessel") != NULL);
    druecke(" ");
    PRUEFE(spielstand.zuege == 11 && !spielstand.objekte[2].auf_feld);
    PRUEFE(inventar_finden(&spielstand.inventar, &ART_SCHLUESSEL) == &spielstand.objekte[2]);
    druecke(" ");                                    /* nichts mehr da */
    PRUEFE(spielstand.zuege == 11);

    /* Tuer oeffnen: Schluessel verbraucht */
    druecke("rhrh");                                 /* (5, 3) ueber (4,5),(4,4),(5,4) */
    PRUEFE(punkt_gleich(spielstand.figur, (Punkt){5, 3}));
    druecke(" ");
    PRUEFE(spielstand.objekte[0].offen && spielstand.inventar.erster == NULL);
    PRUEFE(feldzeichen(&spielstand, (Punkt){6, 3}) == '/');
    druecke("r ");                                   /* auf der Tuer: nichts */
    PRUEFE(punkt_gleich(spielstand.figur, (Punkt){6, 3}) && spielstand.zuege == 17);

    /* Muenze, Tresor */
    druecke("rrrrruuuu ");                           /* (11, 7) */
    druecke("hhhhrrhh ");                            /* (13, 1) */
    PRUEFE(tresor->inhalt == NULL && tresor->offen);
    PRUEFE(feldzeichen(&spielstand, (Punkt){13, 1}) == '@');
    als_text(&spielstand.inventar, text, sizeof(text));
    PRUEFE(strcmp(text, "Beute Muenze") == 0);
    druecke(" ");                                    /* Tresor leer */
    PRUEFE(spielstand.zuege == 36);

    /* Ablegen und wieder aufnehmen */
    druecke("L");
    PRUEFE(spielstand.inventar.erster == NULL && spielstand.zuege == 37);
    PRUEFE(feldzeichen(&spielstand, (Punkt){13, 2}) == 'b');
    PRUEFE(feldzeichen(&spielstand, (Punkt){12, 2}) == 'c');
    druecke("L");                                    /* leer: kein Zug */
    PRUEFE(spielstand.zuege == 37);
    druecke("u l ");
    als_text(&spielstand.inventar, text, sizeof(text));
    PRUEFE(strcmp(text, "Muenze Beute") == 0 && spielstand.zuege == 41);
    PRUEFE(feldzeichen(&spielstand, (Punkt){13, 1}) == 't');

    /* Zweite Muenze: eigene Instanz, kein neuer Code */
    spielstand.figur = (Punkt){12, 7};
    druecke(" ");
    als_text(&spielstand.inventar, text, sizeof(text));
    PRUEFE(strcmp(text, "Muenze Muenze Beute") == 0);
    PRUEFE(spielstand.inventar.erster->gegenstand != spielstand.inventar.erster->naechster->gegenstand);

    /* Geheimtuer: sieht wie Wand aus, blockiert, oeffnet ohne Schluessel */
    PRUEFE(spiel_starten() == 0);
    PRUEFE(feldzeichen(&spielstand, (Punkt){6, 6}) == '#');
    spielstand.figur = (Punkt){5, 6};
    druecke("r");
    PRUEFE(punkt_gleich(spielstand.figur, (Punkt){5, 6}) && spielstand.zuege == 0);
    druecke(" ");
    PRUEFE(feldzeichen(&spielstand, (Punkt){6, 6}) == '/' && spielstand.zuege == 1);
    druecke("rr");
    PRUEFE(punkt_gleich(spielstand.figur, (Punkt){7, 6}) && spielstand.zuege == 3);
    druecke(" ");                                    /* auf/neben offener Geheimtuer: nichts */
    PRUEFE(spielstand.zuege == 3);
    /* Schluessel auf eigenem Feld geht vor der Tuer daneben */
    spielstand.objekte[2].position = (Punkt){5, 3};
    spielstand.figur = (Punkt){5, 3};
    druecke(" ");
    PRUEFE(!spielstand.objekte[2].auf_feld && !spielstand.objekte[0].offen);

    /* Config wirkt weiter: inventar_groesse 1 */
    starte_mit_regeln("inventar_groesse = 1\n");
    spielstand.figur = (Punkt){11, 7};
    druecke(" ");
    spielstand.figur = (Punkt){12, 7};
    druecke(" ");
    PRUEFE(inventar_anzahl(&spielstand.inventar) == 1 && spielstand.objekte[7].auf_feld);
    spielstand.figur = (Punkt){13, 1};
    druecke(" ");                                    /* Inventar voll: Tresor bleibt zu */
    PRUEFE(tresor->inhalt != NULL && !tresor->offen);
    remove(REGELDATEI);

    /* Speicherfehler: Spielstand unveraendert, Fehler wird gemeldet */
    PRUEFE(spiel_starten() == 0);
    spielstand.figur = (Punkt){13, 1};
    fehlschlag_ab = 0;
    PRUEFE(taste_gedrueckt(SDLK_SPACE) < 0);
    PRUEFE(tresor->inhalt != NULL && !tresor->offen && spielstand.zuege == 0
           && spielstand.inventar.erster == NULL);
    spielstand.figur = (Punkt){3, 5};
    fehlschlag_ab = 0;
    PRUEFE(handeln(&spielstand) < 0 && spielstand.objekte[2].auf_feld);

    /* text_ausgeben, zeichne, andere Tasten, aufraeumen */
    PRUEFE(text_ausgeben() == 0);
    PRUEFE(taste_gedrueckt(SDLK_x) == 0);
    druecke(" ");
    SDL_Surface *flaeche = SDL_CreateRGBSurfaceWithFormat(0, 640, 480, 32, SDL_PIXELFORMAT_RGBA32);
    SDL_Renderer *renderer = flaeche != NULL ? SDL_CreateSoftwareRenderer(flaeche) : NULL;
    PRUEFE(renderer != NULL);
    if (renderer != NULL) {
        PRUEFE(zeichne(renderer) == 0);
        SDL_DestroyRenderer(renderer);
    }
    SDL_FreeSurface(flaeche);
    aufraeumen();
    PRUEFE(spielstand.inventar.erster == NULL);
    aufraeumen();

    if (gesichert) {
        rename(SICHERUNG, REGELDATEI);
    }
    if (fehler == 0) {
        printf("Alle Tests bestanden.\n");
    }
    return fehler == 0 ? 0 : 1;
}
