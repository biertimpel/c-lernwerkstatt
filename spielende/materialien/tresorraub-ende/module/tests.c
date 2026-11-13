/* Test-Suite fuer den Tresorraub nach dem Aufteilen in Module.
   Sie prueft jedes Modul einzeln ueber seinen Header (Unit Tests) und am
   Ende die Anschluesse an das Geruest. Diese Datei muessen Sie nicht
   veraendern.

   Bauen und starten:  make test
   Jeder fehlgeschlagene Test nennt seine Zeile hier in tests.c. Lesen Sie
   dort nach, was erwartet wird.

   Die Tests legen im aktuellen Ordner Testdateien an und loeschen sie
   wieder. Eine vorhandene regeln.txt und spielstand.txt werden vorher
   gesichert und danach wiederhergestellt. */
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
            printf("  FEHLGESCHLAGEN (tests.c, Zeile %d): %s\n", __LINE__, #bedingung); \
            fehler++; \
        } \
    } while (0)

static void gruppe(const char *name) {
    printf("%s\n", name);
}

static void schreibe(const char *name, const char *inhalt) {
    FILE *f = fopen(name, "w");
    if (f == NULL) {
        printf("Testdatei %s kann nicht angelegt werden.\n", name);
        exit(1);
    }
    fputs(inhalt, f);
    fclose(f);
}

/* Erstes Objekt mit diesem Artnamen, sonst NULL. */
static Objekt *objekt_namens(Spielstand *s, const char *name) {
    for (size_t i = 0; i < s->anzahl_objekte; i++) {
        if (strcmp(s->objekte[i].art->name, name) == 0) {
            return &s->objekte[i];
        }
    }
    return NULL;
}

static void neuer_stand(Spielstand *s) {
    Regeln r = regeln_standard();
    startzustand(s, &r);
}

static void teste_spielfeld(void) {
    gruppe("spielfeld: Grundriss, Abstaende, Ueberwachung");
    PRUEFE(punkt_gleich((Punkt){3, 7}, (Punkt){3, 7}));
    PRUEFE(!punkt_gleich((Punkt){3, 7}, (Punkt){7, 3}));
    PRUEFE(rasterabstand((Punkt){1, 5}, (Punkt){3, 6}) == 3);
    PRUEFE(innerhalb((Punkt){0, 0}) && innerhalb((Punkt){14, 8}));
    PRUEFE(!innerhalb((Punkt){15, 3}) && !innerhalb((Punkt){-1, 3}));
    PRUEFE(ist_wand((Punkt){0, 0}) && !ist_wand((Punkt){3, 7}));
    PRUEFE(begehbar((Punkt){3, 7}) && !begehbar((Punkt){3, 4}) && !begehbar((Punkt){15, 3}));
    Waechter w[2] = {{{1, 5}, 1}, {{9, 5}, 2}};
    PRUEFE(ueberwacht((Punkt){1, 5}, w, 2));   /* das Feld des Waechters selbst */
    PRUEFE(ueberwacht((Punkt){2, 5}, w, 2) && ueberwacht((Punkt){9, 3}, w, 2));
    PRUEFE(!ueberwacht((Punkt){3, 5}, w, 2) && !ueberwacht((Punkt){8, 3}, w, 2));
}

static void teste_regeln(void) {
    gruppe("regeln: Standardwerte und regeln.txt");
    Regeln r = regeln_standard();
    PRUEFE(r.zuglimit == 60 && r.reichweite[0] == 1 && r.reichweite[1] == 2
           && r.inventar_groesse == 3);
    remove("test_regeln.txt");
    PRUEFE(regeln_laden("test_regeln.txt", &r) == 1 && r.zuglimit == 60);
    schreibe("test_regeln.txt", "# Kommentar\nzuglimit = 40\n\ninventar_groesse = 1\n");
    PRUEFE(regeln_laden("test_regeln.txt", &r) == 0 && r.zuglimit == 40 && r.inventar_groesse == 1);
    r = regeln_standard();
    schreibe("test_regeln.txt", "zuglimit = 40\nzuglimt = 3\n");
    PRUEFE(regeln_laden("test_regeln.txt", &r) == -1 && r.zuglimit == 60);
    remove("test_regeln.txt");
}

