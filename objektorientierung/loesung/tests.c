/* Tests zur Musterloesung "Objektorientierung in C". Prueft Objektarten,
   Startzustand, alle Methoden ueber taste_gedrueckt, Geheimtuer und zweite
   Muenze, Speichern und Laden (Format Version 2), Speicherfehler und
   Zeichnen. Den Vergleich mit dem Stand vor dem
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

#define TESTSTAND "test_spielstand.txt"

/* Gueltiger Spielstand Version 2 nach dem Start. */
static const char *START_V2[] = {
    "TRESORRAUB 2", "FIGUR 3 7", "ZUEGE 0", "OBJEKTE 8",
    "Tuer 6 3 1 0 -1", "Tresor 13 1 1 0 4", "Schluessel 3 5 1 0 -1",
    "Muenze 11 7 1 0 -1", "Beute 13 1 0 0 -1", "Ausgang 1 1 1 0 -1",
    "Geheimtuer 6 6 1 0 -1", "Muenze 12 7 1 0 -1", "INVENTAR 0", "ENDE"
};

/* Schreibt START_V2, Zeile nr durch ersatz ersetzt (nr < 0: unveraendert),
   und laedt die Datei in spielstand. */
static int lade_variante(int nr, const char *ersatz) {
    FILE *f = fopen(TESTSTAND, "w");
    if (f == NULL) {
        exit(1);
    }
    for (int i = 0; i < 14; i++) {
        fprintf(f, "%s\n", i == nr ? ersatz : START_V2[i]);
    }
    fclose(f);
    return spielstand_laden(&spielstand, TESTSTAND);
}

#define SICHERUNG "regeln.txt.testsicherung"

