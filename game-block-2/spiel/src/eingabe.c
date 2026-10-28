#include "eingabe.h"
#include "spiel.h"
#include <ctype.h>
#include <errno.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int zahlen_lesen(const char *text, int *werte, size_t anzahl) {
    if (!text || !werte) return FEHLER_ARGUMENT;
    const char *p = text;
    for (size_t i = 0; i < anzahl; i++) {
        while (isspace((unsigned char)*p)) p++;
        errno = 0;
        char *ende;
        long wert = strtol(p, &ende, 10);
        if (p == ende || errno == ERANGE || wert < INT_MIN || wert > INT_MAX)
            return FEHLER_FORMAT;
        if (*ende && !isspace((unsigned char)*ende)) return FEHLER_FORMAT;
        werte[i] = (int)wert;
        p = ende;
    }
    while (isspace((unsigned char)*p)) p++;
    return *p ? FEHLER_FORMAT : OK;
}
int befehl_lesen(char *befehl) {
    char zeile[128];
    if (!befehl) return FEHLER_ARGUMENT;
    if (!fgets(zeile, sizeof zeile, stdin))
        return ferror(stdin) ? FEHLER_DATEI : ENDE_EINGABE;
    if (!strchr(zeile, '\n') && !feof(stdin)) {
        int c;
        while ((c = getchar()) != '\n' && c != EOF) { }
        return ferror(stdin) ? FEHLER_DATEI : FEHLER_EINGABE;
    }
    char *p = zeile;
    while (isspace((unsigned char)*p)) p++;
    if (!*p) return FEHLER_EINGABE;
    *befehl = (char)tolower((unsigned char)*p++);
    while (isspace((unsigned char)*p)) p++;
    return *p ? FEHLER_EINGABE : OK;
}
