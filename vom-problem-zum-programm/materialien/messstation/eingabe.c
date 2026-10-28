/* Vorgegebene technische Infrastruktur, keine fachliche Musterloesung.
   Arrays, Pointer und Header muessen fuer die Verwendung dieses
   Bausteins noch nicht verstanden oder selbst implementiert werden. */
#include <ctype.h>
#include <errno.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "eingabe.h"

static void eingabefehler(void) {
    fprintf(stderr, "Eingabefehler: Eine ganze Zahl pro Zeile erwartet.\n");
    exit(1);
}

int lese_ganzzahl(void) {
    char zeile[128];
    if (fgets(zeile, sizeof zeile, stdin) == NULL) {
        eingabefehler();
    }
    if (strchr(zeile, '\n') == NULL && !feof(stdin)) {
        eingabefehler();
    }

    char *ende;
    errno = 0;
    long wert = strtol(zeile, &ende, 10);
    if (ende == zeile || errno == ERANGE || wert < INT_MIN || wert > INT_MAX) {
        eingabefehler();
    }
    while (isspace((unsigned char)*ende)) {
        ende++;
    }
    if (*ende != '\0') {
        eingabefehler();
    }
    return (int)wert;
}