int main(void) {
    bool gesichert = rename(REGELDATEI, SICHERUNG) == 0;
    bool stand_gesichert = rename(SPIELSTANDDATEI, SPIELSTANDDATEI ".testsicherung") == 0;
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

    /* Speichern und Laden, Format Version 2 */
    PRUEFE(spiel_starten() == 0);
    PRUEFE(lade_variante(-1, "") == 0 && spielstand.zuege == 0);
    PRUEFE(spielstand_speichern(&spielstand, TESTSTAND) == 0);
    {
        FILE *f = fopen(TESTSTAND, "r");
        char zeile[64];
        bool gleich = f != NULL;
        for (int i = 0; gleich && i < 14; i++) {
            gleich = fgets(zeile, sizeof(zeile), f) != NULL
                && strncmp(zeile, START_V2[i], strlen(START_V2[i])) == 0;
        }
        if (f != NULL) {
            fclose(f);
        }
        PRUEFE(gleich);   /* Startzustand gespeichert = START_V2 */
    }
    /* Rundlauf mit zwei Muenzen, offener Geheimtuer und Beute im Inventar */
    spielstand.figur = (Punkt){5, 6};
    druecke(" ");                                    /* Geheimtuer auf */
    spielstand.figur = (Punkt){11, 7};
    druecke(" ");
    spielstand.figur = (Punkt){12, 7};
    druecke(" ");
    spielstand.figur = (Punkt){13, 1};
    druecke(" ");
    als_text(&spielstand.inventar, text, sizeof(text));
    PRUEFE(strcmp(text, "Beute Muenze Muenze") == 0);
    int zuege_vorher = spielstand.zuege;
    PRUEFE(spielstand_speichern(&spielstand, TESTSTAND) == 0);
    PRUEFE(spiel_starten() == 0 && spielstand.zuege == 0);
    PRUEFE(spielstand_laden(&spielstand, TESTSTAND) == 0);
    als_text(&spielstand.inventar, text, sizeof(text));
    PRUEFE(strcmp(text, "Beute Muenze Muenze") == 0 && spielstand.zuege == zuege_vorher);
    PRUEFE(spielstand.inventar.erster->naechster->gegenstand == &spielstand.objekte[3]
           || spielstand.inventar.erster->naechster->gegenstand == &spielstand.objekte[7]);
    PRUEFE(spielstand.objekte[6].offen && feldzeichen(&spielstand, (Punkt){6, 6}) == '/');
    PRUEFE(tresor->inhalt == NULL && tresor->offen);
    /* Inhalt des Tresors wird als Objekt wiederhergestellt */
    PRUEFE(lade_variante(-1, "") == 0 && tresor->inhalt == &spielstand.objekte[4]);

    /* Abgelehnte Dateien: Spielstand bleibt */
    spielstand.figur = (Punkt){4, 7};
    struct { int nr; const char *zeile; } falsch[] = {
        {0, "TRESORRAUB 1"},            /* Version 1 */
        {3, "OBJEKTE 7"},               /* falsche Anzahl */
        {4, "Tresor 6 3 1 0 -1"},       /* falsche Art an dieser Stelle */
        {4, "Tuer 7 3 1 0 -1"},         /* Tuer nicht an ihrer Stelle */
        {6, "Schluessel 0 0 1 0 -1"},   /* in der Wand */
        {6, "Schluessel 1 5 1 0 -1"},   /* auf einem Waechter */
        {6, "Schluessel 11 7 1 0 -1"},  /* Feld schon belegt */
        {6, "Schluessel 3 5 2 0 -1"},   /* Flag */
        {5, "Tresor 13 1 1 0 8"},       /* Inhalt ausserhalb */
        {5, "Tresor 13 1 1 0 0"},       /* Inhalt nicht aufnehmbar */
        {5, "Tresor 13 1 1 0 3"},       /* Inhalt liegt auf dem Feld */
        {1, "FIGUR 6 3"},               /* in der verschlossenen Tuer */
        {2, "ZUEGE -1"},
        {2, "ZUEGE 99999999999999999999"},
        {12, "INVENTAR 1\n3"},          /* Muenze im Inventar und auf dem Feld */
        {12, "INVENTAR 2\n2\n2"},       /* doppelt (Schluessel liegt ausserdem) */
        {12, "INVENTAR 1\n0"},          /* Tuer ins Inventar */
        {13, "ENDE\nRest"},
        {13, "SCHLUSS"},
    };
    for (size_t i = 0; i < sizeof(falsch) / sizeof(falsch[0]); i++) {
        if (lade_variante(falsch[i].nr, falsch[i].zeile) != -2
            || !punkt_gleich(spielstand.figur, (Punkt){4, 7})) {
            printf("FEHLGESCHLAGEN: Variante %zu (%s) angenommen\n", i, falsch[i].zeile);
            fehler++;
        }
    }
    /* Inventar groesser als inventar_groesse: Abbruch mit Meldung */
    starte_mit_regeln("inventar_groesse = 1\n");
    PRUEFE(lade_variante(6, "Schluessel 3 5 0 0 -1\nMuenze 11 7 0 0 -1") == -2);   /* Format kaputt */
    {
        FILE *f = fopen(TESTSTAND, "w");
        for (int i = 0; f != NULL && i < 14; i++) {
            const char *z = START_V2[i];
            if (i == 6) z = "Schluessel 3 5 0 0 -1";
            if (i == 7) z = "Muenze 11 7 0 0 -1";
            if (i == 12) z = "INVENTAR 2\n2\n3";
            fprintf(f, "%s\n", z);
        }
        if (f != NULL) {
            fclose(f);
        }
        PRUEFE(spielstand_laden(&spielstand, TESTSTAND) == -2 && spielstand.inventar.erster == NULL);
        remove(REGELDATEI);
        PRUEFE(spiel_starten() == 0 && spielstand_laden(&spielstand, TESTSTAND) == 0);
        als_text(&spielstand.inventar, text, sizeof(text));
        PRUEFE(strcmp(text, "Schluessel Muenze") == 0);
    }
    /* Speicherfehler beim Aufbau des Inventars: nichts uebernommen */
    PRUEFE(spiel_starten() == 0);
    fehlschlag_ab = 1;
    PRUEFE(spielstand_laden(&spielstand, TESTSTAND) == -3);
    PRUEFE(spielstand.inventar.erster == NULL && spielstand.objekte[2].auf_feld);
    fehlschlag_ab = -1;
    /* Fehlende Datei */
    remove(TESTSTAND);
    PRUEFE(spielstand_laden(&spielstand, TESTSTAND) == -1);
    /* F5 und F9 ueber die Taste, ohne Zug */
    druecke("hh");
    PRUEFE(taste_gedrueckt(SDLK_F5) == 0 && spielstand.zuege == 2);
    druecke("h");
    PRUEFE(taste_gedrueckt(SDLK_F9) == 0 && spielstand.zuege == 2);
    remove(SPIELSTANDDATEI);

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

    remove(TESTSTAND);
    remove(SPIELSTANDDATEI);
    if (gesichert) {
        rename(SICHERUNG, REGELDATEI);
    }
    if (stand_gesichert) {
        rename(SPIELSTANDDATEI ".testsicherung", SPIELSTANDDATEI);
    }
    if (fehler == 0) {
        printf("Alle Tests bestanden.\n");
    }
    return fehler == 0 ? 0 : 1;
}
