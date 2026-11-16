/* Spielregeln aus regeln.txt (Kapitel Config). */
#include "regeln.h"
#include <ctype.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define ZEILE_MAX 80          /* laengste erlaubte Zeile in regeln.txt */

Regeln regeln_standard(void) {
    Regeln r = {.zuglimit = 60, .reichweite = {1, 2}, .inventar_groesse = 3};
    return r;
}

/* Alles, was das Einlesen ueber eine Regel wissen muss. */
typedef struct {
    const char *name;
    int min;
    int max;
    int *feld;      /* hierhin kommt der gelesene Wert */
    bool gelesen;   /* stand die Regel schon in der Datei? */
} Regelangabe;

static bool nur_leerraum(const char *text) {
    for (; *text != '\0'; text++) {
        if (!isspace((unsigned char)*text)) {
            return false;
        }
    }
    return true;
}

/* Wertet eine Zeile aus. Leerzeilen und Kommentare werden uebersprungen.
   Ein gueltiger Wert kommt in das Feld seiner Regelangabe.
   Ergebnis: 0 bei Erfolg, -1 bei einem Fehler; dann steht der Grund in
   grund. Veraendert zeile. */
static int zeile_auswerten(char *zeile, Regelangabe angaben[], size_t anzahl,
                           char grund[], size_t groesse) {
    const char *anfang = zeile;
    while (isspace((unsigned char)*anfang)) {
        anfang++;
    }
    if (*anfang == '\0' || *anfang == '#') {
        return 0;
    }
    char *gleich = strchr(zeile, '=');
    if (gleich == NULL) {
        snprintf(grund, groesse, "kein \"=\" gefunden");
        return -1;
    }
    *gleich = '\0';   /* trennt Name und Wert */
    char name[32];
    char rest;
    if (sscanf(zeile, "%31s %c", name, &rest) != 1) {
        snprintf(grund, groesse, "vor \"=\" muss genau ein Name stehen");
        return -1;
    }
    Regelangabe *angabe = NULL;
    for (size_t i = 0; i < anzahl && angabe == NULL; i++) {
        if (strcmp(name, angaben[i].name) == 0) {
            angabe = &angaben[i];
        }
    }
    if (angabe == NULL) {
        snprintf(grund, groesse, "unbekannte Regel \"%s\"", name);
        return -1;
    }
    if (angabe->gelesen) {
        snprintf(grund, groesse, "Regel \"%s\" steht doppelt", name);
        return -1;
    }
    char *ende;
    long wert = strtol(gleich + 1, &ende, 10);
    if (ende == gleich + 1 || !nur_leerraum(ende)) {
        snprintf(grund, groesse, "Wert von \"%s\" ist keine ganze Zahl", name);
        return -1;
    }
    if (wert < angabe->min || wert > angabe->max) {
        snprintf(grund, groesse, "Wert von \"%s\" muss zwischen %d und %d liegen",
                 name, angabe->min, angabe->max);
        return -1;
    }
    *angabe->feld = (int)wert;
    angabe->gelesen = true;
    return 0;
}

/* Liest die Regeln aus der Datei. Nicht genannte Regeln behalten ihren
   bisherigen Wert.
   Ergebnis: 0, wenn die Datei fehlerfrei gelesen wurde; 1, wenn sie fehlt
   (Regeln unveraendert, Hinweis im Terminal); -1 bei einem Fehler (Regeln
   unveraendert, Meldung mit Zeilennummer). */
int regeln_laden(const char *dateiname, Regeln *regeln) {
    FILE *datei = fopen(dateiname, "r");
    if (datei == NULL) {
        printf("%s nicht gefunden, es gelten die Standardregeln.\n", dateiname);
        return 1;
    }
    Regeln neu = *regeln;
    Regelangabe angaben[] = {
        {"zuglimit",              1, 999, &neu.zuglimit,         false},
        {"reichweite_waechter_1", 0, 5,   &neu.reichweite[0],    false},
        {"reichweite_waechter_2", 0, 5,   &neu.reichweite[1],    false},
        {"inventar_groesse",      1, 3,   &neu.inventar_groesse, false},
    };
    size_t anzahl = sizeof(angaben) / sizeof(angaben[0]);
    char zeile[ZEILE_MAX + 2];   /* Platz fuer '\n' und '\0' */
    char grund[96] = "";
    int nummer = 0;
    int ergebnis = 0;
    while (fgets(zeile, sizeof(zeile), datei) != NULL) {
        nummer++;
        if (strchr(zeile, '\n') == NULL && !feof(datei)) {
            snprintf(grund, sizeof(grund), "Zeile ist laenger als %d Zeichen", ZEILE_MAX);
            ergebnis = -1;
            break;
        }
        if (zeile_auswerten(zeile, angaben, anzahl, grund, sizeof(grund)) < 0) {
            ergebnis = -1;
            break;
        }
    }
    if (ergebnis == 0 && ferror(datei)) {
        snprintf(grund, sizeof(grund), "Lesefehler");
        ergebnis = -1;
    }
    fclose(datei);   /* nur gelesen: kein gepufferter Schreibfehler moeglich */
    if (ergebnis < 0) {
        fprintf(stderr, "%s, Zeile %d: %s\n", dateiname, nummer, grund);
        return -1;
    }
    *regeln = neu;
    return 0;
}

