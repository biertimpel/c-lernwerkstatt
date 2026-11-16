/* Ihr Code fuer das Kapitel Datenstrukturen.
   Grundriss, Positionen und Regeln stehen in SPIELFELD.md. */
#include "tresorraub.h"
#include "piktogramme/piktogramme.h"
#include <stdio.h>

/* Ein Spielfeld ist auf dem Bildschirm 32 x 32 Pixel gross:
   Jedes Piktogramm hat 16 x 16 Rasterzellen, Skalierung 2. */
#define SKALIERUNG 2
#define FELDGROESSE (16 * SKALIERUNG)

/* Ihre eigenen Funktionen schreiben Sie hier, oberhalb von text_ausgeben
   und zeichne. Sie gehoeren nicht in tresorraub.h. */

int text_ausgeben(void) {
    /* Hier entstehen Ihre Versuche und Textausgaben.
       Starten mit: ./programm --text */
    printf("Noch keine Textausgabe.\n");
    return 0;
}

int zeichne(SDL_Renderer *renderer) {
    /* Hier entsteht die Darstellung des Spielfelds im Fenster.
       Starten mit: ./programm
       Beispiel: Die Spielfigur auf dem Feld (3, 7). Ersetzen Sie das durch
       Ihre eigene Darstellung. Den Rueckgabewert geben Sie weiter. */
    return zeichne_spielfigur_froh(renderer, 3 * FELDGROESSE, 7 * FELDGROESSE,
                                   SKALIERUNG);
}

/* Diese Funktionen ruft das Geruest im Fenster ebenfalls auf. Sie brauchen
   sie erst ab dem Kapitel "Pointer und Datenaustausch". Bis dahin lassen
   Sie die Platzhalter unveraendert. */

int spiel_starten(void) {
    return 0;
}

int taste_gedrueckt(SDL_Keycode taste) {
    (void)taste; /* Platzhalter: entfernen, sobald Sie taste verwenden. */
    return 0;
}

int spiel_status(char ziel[], size_t groesse) {
    int laenge = snprintf(ziel, groesse, "Tresorraub");
    return laenge < 0 || (size_t)laenge >= groesse ? -1 : 0;
}

void aufraeumen(void) {
}
