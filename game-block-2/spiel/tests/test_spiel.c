#include "spiel.h"
#include "eingabe.h"
#include "inventar.h"
#include "objekt.h"
#include "speichern.h"
#include <assert.h>
#include <stdio.h>
#include <string.h>

static unsigned pruefungen = 0;
#define PRUEFE(x) do { bool bestanden_ = (x); pruefungen++; if (!bestanden_) { \
    fprintf(stderr, "Fehlgeschlagen: %s, Zeile %d\n", #x, __LINE__); \
    assert(bestanden_); } } while (0)

static const char *HINWEG = "wwddwwddwwdddddd";
static const char *RUECKWEG = "aaaaaassaawwaaaa";
static const char *DATEI = "build/test-spielstand.sav";

static int schritt(Spiel *s, char c) {
    switch (c) {
        case 'w': return spiel_bewegen(s, 0, -1);
        case 'a': return spiel_bewegen(s, -1, 0);
        case 's': return spiel_bewegen(s, 0, 1);
        case 'd': return spiel_bewegen(s, 1, 0);
        default: return FEHLER_ARGUMENT;
    }
}
static void route(Spiel *s, const char *befehle) {
    for (const char *p = befehle; *p; p++) PRUEFE(schritt(s, *p) == OK);
}
static void datei_schreiben(const char *text) {
    FILE *f = fopen(DATEI, "w");
    PRUEFE(f != NULL);
    PRUEFE(fputs(text, f) != EOF);
    PRUEFE(fclose(f) == 0);
}
static void grundlagen(void) {
    Spiel s;
    PRUEFE(spiel_neu(&s) == OK);
    PRUEFE(s.phase == LAEUFT && s.zuege == 0);
    PRUEFE(punkt_gleich(s.spieler.position, (Punkt){3, 7}));
    for (int y = 0; y < HOEHE; y++) PRUEFE(strlen(s.karte[y]) == BREITE);
    PRUEFE(manhattan_abstand((Punkt){1, 1}, (Punkt){4, 5}) == 7);
    PRUEFE(!spiel_betretbar(&s, (Punkt){-1, 0}));
    PRUEFE(!spiel_betretbar(&s, (Punkt){BREITE, 3}));
    PRUEFE(spiel_bewegen(&s, 2, 0) == FEHLER_ARGUMENT);
    PRUEFE(spiel_bewegen(&s, 0, 1) == BLOCKIERT);
    PRUEFE(s.zuege == 0);
    PRUEFE(spiel_ueberwacht(&s, (Punkt){2, 5}));
    PRUEFE(!spiel_ueberwacht(&s, (Punkt){3, 5}));
    PRUEFE(spiel_ueberwacht(&s, (Punkt){9, 3}));
    PRUEFE(!spiel_ueberwacht(&s, (Punkt){9, 2}));
    PRUEFE(objekt_erzeugen(-1) == NULL);
    PRUEFE(objekt_finden(&s, ID_TUER)->methoden != objekt_finden(&s, ID_TRESOR)->methoden);
    PRUEFE(spiel_zeichen(&s, (Punkt){6, 3}) == 'D');
    PRUEFE(inventar_hinzufuegen(&s.spieler, (Gegenstand)99) == FEHLER_ARGUMENT);
    PRUEFE(!inventar_entfernen(&s.spieler, BEUTE));
    spiel_freigeben(&s);
    spiel_freigeben(&s);
}
static void spielwege(void) {
    Spiel s;
    PRUEFE(spiel_neu(&s) == OK);
    route(&s, "ddwwww");
    PRUEFE(spiel_bewegen(&s, 1, 0) == BLOCKIERT);
    PRUEFE(s.zuege == 6 && s.spieler.position.x == 5);
    spiel_freigeben(&s);

    PRUEFE(spiel_neu(&s) == OK);
    route(&s, HINWEG);
    PRUEFE(s.phase == LAEUFT && s.zuege == 16);
    PRUEFE(inventar_hat(&s.spieler, BEUTE));
    PRUEFE(!inventar_hat(&s.spieler, SCHLUESSEL));
    PRUEFE(((Tuer *)objekt_finden(&s, ID_TUER))->offen);
    PRUEFE(spiel_zeichen(&s, (Punkt){6, 3}) == '/');
    route(&s, RUECKWEG);
    PRUEFE(s.phase == GEWONNEN && s.zuege == 32);
    PRUEFE(spiel_warten(&s) == FEHLER_ARGUMENT);
    spiel_freigeben(&s);

    PRUEFE(spiel_neu(&s) == OK);
    route(&s, HINWEG);
    route(&s, "ssssssaaddwwwwww");
    PRUEFE(inventar_hat(&s.spieler, MUENZE));
    PRUEFE(inventar_anzahl(&s.spieler) == 2);
    route(&s, RUECKWEG);
    PRUEFE(s.phase == GEWONNEN && s.zuege == 48);
    spiel_freigeben(&s);

    PRUEFE(spiel_neu(&s) == OK);
    route(&s, "wwa");
    PRUEFE(s.phase == ENTDECKT && s.zuege == 3);
    spiel_freigeben(&s);

    PRUEFE(spiel_neu(&s) == OK);
    for (int n = 0; n < ZUGLIMIT; n++) PRUEFE(spiel_warten(&s) == OK);
    PRUEFE(s.phase == ZEIT_ABGELAUFEN);
    spiel_freigeben(&s);

    PRUEFE(spiel_neu(&s) == OK);
    for (int n = 0; n < 28; n++) PRUEFE(spiel_warten(&s) == OK);
    route(&s, HINWEG); route(&s, RUECKWEG);
    PRUEFE(s.phase == GEWONNEN && s.zuege == ZUGLIMIT);
    PRUEFE(spiel_speichern(&s, DATEI) == FEHLER_ARGUMENT);
    spiel_freigeben(&s);
}
static void speichern_laden(void) {
    Spiel s;
    PRUEFE(spiel_neu(&s) == OK);
    route(&s, "ww");
    PRUEFE(inventar_hat(&s.spieler, SCHLUESSEL));
    PRUEFE(spiel_speichern(&s, DATEI) == OK);
    route(&s, "dd");
    PRUEFE(spiel_laden(&s, DATEI) == OK);
    PRUEFE(s.zuege == 2 && s.spieler.position.x == 3);
    PRUEFE(inventar_hat(&s.spieler, SCHLUESSEL));
    route(&s, "ddwwddwwdddddd");
    PRUEFE(inventar_hat(&s.spieler, BEUTE));
    PRUEFE(spiel_speichern(&s, DATEI) == OK);
    for (int i = 0; i < 25; i++) {
        PRUEFE(spiel_laden(&s, DATEI) == OK);
        PRUEFE(inventar_hat(&s.spieler, BEUTE));
        PRUEFE(((Tuer *)objekt_finden(&s, ID_TUER))->offen);
    }
    route(&s, RUECKWEG);
    PRUEFE(s.phase == GEWONNEN);
    spiel_freigeben(&s);

    PRUEFE(spiel_neu(&s) == OK);
    route(&s, HINWEG); route(&s, "ssssssaa");
    PRUEFE(spiel_speichern(&s, DATEI) == OK);
    PRUEFE(spiel_laden(&s, DATEI) == OK);
    PRUEFE(inventar_hat(&s.spieler, MUENZE));
    PRUEFE(inventar_hat(&s.spieler, BEUTE));
    spiel_freigeben(&s);
}
static void fehlerhafte_dateien(void) {
    const char *dateien[] = {
        "", "FALSCH 1\n", "TRESORRAUB 2\n",
        "TRESORRAUB 1\n3 7 0\n1 0 0 1\n0\nUNERWARTET",
        "TRESORRAUB 1\n-1 7 0\n1 0 0 1\n0\n",
        "TRESORRAUB 1\n3 7 60\n1 0 0 1\n0\n",
        "TRESORRAUB 1\n999999999999999999999 7 0\n1 0 0 1\n0\n",
        "TRESORRAUB 1\n3 7 0\n1 0 0 1\n4\n",
        "TRESORRAUB 1\n3 7 0\n1 0 0 1\n1\n99\n",
        "TRESORRAUB 1\n3 7 0\n0 0 0 1\n2\n1\n1\n",
        "TRESORRAUB 1\n3 7 0\n1 1 0 1\n0\n",
        "TRESORRAUB 1\n3 7 0\n0 0 0 1\n0\n",
        "TRESORRAUB 1\n3 7 0\n1 0 1 1\n0\n",
        "TRESORRAUB 1\n3 7 0\n1 0 0 0\n0\n",
        "TRESORRAUB 1\n2 5 0\n1 0 0 1\n0\n",
        "TRESORRAUB 1\n0 0 0\n1 0 0 1\n0\n",
        "TRESORRAUB 1\n3 7 0\n1 0 0 1\n0x\n",
        "TRESORRAUB 1\n3 7 0\n1 0 0 1\n0\n0\n"
    };
    Spiel s;
    PRUEFE(spiel_neu(&s) == OK);
    route(&s, "w");
    for (size_t i = 0; i < sizeof dateien / sizeof dateien[0]; i++) {
        Spiel vorher = s;
        datei_schreiben(dateien[i]);
        PRUEFE(spiel_laden(&s, DATEI) < 0);
        PRUEFE(s.objekte == vorher.objekte && s.spieler.inventar == vorher.spieler.inventar);
        PRUEFE(punkt_gleich(s.spieler.position, vorher.spieler.position));
        PRUEFE(s.zuege == vorher.zuege && strcmp(s.meldung, vorher.meldung) == 0);
    }
    PRUEFE(spiel_laden(&s, "build/nicht-vorhanden/spiel.sav") == FEHLER_DATEI);
    PRUEFE(spiel_speichern(&s, "build/nicht-vorhanden/spiel.sav") == FEHLER_DATEI);
    PRUEFE(spiel_speichern(&s, NULL) == FEHLER_ARGUMENT);
    spiel_freigeben(&s);
    PRUEFE(remove(DATEI) == 0);
}
static void zahleneingabe(void) {
    int w[3];
    PRUEFE(zahlen_lesen("1 -2 3\n", w, 3) == OK);
    PRUEFE(w[0] == 1 && w[1] == -2 && w[2] == 3);
    PRUEFE(zahlen_lesen("1 2", w, 3) == FEHLER_FORMAT);
    PRUEFE(zahlen_lesen("1 2 3 4", w, 3) == FEHLER_FORMAT);
    PRUEFE(zahlen_lesen("1x 2 3", w, 3) == FEHLER_FORMAT);
    PRUEFE(zahlen_lesen("999999999999999999999999999", w, 1) == FEHLER_FORMAT);
}
#ifdef TEST_ALLOC
static void speicherfehler(void) {
    PRUEFE(probe_offene_bloecke() == 0);
    for (int n = 0; n < 16; n++) {
        probe_fehler_nach(n);
        Spiel s = {0};
        int e = spiel_neu(&s);
        PRUEFE(e == OK || e == FEHLER_SPEICHER);
        spiel_freigeben(&s);
        PRUEFE(probe_offene_bloecke() == 0);
    }
    probe_fehler_nach(-1);
    Spiel s;
    PRUEFE(spiel_neu(&s) == OK);
    route(&s, "w");
    long vorher = probe_offene_bloecke();
    probe_fehler_nach(0);
    PRUEFE(schritt(&s, 'w') == FEHLER_SPEICHER);
    PRUEFE(s.zuege == 1 && s.spieler.position.y == 6);
    PRUEFE(objekt_finden(&s, ID_SCHLUESSEL)->aktiv);
    PRUEFE(probe_offene_bloecke() == vorher);
    probe_fehler_nach(-1);
    route(&s, "w");
    PRUEFE(spiel_speichern(&s, DATEI) == OK);
    for (int n = 0; n < 15; n++) {
        vorher = probe_offene_bloecke();
        Objektknoten *kopf = s.objekte;
        probe_fehler_nach(n);
        PRUEFE(spiel_laden(&s, DATEI) == FEHLER_SPEICHER);
        PRUEFE(s.objekte == kopf && s.zuege == 2);
        PRUEFE(inventar_hat(&s.spieler, SCHLUESSEL));
        PRUEFE(probe_offene_bloecke() == vorher);
    }
    probe_fehler_nach(-1);
    spiel_freigeben(&s);
    PRUEFE(probe_offene_bloecke() == 0);
    PRUEFE(remove(DATEI) == 0);
}
#endif
int main(void) {
    grundlagen(); spielwege(); speichern_laden(); fehlerhafte_dateien(); zahleneingabe();
    for (int i = 0; i < 100; i++) {
        Spiel s; PRUEFE(spiel_neu(&s) == OK); spiel_freigeben(&s);
    }
#ifdef TEST_ALLOC
    speicherfehler();
#endif
    printf("Alle %u Pruefungen bestanden.\n", pruefungen);
    return 0;
}
