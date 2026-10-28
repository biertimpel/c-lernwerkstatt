#include "anzeige.h"
#include "inventar.h"
#include <stdio.h>

int anzeige_text(const char *text) { return puts(text) == EOF ? FEHLER_DATEI : OK; }
int anzeige_hilfe(void) {
    return anzeige_text("W/A/S/D: bewegen, Z: warten, I: Inventar, P: speichern, L: laden, H: Hilfe, Q: Ende.\n"
        "Jeden Befehl mit Enter bestaetigen. @ Sie, # Wand, ! ueberwacht, W Waechter.\n"
        "k Schluessel, D Tuer, / offene Tuer, T Tresor, t leerer Tresor, E Ausgang, c Muenze.\n"
        "Nur erfolgreiche Bewegungen und Warten kosten einen Zug. Maximal 60 Zuege.\n"
        "Waechter stehen still. Abstand = horizontale + vertikale Schritte.\n"
        "Der markierte Ueberwachungsbereich wirkt auch durch Waende. Kein echtes Sichtfeld.");
}
int anzeige_inventar(const Spiel *spiel) {
    if (anzeige_text("Inventar:") != OK) return FEHLER_DATEI;
    if (!spiel->spieler.inventar) return anzeige_text("  (leer)");
    for (const Inventarknoten *k = spiel->spieler.inventar; k; k = k->naechster) {
        if (printf("  - %s\n", k->name) < 0) return FEHLER_DATEI;
    }
    return OK;
}
int anzeige_spiel(const Spiel *spiel) {
    if (printf("\nTRESORRAUB | Zug %d/%d | Beute: %s | Bonusmuenze: %s\n",
        spiel->zuege, ZUGLIMIT, inventar_hat(&spiel->spieler, BEUTE) ? "ja" : "nein",
        inventar_hat(&spiel->spieler, MUENZE) ? "ja" : "nein") < 0) return FEHLER_DATEI;
    for (int y = 0; y < HOEHE; y++) {
        for (int x = 0; x < BREITE; x++) {
            if (putchar(spiel_zeichen(spiel, (Punkt){x, y})) == EOF) return FEHLER_DATEI;
        }
        if (putchar('\n') == EOF) return FEHLER_DATEI;
    }
    if (anzeige_text(spiel->meldung) != OK) return FEHLER_DATEI;
    if (spiel->phase != LAEUFT) return OK;
    if (printf("W/A/S/D, I, P, L, H, Z, Q > ") < 0 || fflush(stdout) == EOF)
        return FEHLER_DATEI;
    return OK;
}
