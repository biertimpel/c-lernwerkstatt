/* Spielregeln aus regeln.txt (Kapitel Config). */
#ifndef REGELN_H
#define REGELN_H

#include "spielfeld.h"

#define REGELDATEI "regeln.txt"

typedef struct {
    int zuglimit;
    int reichweite[ANZAHL_WAECHTER];
    int inventar_groesse;
} Regeln;

/* Standardwerte: Zuglimit 60, Reichweiten 1 und 2, Inventargroesse 3. */
Regeln regeln_standard(void);

/* Liest die Regeln aus der Datei. Nicht genannte Regeln behalten ihren
   bisherigen Wert. Ergebnis: 0 gelesen; 1 Datei fehlt (Regeln unveraendert,
   Hinweis im Terminal); -1 Fehler (Regeln unveraendert, Meldung mit
   Zeilennummer auf stderr). */
int regeln_laden(const char *dateiname, Regeln *regeln);

#endif
