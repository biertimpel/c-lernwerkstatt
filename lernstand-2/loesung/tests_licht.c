/* Tests fuer die Musterloesung des Lernstandschecks II (nur fuer Lehrende).
   Prueft Lichtschalter, Licht, Zeit, Positionen aus regeln.txt und
   Speichern im Dunkeln. Die Namen sind die der Musterloesung; Loesungen der
   Studierenden haben eigene Namen. Bauen: make tests_licht */
#include "tresorraub.h"
#include "spielfeld.h"
#include "regeln.h"
#include "spiel.h"
#include "inventar.h"
#include "objekte.h"
#include "spiellogik.h"
#include "speicherstand.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static int geprueft = 0;
static int fehler = 0;

#define PRUEFE(bedingung) do { \
        geprueft++; \
        if (!(bedingung)) { \
            printf("  FEHLGESCHLAGEN (tests_licht.c, Zeile %d): %s\n", __LINE__, #bedingung); \
            fehler++; \
        } \
    } while (0)

static void schreibe(const char *name, const char *inhalt) {
    FILE *f = fopen(name, "w");
    if (f == NULL) {
        printf("Testdatei %s kann nicht angelegt werden.\n", name);
        exit(1);
    }
    fputs(inhalt, f);
    fclose(f);
}

static void neuer_stand(Spielstand *s) {
    Regeln r = regeln_standard();
    startzustand(s, &r);
}

static void teste_regeln(void) {
    printf("regeln: Positionen und Dauer\n");
    Regeln r = regeln_standard();
    PRUEFE(r.licht_dauer == 5 && punkt_gleich(r.schluessel, (Punkt){3, 5})
           && punkt_gleich(r.lichtschalter, (Punkt){5, 7})
           && punkt_gleich(r.waechter[0], (Punkt){1, 5}));
    schreibe("test_regeln.txt", "waechter_1_y = 6\nschluessel_x = 1\nschluessel_y = 7\nlicht_dauer = 3\n");
    PRUEFE(regeln_laden("test_regeln.txt", &r) == 0 && r.waechter[0].y == 6
           && punkt_gleich(r.schluessel, (Punkt){1, 7}) && r.licht_dauer == 3);
    r = regeln_standard();
    schreibe("test_regeln.txt", "schluessel_x = 15\n");
    PRUEFE(regeln_laden("test_regeln.txt", &r) == -1 && r.schluessel.x == 3);
    schreibe("test_regeln.txt", "licht_dauer = 0\n");
    PRUEFE(regeln_laden("test_regeln.txt", &r) == -1 && r.licht_dauer == 5);
    remove("test_regeln.txt");
}

static void teste_aufstellung(void) {
    printf("objekte: Aufstellung pruefen\n");
    Regeln r = regeln_standard();
    PRUEFE(aufstellung_pruefen(&r) == 0);
    r.waechter[0] = (Punkt){1, 6};
    r.schluessel = (Punkt){1, 7};
    PRUEFE(aufstellung_pruefen(&r) == 0);                 /* Musterloesung */
    r = regeln_standard();
    r.schluessel = (Punkt){0, 0};
    PRUEFE(aufstellung_pruefen(&r) == -1);                /* Wand */
    r = regeln_standard();
    r.lichtschalter = (Punkt){6, 3};
    PRUEFE(aufstellung_pruefen(&r) == -1);                /* auf der Tuer */
    r = regeln_standard();
    r.waechter[1] = (Punkt){3, 7};
    PRUEFE(aufstellung_pruefen(&r) == -1);                /* auf der Figur */
    r = regeln_standard();
    r.waechter[0] = (Punkt){3, 6};
    PRUEFE(aufstellung_pruefen(&r) == -1);                /* Startfeld ueberwacht */
    r = regeln_standard();
    r.lichtschalter = r.schluessel;
    PRUEFE(aufstellung_pruefen(&r) == -1);                /* zwei Objekte */
}

static void teste_schalter(void) {
    printf("Lichtschalter\n");
    Spielstand s;
    neuer_stand(&s);
    const Objekt *schalter = objekt_auf(&s, (Punkt){5, 7});
    PRUEFE(schalter != NULL && strcmp(schalter->art->name, "Lichtschalter") == 0);
    PRUEFE(s.licht_an);
    PRUEFE(handeln(&s) == 0 && s.zuege == 0 && s.licht_an);   /* zu weit weg */
    PRUEFE(figur_bewegen(&s, 1, 0) && punkt_gleich(s.figur, (Punkt){4, 7}));
    PRUEFE(!figur_bewegen(&s, 1, 0) && s.zuege == 1);         /* nicht betretbar */
    s.zeit_ms = 1000;
    PRUEFE(handeln(&s) == 0 && !s.licht_an && s.licht_aus_seit_ms == 1000 && s.zuege == 2);
    char zeile[128];
    PRUEFE(spielstand_lesen(&s, zeile, sizeof(zeile)) == 0 && strstr(zeile, "| Licht aus") != NULL);
    PRUEFE(feldzeichen(&s, (Punkt){5, 7}) == 'S');
}