static void teste_inventar(void) {
    gruppe("inventar: Liste von Pointern auf Gegenstaende");
    Spielstand s;
    neuer_stand(&s);
    Objekt *schluessel = objekt_namens(&s, "Schluessel");
    Objekt *muenze = objekt_namens(&s, "Muenze");
    Objekt *beute = objekt_namens(&s, "Beute");
    PRUEFE(schluessel != NULL && muenze != NULL && beute != NULL);
    if (schluessel == NULL || muenze == NULL || beute == NULL) {
        return;
    }
    Inventar inv = {NULL};
    PRUEFE(inventar_anzahl(&inv) == 0);
    PRUEFE(inventar_aufnehmen(&inv, schluessel, 2) == AUFGENOMMEN);
    PRUEFE(inventar_aufnehmen(&inv, muenze, 2) == AUFGENOMMEN);
    PRUEFE(inventar_aufnehmen(&inv, beute, 2) == INVENTAR_VOLL && inventar_anzahl(&inv) == 2);
    PRUEFE(inv.erster != NULL && inv.erster->gegenstand == muenze);   /* vorne eingefuegt */
    PRUEFE(inventar_finden(&inv, schluessel->art) == schluessel);
    PRUEFE(inventar_finden(&inv, beute->art) == NULL);
    PRUEFE(inventar_entfernen(&inv, schluessel) && inventar_anzahl(&inv) == 1);
    PRUEFE(!inventar_entfernen(&inv, schluessel));
    inventar_leeren(&inv);
    PRUEFE(inv.erster == NULL);
    inventar_leeren(&inv);   /* zweimal sicher */
}

static void teste_objekte(void) {
    gruppe("objekte: Startzustand und Hilfsfunktionen");
    Spielstand s;
    neuer_stand(&s);
    PRUEFE(s.zuege == 0 && s.zustand == LAEUFT && s.inventar.erster == NULL);
    PRUEFE(punkt_gleich(s.figur, (Punkt){3, 7}));
    PRUEFE(s.anzahl_objekte == 8);
    const Objekt *tuer = objekt_auf(&s, (Punkt){6, 3});
    PRUEFE(tuer != NULL && strcmp(tuer->art->name, "Tuer") == 0 && !tuer->offen);
    PRUEFE(objekt_auf(&s, (Punkt){3, 7}) == NULL);
    const Objekt *im_tresor = gegenstand_auf(&s, (Punkt){13, 1});
    PRUEFE(im_tresor != NULL && strcmp(im_tresor->art->name, "Beute") == 0);
    PRUEFE(waechter_auf(&s, (Punkt){9, 5}) && !waechter_auf(&s, (Punkt){9, 4}));
    PRUEFE(objekt_zeichen(tuer) == 'D');
    Regeln weit = regeln_standard();
    weit.reichweite[1] = 3;
    startzustand(&s, &weit);
    PRUEFE(s.waechter[1].reichweite == 3);
}

static void teste_spiellogik(void) {
    gruppe("spiellogik: Bewegen, Leertaste, Ablegen, Ausgabe");
    Spielstand s;
    neuer_stand(&s);
    PRUEFE(!figur_bewegen(&s, 0, 1) && s.zuege == 0);            /* Wand */
    PRUEFE(!figur_bewegen(&s, 1, 1) && s.zuege == 0);            /* diagonal */
    PRUEFE(figur_bewegen(&s, 0, -1) && figur_bewegen(&s, 0, -1));
    PRUEFE(punkt_gleich(s.figur, (Punkt){3, 5}) && s.zuege == 2);
    PRUEFE(handeln(&s) == 0 && s.zuege == 3 && inventar_anzahl(&s.inventar) == 1);
    PRUEFE(feldzeichen(&s, (Punkt){3, 5}) == '@');
    char zeile[128];
    PRUEFE(spielstand_lesen(&s, zeile, sizeof(zeile)) == 0
           && strcmp(zeile, "Position (3, 5) | Zuege 3/60") == 0);
    PRUEFE(ablegen(&s) == 1 && s.zuege == 4 && s.inventar.erster == NULL);
    s.figur = (Punkt){5, 3};
    PRUEFE(!figur_bewegen(&s, 1, 0));                           /* Tuer zu */
    PRUEFE(spielstand_lesen(&s, zeile, 10) == -1);              /* zu klein */
}

