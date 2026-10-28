#ifndef EINGABE_H
#define EINGABE_H
#include <stddef.h>
int befehl_lesen(char *befehl);
/* Genau anzahl ganze Zahlen, keine weiteren Zeichen. */
int zahlen_lesen(const char *text, int *werte, size_t anzahl);
#endif
