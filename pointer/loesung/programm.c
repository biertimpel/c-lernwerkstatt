/* Vorbereitetes SDL2-Programmgeruest fuer das Kapitel Pointer und Datenaustausch.
   Ihr eigener Code entsteht in tresorraub.c. Diese Datei muessen Sie nicht
   veraendern. Aufbau wie im Kapitel "Manpages und Bibliotheksdokumentation". */
#include <SDL.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include "tresorraub.h"
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

/* SDL bleibt hier im Geruest. Der Spielcode erhaelt nur den Tastencode. */
static bool ereignis_bearbeiten(const SDL_Event *ereignis, bool galerie) {
    if (ereignis->type == SDL_QUIT) {
        return false;
    }
    if (ereignis->type == SDL_KEYDOWN) {
        if (ereignis->key.keysym.sym == SDLK_ESCAPE) {
            return false;
        }
        if (!galerie && ereignis->key.repeat == 0) {
            taste_gedrueckt(ereignis->key.keysym.sym);
        }
    }
    return true;
}

int main(int argc, char *argv[]) {
    bool version = false;
    bool hilfe = false;
    bool text = false;
    bool galerie = false;
    bool testlauf = false;
    /* Erst alle Optionen pruefen, dann handeln: So haengt das Ergebnis
       nicht von der Reihenfolge der Optionen ab. */
    for (int argument = 1; argument < argc; argument++) {
        if (strcmp(argv[argument], "--version") == 0) {
            version = true;
        } else if (strcmp(argv[argument], "--text") == 0) {
            text = true;
        } else if (strcmp(argv[argument], "--galerie") == 0) {
            galerie = true;
        } else if (strcmp(argv[argument], "--testlauf") == 0) {
            testlauf = true;
        } else if (strcmp(argv[argument], "--hilfe") == 0) {
            hilfe = true;
        } else {
            fprintf(stderr, "Unbekannte Option: %s\n", argv[argument]);
            return 2;
        }
    }
    if (hilfe) {
        puts("./programm [--text] [--galerie] [--testlauf] [--version] [--hilfe]");
        puts("  --text      text_ausgeben() aufrufen, kein Fenster");
        puts("  --galerie   alle Piktogramme im Fenster zeigen");
        puts("  --testlauf  verstecktes Fenster, genau ein Bild, dann beenden");
        return 0;
    }
    if (version) {
        version_ausgeben();
        return 0;
    }
    if (text) {
        /* Textmodus: kein Fenster, Ihre Funktion wird genau einmal aufgerufen. */
        return text_ausgeben() < 0 ? 1 : 0;
    }

    /* Alle Variablen, die nach der Marke "ende" noch gebraucht werden,
       stehen vor dem ersten goto und sind dort bereits initialisiert. */
    SDL_Window *fenster = NULL;
    SDL_Renderer *renderer = NULL;
    Uint32 fenster_flags = testlauf ? SDL_WINDOW_HIDDEN : SDL_WINDOW_RESIZABLE;
    bool laeuft = true;
    bool spiel_gestartet = false;
    int status = 1; /* Gilt als Fehler, bis der Ablauf regulaer endet. */

    /* Bewusstes C-Muster: goto springt nur vorwaerts zum gemeinsamen
       Aufraeumen am Funktionsende. Nicht als allgemeine Ablaufsteuerung
       verwenden. */
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        goto ende;
    }
    fenster = SDL_CreateWindow("Tresorraub",
                              SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                              640, 480, fenster_flags);
    if (fenster == NULL) {
        goto ende;
    }
    /* Softwaredarstellung genuegt fuer die kleinen statischen Motive. */
    renderer = SDL_CreateRenderer(fenster, -1, SDL_RENDERER_SOFTWARE);
    if (renderer == NULL) {
        goto ende;
    }
    if (SDL_RenderSetLogicalSize(renderer, 640, 480) < 0) {
        goto ende;
    }

    if (!galerie) {
        spiel_starten();
        spiel_gestartet = true;
    }
    bool titel_aktualisieren = spiel_gestartet;
    while (laeuft) {
        SDL_Event ereignis;
        while (laeuft && SDL_PollEvent(&ereignis)) {
            laeuft = ereignis_bearbeiten(&ereignis, galerie);
            if (spiel_gestartet && ereignis.type == SDL_KEYDOWN) {
                titel_aktualisieren = true;
            }
        }
        if (!laeuft) {
            break;
        }
        if (titel_aktualisieren) {
            char titel[96];
            if (spiel_status(titel, sizeof(titel)) < 0) {
                SDL_SetError("Spielstatus konnte nicht erstellt werden.");
                goto ende;
            }
            SDL_SetWindowTitle(fenster, titel);
            titel_aktualisieren = false;
        }
        if (SDL_SetRenderDrawColor(renderer, 245, 246, 249, 255) < 0
            || SDL_RenderClear(renderer) < 0
            || SDL_SetRenderDrawColor(renderer, 23, 37, 59, 255) < 0) {
            goto ende;
        }

        int ergebnis = galerie ? zeichne_galerie(renderer) : zeichne(renderer);
        if (ergebnis < 0) {
            goto ende;
        }
        SDL_RenderPresent(renderer); /* SDL2: void, kein Fehlercode. */
        if (testlauf) {
            break;
        }
        SDL_Delay(16); /* Keine leere Dauerschleife mit voller CPU-Auslastung. */
    }
    status = 0;

ende:
    if (status != 0) {
        /* Meldung vor dem Aufraeumen auslesen, nicht erst danach. */
        fprintf(stderr, "SDL-Fehler: %s\n", SDL_GetError());
    }
    if (spiel_gestartet) {
        aufraeumen();
    }
    if (renderer != NULL) {
        SDL_DestroyRenderer(renderer);
    }
    if (fenster != NULL) {
        SDL_DestroyWindow(fenster);
    }
    SDL_Quit();
    return status;
}
