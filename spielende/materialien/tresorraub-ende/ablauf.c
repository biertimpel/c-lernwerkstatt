/* Vergleichslauf fuer den Tresorraub.
   Startet das Spiel, drueckt eine feste Folge von Tasten und gibt nach jeder
   Taste die Statuszeile und eine Pruefsumme des gezeichneten Bildes aus.
   Ein Umbau, der das Verhalten nicht aendert, liefert dieselbe Ausgabe:
       ./ablauf > vorher.txt     (vor dem Umbau)
       ./ablauf > nachher.txt    (nach dem Umbau)
       diff vorher.txt nachher.txt
   Die Tastenfolge enthaelt F5 und F9; der Vergleichslauf schreibt deshalb
   spielstand.txt im aktuellen Ordner.
   Diese Datei muessen Sie nicht veraendern. */
#include "tresorraub.h"
#include <stdint.h>
#include <stdio.h>

#define BILD_BREITE 640
#define BILD_HOEHE 480

/* Zeichnet das Spiel in ein unsichtbares Bild und bildet daraus eine Zahl
   (FNV-1a). Gleiche Bilder liefern gleiche Zahlen. */
static int bild_pruefsumme(SDL_Renderer *renderer, SDL_Surface *bild, uint32_t *summe) {
    SDL_SetRenderDrawColor(renderer, 20, 20, 28, 255);
    SDL_RenderClear(renderer);
    if (zeichne(renderer) < 0) {
        return -1;
    }
    SDL_RenderPresent(renderer);
    uint32_t h = 2166136261u;
    const unsigned char *zeilen = bild->pixels;
    for (int y = 0; y < bild->h; y++) {
        const unsigned char *zeile = zeilen + (size_t)y * (size_t)bild->pitch;
        for (int i = 0; i < bild->w * 4; i++) {
            h = (h ^ zeile[i]) * 16777619u;
        }
    }
    *summe = h;
    return 0;
}

static int schritt(const char *name, SDL_Keycode taste, SDL_Renderer *renderer,
                   SDL_Surface *bild) {
    printf("Taste %s\n", name);
    fflush(stdout);
    if (taste != SDLK_UNKNOWN && taste_gedrueckt(taste) < 0) {
        return -1;
    }
    char status[128];
    uint32_t summe;
    if (spiel_status(status, sizeof(status)) < 0 || bild_pruefsumme(renderer, bild, &summe) < 0) {
        return -1;
    }
    printf("  %s | Bild %08lx\n", status, (unsigned long)summe);
    fflush(stdout);
    return 0;
}

int main(int argc, char *argv[]) {
    (void)argc;
    (void)argv;
    static const struct { const char *name; SDL_Keycode taste; } FOLGE[] = {
        {"Start", SDLK_UNKNOWN}, {"Leertaste", SDLK_SPACE}, {"runter", SDLK_DOWN}, {"rechts", SDLK_RIGHT}, {"rechts", SDLK_RIGHT}, {"hoch", SDLK_UP}, {"hoch", SDLK_UP}, {"hoch", SDLK_UP}, {"hoch", SDLK_UP},
        {"rechts", SDLK_RIGHT}, {"Leertaste", SDLK_SPACE}, {"runter", SDLK_DOWN}, {"runter", SDLK_DOWN}, {"links", SDLK_LEFT}, {"links", SDLK_LEFT}, {"Leertaste", SDLK_SPACE},
        {"F5", SDLK_F5}, {"rechts", SDLK_RIGHT}, {"hoch", SDLK_UP}, {"F9", SDLK_F9},
        {"Leertaste", SDLK_SPACE}, {"L", SDLK_l}, {"hoch", SDLK_UP}, {"rechts", SDLK_RIGHT}, {"hoch", SDLK_UP}, {"Leertaste", SDLK_SPACE}, {"rechts", SDLK_RIGHT}, {"hoch", SDLK_UP},
        {"Leertaste", SDLK_SPACE}, {"rechts", SDLK_RIGHT}, {"Leertaste", SDLK_SPACE}, {"rechts", SDLK_RIGHT}, {"rechts", SDLK_RIGHT}, {"rechts", SDLK_RIGHT},
        {"rechts", SDLK_RIGHT}, {"rechts", SDLK_RIGHT}, {"runter", SDLK_DOWN}, {"runter", SDLK_DOWN}, {"runter", SDLK_DOWN}, {"runter", SDLK_DOWN}, {"Leertaste", SDLK_SPACE},
        {"hoch", SDLK_UP}, {"hoch", SDLK_UP}, {"hoch", SDLK_UP}, {"hoch", SDLK_UP}, {"rechts", SDLK_RIGHT}, {"rechts", SDLK_RIGHT}, {"hoch", SDLK_UP}, {"hoch", SDLK_UP}, {"Leertaste", SDLK_SPACE},
        {"F5", SDLK_F5}, {"Leertaste", SDLK_SPACE}, {"L", SDLK_l}, {"L", SDLK_l}, {"runter", SDLK_DOWN}, {"Leertaste", SDLK_SPACE}, {"links", SDLK_LEFT}, {"Leertaste", SDLK_SPACE}, {"links", SDLK_LEFT},
        {"F9", SDLK_F9}, {"x", SDLK_x}
    };
    SDL_Surface *bild = SDL_CreateRGBSurfaceWithFormat(0, BILD_BREITE, BILD_HOEHE, 32,
                                                       SDL_PIXELFORMAT_ARGB8888);
    SDL_Renderer *renderer = bild == NULL ? NULL : SDL_CreateSoftwareRenderer(bild);
    int status = 1;
    if (renderer == NULL) {
        fprintf(stderr, "Bild konnte nicht angelegt werden: %s\n", SDL_GetError());
    } else if (spiel_starten() < 0) {
        fprintf(stderr, "spiel_starten hat einen Fehler gemeldet.\n");
    } else {
        status = 0;
        for (size_t i = 0; i < sizeof(FOLGE) / sizeof(FOLGE[0]) && status == 0; i++) {
            if (schritt(FOLGE[i].name, FOLGE[i].taste, renderer, bild) < 0) {
                fprintf(stderr, "Fehler bei Schritt %zu.\n", i + 1);
                status = 1;
            }
        }
    }
    aufraeumen();
    if (renderer != NULL) {
        SDL_DestroyRenderer(renderer);
    }
    SDL_FreeSurface(bild);
    return status;
}