static void teste_spielende(void) {
    gruppe("spiellogik: Spielende");
    Spielstand s;
    char zeile[128];
    /* Entdeckt: Schritt in den Bereich des Waechters auf (1, 5) */
    neuer_stand(&s);
    s.figur = (Punkt){3, 5};
    PRUEFE(figur_bewegen(&s, -1, 0) && s.zustand == ENTDECKT);
    PRUEFE(spielstand_lesen(&s, zeile, sizeof(zeile)) == 0
           && strstr(zeile, "| WASTED: entdeckt") != NULL);
    PRUEFE(!figur_bewegen(&s, 1, 0) && punkt_gleich(s.figur, (Punkt){2, 5}));   /* vorbei */
    /* Zuglimit: der 60. Zug ohne Sieg */
    neuer_stand(&s);
    s.zuege = 59;
    PRUEFE(figur_bewegen(&s, 0, -1) && s.zuege == 60 && s.zustand == ZEIT_ABGELAUFEN);
    /* Auch ein gezaehlter Zug ohne Bewegung (Aufnehmen) beendet das Spiel */
    neuer_stand(&s);
    s.figur = (Punkt){3, 5};
    s.zuege = 59;
    PRUEFE(handeln(&s) == 0 && s.zustand == ZEIT_ABGELAUFEN);
    inventar_leeren(&s.inventar);
    /* Ausgang ohne Beute: kein Sieg */
    neuer_stand(&s);
    s.figur = (Punkt){2, 1};
    PRUEFE(figur_bewegen(&s, -1, 0) && s.zustand == LAEUFT);
    /* Ausgang mit Beute, auch im 60. Zug: gewonnen */
    neuer_stand(&s);
    Objekt *beute = objekt_namens(&s, "Beute");
    Objekt *tresor = objekt_namens(&s, "Tresor");
    if (beute == NULL || tresor == NULL) {
        PRUEFE(beute != NULL && tresor != NULL);
        return;
    }
    tresor->inhalt = NULL;
    tresor->offen = true;
    PRUEFE(inventar_aufnehmen(&s.inventar, beute, 3) == AUFGENOMMEN);
    s.figur = (Punkt){2, 1};
    s.zuege = 59;
    PRUEFE(figur_bewegen(&s, -1, 0) && s.zustand == GEWONNEN && s.zuege == 60);
    PRUEFE(spielstand_lesen(&s, zeile, sizeof(zeile)) == 0
           && strstr(zeile, "| Entkommen!") != NULL);
    PRUEFE(handeln(&s) == 0 && ablegen(&s) == 0 && inventar_anzahl(&s.inventar) == 1);
    PRUEFE(spielstand_speichern(&s, "test_spielstand.txt") == -2);   /* vorbei */
    inventar_leeren(&s.inventar);
}

