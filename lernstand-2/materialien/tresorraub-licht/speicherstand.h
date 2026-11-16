/* Spielstand speichern und laden, Format Version 2 (FORMAT.md). */
#ifndef SPEICHERSTAND_H
#define SPEICHERSTAND_H

#include "spiel.h"

#define SPIELSTANDDATEI "spielstand.txt"

/* 0 bei Erfolg, -1 Dateifehler, -2 nach dem Spielende. */
int spielstand_speichern(const Spielstand *s, const char *pfad);

/* Alles oder nichts. 0 Erfolg; -1 Dateifehler; -2 Format oder Regeln
   verletzt; -3 kein Speicher. Bei einem Fehler bleibt *ziel unveraendert. */
int spielstand_laden(Spielstand *ziel, const char *pfad);

#endif
