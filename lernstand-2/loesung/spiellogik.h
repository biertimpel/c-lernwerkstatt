/* Spiellogik: Zuege, Spielende, Bewegung, Leertaste, Ablegen, Ausgabe. */
#ifndef SPIELLOGIK_H
#define SPIELLOGIK_H

#include "spiel.h"

/* Prueft nach einem Zug, ob das Spiel verloren ist (entdeckt oder
   Zuglimit erreicht). Im Dunkeln entdecken die Waechter niemanden.
   Aendert nichts, wenn das Spiel schon zu Ende ist. */
void spielende_pruefen(Spielstand *s);

/* Merkt sich die Zeit und schaltet das Licht wieder ein, wenn die Dauer
   aus den Regeln abgelaufen ist; danach gilt die Entdeckung wieder.
   1, wenn sich etwas geaendert hat, sonst 0. */
int zeit_fortschreiben(Spielstand *s, Uint64 jetzt_ms);

/* Zaehlt einen Zug und prueft danach das Spielende. */
void zug_zaehlen(Spielstand *s);

/* Die folgenden drei Funktionen aendern nach dem Spielende nichts mehr. */

/* Genau ein Schritt zu einem Nachbarfeld. true, wenn die Figur gezogen ist.
   Betritt die Figur ein Objekt mit Methode betreten, wird sie aufgerufen. */
bool figur_bewegen(Spielstand *s, int dx, int dy);

/* Leertaste. 0, auch bei Spielfaellen; negativ bei einem Programmfehler. */
int handeln(Spielstand *s);

/* Taste L. Ergebnis: Zahl der abgelegten Gegenstaende. */
int ablegen(Spielstand *s);

/* Zeichen eines Feldes fuer die Textausgabe. */
char feldzeichen(const Spielstand *s, Punkt p);
void spielfeld_ausgeben(const Spielstand *s);

/* Statuszeile fuer den Fenstertitel. 0 oder -1, wenn ziel zu klein ist. */
int spielstand_lesen(const Spielstand *s, char ziel[], size_t groesse);

#endif