static void teste_speicherstand(void) {
    gruppe("speicherstand: Format Version 2");
    Spielstand s;
    neuer_stand(&s);
    figur_bewegen(&s, 0, -1);
    figur_bewegen(&s, 0, -1);
    handeln(&s);                                 /* Schluessel aufnehmen */
    PRUEFE(spielstand_speichern(&s, "test_spielstand.txt") == 0);
    Spielstand t;
    neuer_stand(&t);
    PRUEFE(spielstand_laden(&t, "test_spielstand.txt") == 0);
    PRUEFE(punkt_gleich(t.figur, (Punkt){3, 5}) && t.zuege == 3 && t.zustand == LAEUFT);
    PRUEFE(inventar_anzahl(&t.inventar) == 1
           && strcmp(t.inventar.erster->gegenstand->art->name, "Schluessel") == 0);
    PRUEFE(t.inventar.erster->gegenstand == objekt_namens(&t, "Schluessel"));   /* eigene Objekte */
    /* Spielstaende, in denen das Spiel schon zu Ende waere, werden abgelehnt */
    inventar_leeren(&s.inventar);
    neuer_stand(&s);
    s.figur = (Punkt){2, 5};                     /* ueberwacht */
    PRUEFE(spielstand_speichern(&s, "test_spielstand.txt") == 0);
    PRUEFE(spielstand_laden(&t, "test_spielstand.txt") == -2 && t.zuege == 3);
    neuer_stand(&s);
    s.zuege = 60;
    PRUEFE(spielstand_speichern(&s, "test_spielstand.txt") == 0);
    PRUEFE(spielstand_laden(&t, "test_spielstand.txt") == -2);
    schreibe("test_spielstand.txt", "TRESORRAUB 1\nFIGUR 3 7\nZUEGE 0\nTUER 0\n"
             "SCHLUESSEL 3 5 1\nMUENZE 11 7 1\nBEUTE 13 1 0 1\nINVENTAR 0\nENDE\n");
    PRUEFE(spielstand_laden(&t, "test_spielstand.txt") == -2);     /* Version 1 */
    remove("test_spielstand.txt");
    PRUEFE(spielstand_laden(&t, "test_spielstand.txt") == -1);     /* fehlt */
    PRUEFE(punkt_gleich(t.figur, (Punkt){3, 5}) && inventar_anzahl(&t.inventar) == 1);
    inventar_leeren(&t.inventar);
}

static void teste_geruest(void) {
    gruppe("tresorraub.c: Anschluesse an das Geruest");
    char zeile[128];
    remove(REGELDATEI);
    PRUEFE(spiel_starten() == 0);
    PRUEFE(spiel_status(zeile, sizeof(zeile)) == 0
           && strcmp(zeile, "Position (3, 7) | Zuege 0/60") == 0);
    PRUEFE(taste_gedrueckt(SDLK_UP) == 0 && taste_gedrueckt(SDLK_UP) == 0
           && taste_gedrueckt(SDLK_SPACE) == 0);
    PRUEFE(taste_gedrueckt(SDLK_LEFT) == 0);     /* (2, 5): entdeckt */
    PRUEFE(spiel_status(zeile, sizeof(zeile)) == 0 && strstr(zeile, "WASTED") != NULL);
    PRUEFE(taste_gedrueckt(SDLK_RIGHT) == 0);    /* vorbei: nichts geschieht */
    PRUEFE(spiel_status(zeile, sizeof(zeile)) == 0 && strstr(zeile, "Position (2, 5)") != NULL);
    PRUEFE(taste_gedrueckt(SDLK_n) == 0);        /* neues Spiel */
    PRUEFE(spiel_status(zeile, sizeof(zeile)) == 0
           && strcmp(zeile, "Position (3, 7) | Zuege 0/60") == 0);
    SDL_Surface *flaeche = SDL_CreateRGBSurfaceWithFormat(0, 640, 480, 32, SDL_PIXELFORMAT_RGBA32);
    SDL_Renderer *renderer = flaeche != NULL ? SDL_CreateSoftwareRenderer(flaeche) : NULL;
    PRUEFE(renderer != NULL);
    if (renderer != NULL) {
        PRUEFE(zeichne(renderer) == 0);
        SDL_DestroyRenderer(renderer);
    }
    SDL_FreeSurface(flaeche);
    PRUEFE(text_ausgeben() == 0);
    aufraeumen();
    aufraeumen();
}

int main(int argc, char *argv[]) {
    (void)argc;
    (void)argv;
    bool regeln_gesichert = rename(REGELDATEI, "regeln.txt.testsicherung") == 0;
    bool stand_gesichert = rename(SPIELSTANDDATEI, "spielstand.txt.testsicherung") == 0;
    teste_spielfeld();
    teste_regeln();
    teste_inventar();
    teste_objekte();
    teste_spiellogik();
    teste_spielende();
    teste_speicherstand();
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
