/* Vorbereiteter SDL2-Fensterrahmen. Eigener Zeichencode: zeichnung.c.
   Pointer, Ereignisse und Ressourcenverwaltung werden spaeter vertieft. */
#include <SDL.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include "zeichnung.h"
#include "piktogramme/piktogramme.h"

#if SDL_MAJOR_VERSION != 2
#error Dieses Kursmaterial benoetigt die SDL2-API.
#endif

static void version_ausgeben(void) {
    SDL_version header_version;
    SDL_version laufzeit_version;
    SDL_VERSION(&header_version);
    SDL_GetVersion(&laufzeit_version);
    printf("SDL-Header: %u.%u.%u\n", (unsigned)header_version.major,
           (unsigned)header_version.minor, (unsigned)header_version.patch);
    printf("SDL-Laufzeit: %u.%u.%u\n", (unsigned)laufzeit_version.major,
           (unsigned)laufzeit_version.minor, (unsigned)laufzeit_version.patch);
}

int main(int argc, char *argv[]) {
    bool galerie = false;
    bool diagnose = false;
    bool testlauf = false;
    for (int argument = 1; argument < argc; argument++) {
        if (strcmp(argv[argument], "--version") == 0) {
            version_ausgeben();
            return 0;
        } else if (strcmp(argv[argument], "--galerie") == 0) {
            galerie = true;
        } else if (strcmp(argv[argument], "--diagnose") == 0) {
            diagnose = true;
        } else if (strcmp(argv[argument], "--testlauf") == 0) {
            testlauf = true;
        } else if (strcmp(argv[argument], "--hilfe") == 0) {
            puts("./programm [--version] [--galerie] [--diagnose] [--testlauf]");
            return 0;
        } else {
            fprintf(stderr, "Unbekannte Option: %s\n", argv[argument]);
            return 2;
        }
    }

    version_ausgeben();
    SDL_Window *fenster = NULL;
    SDL_Renderer *renderer = NULL;
    int status = 1;
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        goto fehler;
    }
    Uint32 fenster_flags = testlauf ? SDL_WINDOW_HIDDEN : SDL_WINDOW_RESIZABLE;
    fenster = SDL_CreateWindow("Tresorraub: Mein erstes Piktogramm",
                              SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                              640, 480, fenster_flags);
    if (fenster == NULL) {
        goto fehler;
    }
    /* Softwaredarstellung genuegt fuer die kleinen statischen Motive. */
    renderer = SDL_CreateRenderer(fenster, -1, SDL_RENDERER_SOFTWARE);
    if (renderer == NULL) {
        goto fehler;
    }
    if (SDL_RenderSetLogicalSize(renderer, 640, 480) < 0) {
        goto fehler;
    }

    bool laeuft = true;
    while (laeuft) {
        SDL_Event ereignis;
        while (SDL_PollEvent(&ereignis)) {
            if (ereignis.type == SDL_QUIT
                || (ereignis.type == SDL_KEYDOWN
                    && ereignis.key.keysym.sym == SDLK_ESCAPE)) {
                laeuft = false;
            }
        }
        if (!laeuft) {
            break;
        }
        if (SDL_SetRenderDrawColor(renderer, 245, 246, 249, 255) < 0
            || SDL_RenderClear(renderer) < 0
            || SDL_SetRenderDrawColor(renderer, 23, 37, 59, 255) < 0) {
            goto fehler;
        }

        int ergebnis;
        if (diagnose) {
            /* Absichtlich simulierter Fehler, keine defekte Grafikoperation. */
            ergebnis = SDL_SetError("Absichtlich ausgeloester Diagnosefehler.");
        } else if (galerie) {
            ergebnis = zeichne_galerie(renderer);
        } else {
            ergebnis = zeichne(renderer);
        }
        if (ergebnis < 0) {
            goto fehler;
        }
        SDL_RenderPresent(renderer); /* SDL2: void, kein Fehlercode. */
        if (testlauf) {
            break;
        }
        SDL_Delay(16); /* Keine leere Dauerschleife mit voller CPU-Auslastung. */
    }
    status = 0;
    goto aufraeumen;

fehler:
    /* Meldung vor dem Aufraeumen auslesen, nicht erst danach. */
    fprintf(stderr, "SDL-Fehler: %s\n", SDL_GetError());
aufraeumen:
    if (renderer != NULL) {
        SDL_DestroyRenderer(renderer);
    }
    if (fenster != NULL) {
        SDL_DestroyWindow(fenster);
    }
    SDL_Quit();
    return status;
}
