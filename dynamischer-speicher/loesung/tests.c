/* Tests zur Musterloesung. Prueft die erwarteten Ergebnisse aus den
   Selbstkontrollen und den Fehlerpfad bei fehlgeschlagenem malloc. */
#include <SDL.h>   /* vor dem Ersetzen von malloc einbinden */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* malloc ersetzen, um Speicherfehler kontrolliert auszuloesen:
   Ist fehlschlag_ab gleich 0, schlaegt der naechste Aufruf fehl. */
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

/* Hilfe: Namen der Eintraege in Reihenfolge, durch Leerzeichen getrennt. */
static void als_text(const Inventar *inv, char ziel[], size_t groesse) {
    ziel[0] = '\0';
    for (const Eintrag *e = inv->erster; e != NULL; e = e->naechster) {
        strncat(ziel, e->name, groesse - strlen(ziel) - 1);
        if (e->naechster != NULL) {
            strncat(ziel, " ", groesse - strlen(ziel) - 1);
        }
    }
}

int main(void) {
    char text[64];

    /* Seite 2 */
    Eintrag *e = eintrag_anlegen("Beute");
    PRUEFE(e != NULL && strcmp(e->name, "Beute") == 0 && e->naechster == NULL);
    free(e);
    PRUEFE(eintrag_anlegen("Name mit mehr als fuenfzehn Zeichen") == NULL);
    fehlschlag_ab = 0;
    PRUEFE(eintrag_anlegen("Beute") == NULL);

    /* Seite 3 */
    Inventar inv = {NULL};
    PRUEFE(inventar_anzahl(&inv) == 0 && !inventar_enthaelt(&inv, "Beute"));
    PRUEFE(inventar_aufnehmen(&inv, "Schluessel") == AUFGENOMMEN);
    PRUEFE(inventar_aufnehmen(&inv, "Beute") == AUFGENOMMEN);
    PRUEFE(inventar_aufnehmen(&inv, "Schluessel") == SCHON_VORHANDEN);
    PRUEFE(inventar_anzahl(&inv) == 2);
    PRUEFE(inventar_aufnehmen(&inv, "Muenze") == AUFGENOMMEN);
    als_text(&inv, text, sizeof(text));
    PRUEFE(strcmp(text, "Muenze Beute Schluessel") == 0);   /* neu steht vorne */
    PRUEFE(inventar_aufnehmen(&inv, "Kompass") == INVENTAR_VOLL);
    PRUEFE(inventar_anzahl(&inv) == 3);

    /* Speicherfehler beim Aufnehmen: Inventar bleibt unveraendert */
    inventar_entfernen(&inv, "Beute");
    fehlschlag_ab = 0;
    PRUEFE(inventar_aufnehmen(&inv, "Beute") == FEHLER_SPEICHER);
    als_text(&inv, text, sizeof(text));
    PRUEFE(strcmp(text, "Muenze Schluessel") == 0);
    PRUEFE(inventar_aufnehmen(&inv, "Beute") == AUFGENOMMEN);

    /* Seite 4: Liste ist Beute, Muenze, Schluessel */
    PRUEFE(inventar_entfernen(&inv, "Muenze"));          /* Mitte */
    als_text(&inv, text, sizeof(text));
    PRUEFE(strcmp(text, "Beute Schluessel") == 0);
    PRUEFE(inventar_entfernen(&inv, "Schluessel"));      /* Ende */
    als_text(&inv, text, sizeof(text));
    PRUEFE(strcmp(text, "Beute") == 0);
    PRUEFE(inventar_entfernen(&inv, "Beute"));           /* Kopf, einziger */
    PRUEFE(inv.erster == NULL);
    PRUEFE(!inventar_entfernen(&inv, "Beute"));          /* leer */
    inventar_aufnehmen(&inv, "Schluessel");
    inventar_aufnehmen(&inv, "Muenze");
    PRUEFE(!inventar_entfernen(&inv, "Beute"));          /* nicht vorhanden */
    PRUEFE(inventar_entfernen(&inv, "Muenze"));          /* Kopf von zweien */
    als_text(&inv, text, sizeof(text));
    PRUEFE(strcmp(text, "Schluessel") == 0);
    inventar_leeren(&inv);
    PRUEFE(inv.erster == NULL);
    inventar_leeren(&inv);                               /* zweimal sicher */
    PRUEFE(inv.erster == NULL);

    /* Seite 5: Startzustand und Statuszeile */
    char zeile[96];
    PRUEFE(spiel_status(zeile, sizeof(zeile)) == 0);   /* vor spiel_starten */
    PRUEFE(spiel_starten() == 0);
    PRUEFE(spielstand.inventar.erster == NULL);
    PRUEFE(spiel_status(zeile, sizeof(zeile)) == 0
           && strcmp(zeile, "Position (3, 7) | Zuege 0/60") == 0);

    /* Seite 5: Leertaste ohne Gegenstand und ohne Tuer */
    PRUEFE(taste_gedrueckt(SDLK_SPACE) == 0 && spielstand.zuege == 0);
    /* Tuer ohne Schluessel */
    spielstand.figur = (Punkt){5, 3};
    PRUEFE(taste_gedrueckt(SDLK_SPACE) == 0 && !spielstand.tuer_offen && spielstand.zuege == 0);
    /* Zum Schluessel laufen: (3,7) -> (3,5) */
    spielstand.figur = (Punkt){3, 7};
    PRUEFE(taste_gedrueckt(SDLK_UP) == 0 && taste_gedrueckt(SDLK_UP) == 0);
    PRUEFE(spiel_status(zeile, sizeof(zeile)) == 0
           && strcmp(zeile, "Position (3, 5) | Zuege 2/60 | Hier liegt: Schluessel") == 0);
    PRUEFE(taste_gedrueckt(SDLK_SPACE) == 0);
    PRUEFE(spielstand.zuege == 3 && !spielstand.schluessel_liegt);
    PRUEFE(inventar_enthaelt(&spielstand.inventar, "Schluessel"));
    PRUEFE(spiel_status(zeile, sizeof(zeile)) == 0
           && strcmp(zeile, "Position (3, 5) | Zuege 3/60") == 0);
    PRUEFE(feldzeichen(&spielstand, (Punkt){3, 5}) == '@');
    PRUEFE(taste_gedrueckt(SDLK_SPACE) == 0 && spielstand.zuege == 3);   /* nichts mehr da */
    /* Zur Tuer: rechts, rechts, hoch, hoch -> (5,3); ueber (3,4) ist eine Wand */
    PRUEFE(taste_gedrueckt(SDLK_RIGHT) == 0 && taste_gedrueckt(SDLK_RIGHT) == 0);
    PRUEFE(taste_gedrueckt(SDLK_UP) == 0 && taste_gedrueckt(SDLK_UP) == 0);
    PRUEFE(punkt_gleich(spielstand.figur, (Punkt){5, 3}) && spielstand.zuege == 7);
    PRUEFE(taste_gedrueckt(SDLK_RIGHT) == 0 && spielstand.zuege == 7);   /* Tuer zu */
    PRUEFE(taste_gedrueckt(SDLK_SPACE) == 0);
    PRUEFE(spielstand.tuer_offen && spielstand.zuege == 8);
    PRUEFE(spielstand.inventar.erster == NULL);                          /* verbraucht */
    PRUEFE(taste_gedrueckt(SDLK_RIGHT) == 0 && spielstand.zuege == 9);
    PRUEFE(punkt_gleich(spielstand.figur, (Punkt){6, 3}));
    PRUEFE(taste_gedrueckt(SDLK_SPACE) == 0 && spielstand.zuege == 9);   /* Tuer schon offen */
    /* Muenze und Beute */
    spielstand.figur = (Punkt){11, 7};
    PRUEFE(taste_gedrueckt(SDLK_SPACE) == 0 && !spielstand.muenze_liegt);
    spielstand.figur = (Punkt){13, 1};
    PRUEFE(spiel_status(zeile, sizeof(zeile)) == 0
           && strcmp(zeile, "Position (13, 1) | Zuege 10/60 | Hier liegt: Beute") == 0);
    PRUEFE(taste_gedrueckt(SDLK_SPACE) == 0 && !spielstand.beute_im_tresor);
    PRUEFE(feldzeichen(&spielstand, (Punkt){13, 1}) == '@');
    PRUEFE(spielstand.zuege == 11);
    als_text(&spielstand.inventar, text, sizeof(text));
    PRUEFE(strcmp(text, "Beute Muenze") == 0);

    /* Seite 5: Speicherfehler beim Aufnehmen */
    {
        Spielstand t = startzustand();
        t.figur = (Punkt){3, 5};
        fehlschlag_ab = 0;
        PRUEFE(handeln(&t) < 0);
        PRUEFE(t.inventar.erster == NULL && t.schluessel_liegt && t.zuege == 0);
        PRUEFE(handeln(&t) == 0 && !t.schluessel_liegt);
        inventar_leeren(&t.inventar);
    }

    /* Seite 6: Ablegen bei (10, 3) */
    spielstand.figur = (Punkt){10, 3};
    PRUEFE(taste_gedrueckt(SDLK_l) == 0);
    PRUEFE(spielstand.inventar.erster == NULL && spielstand.zuege == 12);
    PRUEFE(spielstand.beute_liegt && punkt_gleich(spielstand.beute, (Punkt){11, 3}));
    PRUEFE(spielstand.muenze_liegt && punkt_gleich(spielstand.muenze, (Punkt){11, 4}));
    PRUEFE(feldzeichen(&spielstand, (Punkt){11, 3}) == 'b');
    PRUEFE(feldzeichen(&spielstand, (Punkt){13, 1}) == 't');
    PRUEFE(taste_gedrueckt(SDLK_l) == 0 && spielstand.zuege == 12);   /* leer */
    /* Wieder aufnehmen */
    spielstand.figur = (Punkt){11, 3};
    PRUEFE(spiel_status(zeile, sizeof(zeile)) == 0
           && strcmp(zeile, "Position (11, 3) | Zuege 12/60 | Hier liegt: Beute") == 0);
    PRUEFE(taste_gedrueckt(SDLK_SPACE) == 0 && !spielstand.beute_liegt);
    PRUEFE(!spielstand.beute_im_tresor && spielstand.zuege == 13);
    /* Ablegen am Ausgang: (1,1) ist kein freies Feld */
    spielstand.figur = (Punkt){1, 2};
    PRUEFE(taste_gedrueckt(SDLK_l) == 0 && spielstand.beute_liegt);
    PRUEFE(punkt_gleich(spielstand.beute, (Punkt){2, 1}));
    /* Kein Platz: Gegenstand ohne Position (Kompass) bleibt im Inventar */
    PRUEFE(inventar_aufnehmen(&spielstand.inventar, "Kompass") == AUFGENOMMEN);
    PRUEFE(ablegen(&spielstand) == 0 && inventar_anzahl(&spielstand.inventar) == 1);

    /* Andere Tasten, zeichne, aufraeumen */
    PRUEFE(taste_gedrueckt(SDLK_x) == 0);
    SDL_Surface *flaeche = SDL_CreateRGBSurfaceWithFormat(0, 640, 480, 32, SDL_PIXELFORMAT_RGBA32);
    SDL_Renderer *renderer = flaeche != NULL ? SDL_CreateSoftwareRenderer(flaeche) : NULL;
    PRUEFE(renderer != NULL);
    if (renderer != NULL) {
        PRUEFE(zeichne(renderer) == 0);
        SDL_DestroyRenderer(renderer);
    }
    SDL_FreeSurface(flaeche);
    /* Speicherfehler ueber die Leertaste meldet einen Fehler */
    spielstand.figur = (Punkt){2, 1};
    fehlschlag_ab = 0;
    PRUEFE(taste_gedrueckt(SDLK_SPACE) < 0 && spielstand.beute_liegt);
    aufraeumen();
    PRUEFE(spielstand.inventar.erster == NULL);
    aufraeumen();   /* zweimal sicher */

    if (fehler == 0) {
        printf("Alle Tests bestanden.\n");
    }
    return fehler == 0 ? 0 : 1;
}
