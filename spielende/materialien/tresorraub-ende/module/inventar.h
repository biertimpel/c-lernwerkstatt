/* Inventar: einfach verkettete Liste von Pointern auf Gegenstaende. */
#ifndef INVENTAR_H
#define INVENTAR_H

#include "spiel.h"

size_t inventar_anzahl(const Inventar *inventar);

/* Erster Gegenstand dieser Art im Inventar, sonst NULL. */
Objekt *inventar_finden(const Inventar *inventar, const Objektart *art);

/* Nimmt einen Gegenstand vorne auf. AUFGENOMMEN, INVENTAR_VOLL oder
   FEHLER_SPEICHER; in beiden letzten Faellen bleibt das Inventar gleich. */
Aufnahme inventar_aufnehmen(Inventar *inventar, Objekt *gegenstand, size_t hoechstens);

void inventar_ausgeben(const Inventar *inventar);

/* Entfernt den Eintrag dieses Gegenstands. true, wenn er vorhanden war. */
bool inventar_entfernen(Inventar *inventar, const Objekt *gegenstand);

/* Gibt alle Eintraege frei (nicht die Gegenstaende). Danach leer. */
void inventar_leeren(Inventar *inventar);

#endif
