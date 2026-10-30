#ifndef MESSSTATION_EINGABE_H
#define MESSSTATION_EINGABE_H

/* Liest eine einzelne ganze Zahl pro Zeile aus stdin.
   Leerraum um die Zahl ist erlaubt. Bei technischen Fehlern:
   Meldung auf stderr und Programmende mit Status 1.
   Eine fachliche Bewertung des Werts findet nicht statt. */
int lese_ganzzahl(void);

#endif
