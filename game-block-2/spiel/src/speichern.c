#include "speichern.h"
#include "eingabe.h"
#include "inventar.h"
#include "objekt.h"
#include <ctype.h>
#include <stdio.h>
#include <string.h>

static int datei_zahlen(FILE *datei, int *werte, size_t anzahl) {
    char zeile[192];
    if (!fgets(zeile, sizeof zeile, datei)) return FEHLER_FORMAT;
    if (!strchr(zeile, '\n') && !feof(datei)) return FEHLER_FORMAT;
    return zahlen_lesen(zeile, werte, anzahl);
}

int spiel_speichern(const Spiel *spiel, const char *pfad) {
    if (!spiel || !pfad || !*pfad || spiel->phase != LAEUFT) return FEHLER_ARGUMENT;
    const Objekt *key = objekt_finden(spiel, ID_SCHLUESSEL);
    const Tuer *tuer = (const Tuer *)objekt_finden(spiel, ID_TUER);
    const Tresor *tresor = (const Tresor *)objekt_finden(spiel, ID_TRESOR);
    const Objekt *muenze = objekt_finden(spiel, ID_MUENZE);
    FILE *datei = fopen(pfad, "w");
    if (!datei) return FEHLER_DATEI;
    bool fehler = fprintf(datei, "TRESORRAUB 1\n%d %d %d\n%d %d %d %d\n%zu\n",
        spiel->spieler.position.x, spiel->spieler.position.y, spiel->zuege,
        key->aktiv ? 1 : 0, tuer->offen ? 1 : 0,
        tresor->gepluendert ? 1 : 0, muenze->aktiv ? 1 : 0,
        inventar_anzahl(&spiel->spieler)) < 0;
    for (const Inventarknoten *k = spiel->spieler.inventar; k; k = k->naechster) {
        if (fprintf(datei, "%d\n", (int)k->typ) < 0) fehler = true;
    }
    if (fclose(datei) == EOF) fehler = true;
    return fehler ? FEHLER_DATEI : OK;
}

static bool null_oder_eins(int n) { return n == 0 || n == 1; }

int spiel_laden(Spiel *spiel, const char *pfad) {
    if (!spiel || !pfad || !*pfad) return FEHLER_ARGUMENT;
    FILE *datei = fopen(pfad, "r");
    if (!datei) return FEHLER_DATEI;
    int ergebnis = FEHLER_FORMAT;
    Spiel kandidat = {0};
    char kopf[64];
    int p[3], zustand[4], anzahl[1];
    if (!fgets(kopf, sizeof kopf, datei)) goto fertig;
    kopf[strcspn(kopf, "\r\n")] = '\0';
    if (strcmp(kopf, "TRESORRAUB 1") != 0) goto fertig;
    if (datei_zahlen(datei, p, 3) != OK
        || datei_zahlen(datei, zustand, 4) != OK
        || datei_zahlen(datei, anzahl, 1) != OK) goto fertig;
    for (int i = 0; i < 4; i++) if (!null_oder_eins(zustand[i])) goto fertig;
    if (p[2] < 0 || p[2] >= ZUGLIMIT || anzahl[0] < 0
        || anzahl[0] > INVENTAR_LIMIT) goto fertig;
    ergebnis = spiel_neu(&kandidat);
    if (ergebnis != OK) goto fertig;
    ergebnis = FEHLER_FORMAT;
    kandidat.spieler.position = (Punkt){p[0], p[1]};
    kandidat.zuege = p[2];
    Objekt *key = objekt_finden(&kandidat, ID_SCHLUESSEL);
    Tuer *tuer = (Tuer *)objekt_finden(&kandidat, ID_TUER);
    Tresor *tresor = (Tresor *)objekt_finden(&kandidat, ID_TRESOR);
    Objekt *muenze = objekt_finden(&kandidat, ID_MUENZE);
    key->aktiv = zustand[0] != 0;
    tuer->offen = zustand[1] != 0;
    tresor->gepluendert = zustand[2] != 0;
    muenze->aktiv = zustand[3] != 0;
    int typen[INVENTAR_LIMIT];
    for (int i = 0; i < anzahl[0]; i++) {
        int typ[1];
        if (datei_zahlen(datei, typ, 1) != OK) goto fertig;
        if (!gegenstand_name((Gegenstand)typ[0])) goto fertig;
        for (int j = 0; j < i; j++) if (typen[j] == typ[0]) goto fertig;
        typen[i] = typ[0];
    }
    for (int i = anzahl[0] - 1; i >= 0; i--) {
        int hinzu = inventar_hinzufuegen(&kandidat.spieler, (Gegenstand)typen[i]);
        if (hinzu != OK) { ergebnis = hinzu; goto fertig; }
    }
    int c;
    while ((c = fgetc(datei)) != EOF) {
        if (!isspace((unsigned char)c)) goto fertig;
    }
    if (ferror(datei)) { ergebnis = FEHLER_DATEI; goto fertig; }
    if (!spiel_betretbar(&kandidat, kandidat.spieler.position)
        || spiel_ueberwacht(&kandidat, kandidat.spieler.position)) goto fertig;
    if (key->aktiv && tuer->offen) goto fertig;
    if (inventar_hat(&kandidat.spieler, SCHLUESSEL) != (!key->aktiv && !tuer->offen)) goto fertig;
    if (inventar_hat(&kandidat.spieler, BEUTE) != tresor->gepluendert) goto fertig;
    if (inventar_hat(&kandidat.spieler, MUENZE) != !muenze->aktiv) goto fertig;
    if (!tuer->offen && kandidat.spieler.position.x >= 6) goto fertig;
    if (!tuer->offen && (tresor->gepluendert || !muenze->aktiv)) goto fertig;
    if (key->aktiv && punkt_gleich(kandidat.spieler.position, key->position)) goto fertig;
    if (muenze->aktiv && punkt_gleich(kandidat.spieler.position, muenze->position)) goto fertig;
    if (!tresor->gepluendert && punkt_gleich(kandidat.spieler.position, tresor->basis.position)) goto fertig;
    if (tresor->gepluendert && punkt_gleich(kandidat.spieler.position, (Punkt){1, 1})) goto fertig;
    ergebnis = OK;
fertig:
    if (ferror(datei)) ergebnis = FEHLER_DATEI;
    if (fclose(datei) == EOF) ergebnis = FEHLER_DATEI;
    if (ergebnis == OK) {
        spiel_freigeben(spiel);
        *spiel = kandidat;
        snprintf(spiel->meldung, sizeof spiel->meldung, "Spielstand geladen.");
    } else {
        spiel_freigeben(&kandidat);
    }
    return ergebnis;
}