static void teste_dunkel(void) {
    printf("Licht und Zeit\n");
    Spielstand s;
    char zeile[128];
    neuer_stand(&s);
    s.figur = (Punkt){4, 7};
    PRUEFE(zeit_fortschreiben(&s, 1000) == 0 && s.zeit_ms == 1000);
    handeln(&s);                                   /* Licht aus bei 1000 */
    s.figur = (Punkt){3, 5};
    PRUEFE(figur_bewegen(&s, -1, 0) && s.zustand == LAEUFT);      /* (2, 5) im Dunkeln */
    PRUEFE(zeit_fortschreiben(&s, 5999) == 0 && !s.licht_an);
    PRUEFE(zeit_fortschreiben(&s, 6000) == 1 && s.licht_an && s.zustand == ENTDECKT);
    PRUEFE(spielstand_lesen(&s, zeile, sizeof(zeile)) == 0 && strstr(zeile, "WASTED: entdeckt") != NULL);
    /* Licht geht an, Figur steht sicher */
    neuer_stand(&s);
    s.figur = (Punkt){4, 7};
    handeln(&s);
    PRUEFE(zeit_fortschreiben(&s, 5000) == 1 && s.licht_an && s.zustand == LAEUFT);
    PRUEFE(zeit_fortschreiben(&s, 9000) == 0);
    /* Erneut betaetigen: Dauer beginnt von vorn */
    neuer_stand(&s);
    s.figur = (Punkt){4, 7};
    zeit_fortschreiben(&s, 0);
    handeln(&s);
    zeit_fortschreiben(&s, 3000);
    handeln(&s);
    PRUEFE(s.licht_aus_seit_ms == 3000 && s.zuege == 2);
    PRUEFE(zeit_fortschreiben(&s, 7000) == 0 && !s.licht_an);
    PRUEFE(zeit_fortschreiben(&s, 8000) == 1 && s.licht_an);
    /* Das Zuglimit gilt auch im Dunkeln */
    neuer_stand(&s);
    s.figur = (Punkt){4, 7};
    handeln(&s);
    s.zuege = 59;
    PRUEFE(figur_bewegen(&s, -1, 0) && s.zustand == ZEIT_ABGELAUFEN);
    /* Nach dem Spielende bleibt das Licht, wie es ist */
    PRUEFE(zeit_fortschreiben(&s, 100000) == 0 && !s.licht_an);
}

static void teste_speichern(void) {
    printf("Speichern und Laden\n");
    Spielstand s;
    neuer_stand(&s);
    s.figur = (Punkt){4, 7};
    PRUEFE(spielstand_speichern(&s, "test_spielstand.txt") == 0);
    handeln(&s);
    PRUEFE(spielstand_speichern(&s, "test_spielstand.txt") == -3);
    PRUEFE(spielstand_laden(&s, "test_spielstand.txt") == 0 && s.licht_an && s.zuege == 0);
    remove("test_spielstand.txt");
}

static void teste_geruest(void) {
    printf("tresorraub.c\n");
    char zeile[128];
    remove(REGELDATEI);
    PRUEFE(spiel_starten() == 0 && spiel_aktualisieren(1000) == 0);
    PRUEFE(taste_gedrueckt(SDLK_RIGHT) == 0 && taste_gedrueckt(SDLK_SPACE) == 0);
    PRUEFE(spiel_status(zeile, sizeof(zeile)) == 0 && strstr(zeile, "Licht aus") != NULL);
    remove(SPIELSTANDDATEI);
    PRUEFE(taste_gedrueckt(SDLK_F5) == 0);
    FILE *f = fopen(SPIELSTANDDATEI, "r");
    PRUEFE(f == NULL);                              /* im Dunkeln nicht gespeichert */
    if (f != NULL) {
        fclose(f);
    }
    SDL_Surface *flaeche = SDL_CreateRGBSurfaceWithFormat(0, 640, 480, 32, SDL_PIXELFORMAT_RGBA32);
    SDL_Renderer *renderer = flaeche != NULL ? SDL_CreateSoftwareRenderer(flaeche) : NULL;
    PRUEFE(renderer != NULL);
    if (renderer != NULL) {
        PRUEFE(zeichne(renderer) == 0);
        SDL_DestroyRenderer(renderer);
    }
    SDL_FreeSurface(flaeche);
    PRUEFE(spiel_aktualisieren(5999) == 0 && spiel_aktualisieren(6000) == 1);
    PRUEFE(spiel_status(zeile, sizeof(zeile)) == 0 && strstr(zeile, "Licht") == NULL);
    PRUEFE(taste_gedrueckt(SDLK_SPACE) == 0 && taste_gedrueckt(SDLK_n) == 0);
    PRUEFE(spiel_status(zeile, sizeof(zeile)) == 0 && strcmp(zeile, "Position (3, 7) | Zuege 0/60") == 0);
    schreibe(REGELDATEI, "schluessel_x = 0\nschluessel_y = 0\n");
    PRUEFE(spiel_starten() == -1);                  /* Schluessel in der Wand */
    remove(REGELDATEI);
    aufraeumen();
}

int main(int argc, char *argv[]) {
    (void)argc;
    (void)argv;
    bool regeln_gesichert = rename(REGELDATEI, "regeln.txt.testsicherung") == 0;
    bool stand_gesichert = rename(SPIELSTANDDATEI, "spielstand.txt.testsicherung") == 0;
    teste_regeln();
    teste_aufstellung();
    teste_schalter();
    teste_dunkel();
    teste_speichern();
    teste_geruest();
    remove(SPIELSTANDDATEI);
    if (regeln_gesichert) {
        rename("regeln.txt.testsicherung", REGELDATEI);
    }
    if (stand_gesichert) {
        rename("spielstand.txt.testsicherung", SPIELSTANDDATEI);
    }
    printf("\n%d Pruefungen, %d fehlgeschlagen.\n", geprueft, fehler);
    return fehler == 0 ? 0 : 1;
}
