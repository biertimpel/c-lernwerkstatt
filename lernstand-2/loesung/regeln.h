/* Spielregeln aus regeln.txt (Kapitel Config). */
#ifndef REGELN_H
#define REGELN_H

#include "spielfeld.h"

#define REGELDATEI "regeln.txt"

typedef struct {
    int zuglimit;
    int reichweite[ANZAHL_WAECHTER];
    int inventar_groesse;
    Punkt waechter[ANZAHL_WAECHTER];   /* Positionen der Waechter */
    Punkt schluessel;                  /* Startposition des Schluessels */
    Punkt lichtschalter;
    int licht_dauer;                   /* Sekunden, die das Licht aus bleibt */
} Regeln;

/* Standardwerte: Zuglimit 60, Reichweiten 1 und 2, Inventargroesse 3,
   Waechter auf (1, 5) und (9, 5), Schluessel auf (3, 5), Lichtschalter auf
   (5, 7), Licht 5 Sekunden aus. Ohne regeln.txt ist das Spielfeld also
   dasselbe wie bisher, nur mit Lichtschalter. */
Regeln regeln_standard(void);

/* Liest die Regeln aus der Datei. Nicht genannte Regeln behalten ihren
   bisherigen Wert. Ergebnis: 0 gelesen; 1 Datei fehlt (Regeln unveraendert,
   Hinweis im Terminal); -1 Fehler (Regeln unveraendert, Meldung mit
   Zeilennummer auf stderr). Prueft jeden Wert nur fuer sich (Bereich);
   ob die Positionen zusammenpassen, prueft aufstellung_pruefen (objekte.h). */
int regeln_laden(const char *dateiname, Regeln *regeln);

#endif
