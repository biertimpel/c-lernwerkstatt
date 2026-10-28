#include "anzeige.h"
#include "eingabe.h"
#include "speichern.h"
#include <stdio.h>
#include <string.h>

static int aktion(Spiel *spiel, char befehl, const char *pfad) {
    switch (befehl) {
        case 'w': return spiel_bewegen(spiel, 0, -1);
        case 'a': return spiel_bewegen(spiel, -1, 0);
        case 's': return spiel_bewegen(spiel, 0, 1);
        case 'd': return spiel_bewegen(spiel, 1, 0);
        case 'z': return spiel_warten(spiel);
        case 'i': return anzeige_inventar(spiel);
        case 'h': return anzeige_hilfe();
        case 'p': {
            int e = spiel_speichern(spiel, pfad);
            if (e != OK) return e;
            return anzeige_text("Spielstand gespeichert.");
        }
        case 'l': return spiel_laden(spiel, pfad);
        default: return FEHLER_EINGABE;
    }
}
int main(int argc, char **argv) {
    const char *pfad = "spielstand.sav";
    bool laden = false, debug = false;
    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "--debug") == 0) debug = true;
        else if ((strcmp(argv[i], "--load") == 0 || strcmp(argv[i], "--save-file") == 0) && i + 1 < argc) {
            laden = strcmp(argv[i], "--load") == 0;
            pfad = argv[++i];
        } else if (strcmp(argv[i], "--help") == 0) {
            return anzeige_hilfe() == OK ? 0 : 1;
        } else {
            fprintf(stderr, "Aufruf: %s [--debug] [--load DATEI | --save-file DATEI]\n", argv[0]);
            return 1;
        }
    }
    Spiel spiel = {0};
    int ergebnis = spiel_neu(&spiel);
    if (ergebnis == OK && laden) ergebnis = spiel_laden(&spiel, pfad);
    if (ergebnis != OK) {
        fprintf(stderr, "Start fehlgeschlagen (Fehlercode %d).\n", ergebnis);
        spiel_freigeben(&spiel);
        return 1;
    }
    if (anzeige_hilfe() != OK) ergebnis = FEHLER_DATEI;
    while (ergebnis == OK) {
        ergebnis = anzeige_spiel(&spiel);
        if (ergebnis != OK || spiel.phase != LAEUFT) break;
        char befehl = '\0';
        ergebnis = befehl_lesen(&befehl);
        if (ergebnis == ENDE_EINGABE || (ergebnis == OK && befehl == 'q')) {
            ergebnis = anzeige_text("Spiel beendet.");
            break;
        }
        if (ergebnis == FEHLER_EINGABE) {
            ergebnis = anzeige_text("Bitte genau einen angebotenen Befehl eingeben.");
            continue;
        }
        if (ergebnis != OK) break;
        ergebnis = aktion(&spiel, befehl, pfad);
        if (debug && fprintf(stderr, "[DEBUG] Befehl=%c Ergebnis=%d Position=(%d,%d) Zuege=%d Phase=%d\n",
            befehl, ergebnis, spiel.spieler.position.x, spiel.spieler.position.y,
            spiel.zuege, (int)spiel.phase) < 0) { ergebnis = FEHLER_DATEI; break; }
        if (ergebnis == BLOCKIERT) ergebnis = OK;
        else if (ergebnis == FEHLER_EINGABE) {
            ergebnis = anzeige_text("Unbekannter Befehl. Kein Zug verbraucht.");
        } else if (ergebnis == FEHLER_DATEI || ergebnis == FEHLER_FORMAT || ergebnis == FEHLER_ARGUMENT) {
            if (printf("Aktion fehlgeschlagen (Fehlercode %d). Spielzustand bleibt bei Ladefehler erhalten.\n", ergebnis) < 0)
                break;
            ergebnis = OK;
        }
    }
    spiel_freigeben(&spiel);
    if (ergebnis != OK) fprintf(stderr, "Programmfehler %d.\n", ergebnis);
    return ergebnis == OK ? 0 : 1;
}
