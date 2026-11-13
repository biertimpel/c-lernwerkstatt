/* Tests zur Musterloesung. Prueft die erwarteten Werte, die auf den
   Seiten des Kapitels in den Selbstkontrollen genannt werden. */
#include "tresorraub.c"

static int fehler = 0;
#define PRUEFE(bedingung) do { \
        if (!(bedingung)) { \
            printf("FEHLGESCHLAGEN (Zeile %d): %s\n", __LINE__, #bedingung); \
            fehler++; \
        } \
    } while (0)

int main(void) {
    /* Seite 1 */
    int zuege[] = {41, 60, 37, 52, 61};
    size_t n = sizeof(zuege) / sizeof(zuege[0]);
    PRUEFE(n == 5);
    PRUEFE(versuche_im_limit(zuege, n) == 4);
    PRUEFE(kuerzester_versuch(zuege, n) == 37);
    PRUEFE(durchschnitt(zuege, n) > 50.19 && durchschnitt(zuege, n) < 50.21);
    int einer[] = {60};
    PRUEFE(versuche_im_limit(einer, 1) == 1);
    PRUEFE(kuerzester_versuch(einer, 1) == 60);
    int zu_lang[] = {61, 70, 99};
    PRUEFE(versuche_im_limit(zu_lang, 3) == 0);
    int vorne[] = {30, 50, 40};
    int hinten[] = {50, 40, 30};
    PRUEFE(kuerzester_versuch(vorne, 3) == 30);
    PRUEFE(kuerzester_versuch(hinten, 3) == 30);
    PRUEFE(kuerzester_versuch(zuege, 0) == -1);
    PRUEFE(durchschnitt(zuege, 0) == 0.0);

    /* Seite 2 */
    char gegenstand[16] = "Beute";
    PRUEFE(sizeof(gegenstand) == 16 && strlen(gegenstand) == 5);
    PRUEFE(gegenstand[4] == 'e' && gegenstand[5] == '\0');
    char zeile[64];
    PRUEFE(statuszeile(zeile, sizeof(zeile), (Punkt){3, 7}, 0) == 0);
    PRUEFE(strcmp(zeile, "Position (3, 7) | Zuege 0/60") == 0);
    char klein[8];
    PRUEFE(statuszeile(klein, sizeof(klein), (Punkt){3, 7}, 0) == -1);
    PRUEFE(strlen(klein) == 7);   /* gekuerzt, aber abgeschlossen */

    /* Seite 3 */
    for (int y = 0; y < HOEHE; y++) {
        PRUEFE(strlen(GRUNDRISS[y]) == BREITE);
    }
    PRUEFE(anzahl_zeichen('#') == 57);
    PRUEFE(anzahl_zeichen('.') == 78);
    PRUEFE(innerhalb((Punkt){3, 7}) && begehbar((Punkt){3, 7}));
    PRUEFE(innerhalb((Punkt){0, 0}) && !begehbar((Punkt){0, 0}));
    PRUEFE(innerhalb((Punkt){14, 8}) && !begehbar((Punkt){14, 8}));
    PRUEFE(!innerhalb((Punkt){15, 3}) && !begehbar((Punkt){15, 3}));
    PRUEFE(!innerhalb((Punkt){-1, 3}) && !begehbar((Punkt){-1, 3}));
    PRUEFE(!innerhalb((Punkt){3, -1}) && !innerhalb((Punkt){3, 9}));
    PRUEFE(begehbar((Punkt){6, 3}));

    /* Seite 4 */
    PRUEFE(rasterabstand((Punkt){1, 5}, (Punkt){3, 6}) == 3);
    PRUEFE(rasterabstand((Punkt){3, 6}, (Punkt){1, 5}) == 3);
    PRUEFE(rasterabstand((Punkt){3, 6}, (Punkt){3, 6}) == 0);
    Spielstand s = startzustand();
    Waechter *w = s.waechter;
    PRUEFE(!ueberwacht((Punkt){3, 7}, w, ANZAHL_WAECHTER));
    PRUEFE(ueberwacht((Punkt){2, 5}, w, ANZAHL_WAECHTER));
    PRUEFE(ueberwacht((Punkt){9, 7}, w, ANZAHL_WAECHTER));
    PRUEFE(ueberwacht((Punkt){11, 5}, w, ANZAHL_WAECHTER));
    PRUEFE(!ueberwacht((Punkt){12, 5}, w, ANZAHL_WAECHTER));
    PRUEFE(!ueberwacht((Punkt){11, 6}, w, ANZAHL_WAECHTER));
    PRUEFE(ueberwacht((Punkt){1, 5}, w, ANZAHL_WAECHTER));
    int frei_ueberwacht = 0;
    for (int y = 0; y < HOEHE; y++) {
        for (int x = 0; x < BREITE; x++) {
            if (begehbar((Punkt){x, y}) && ueberwacht((Punkt){x, y}, w, ANZAHL_WAECHTER)) {
                frei_ueberwacht++;
            }
        }
    }
    PRUEFE(frei_ueberwacht == 17);

    /* Seite 5 */
    PRUEFE(s.zuege == 0 && punkt_gleich(s.figur, (Punkt){3, 7}));
    PRUEFE(!s.hat_schluessel && !s.hat_beute && !s.hat_muenze);
    PRUEFE(!s.tuer_offen && s.beute_im_tresor);
    PRUEFE(feldzeichen(s, (Punkt){3, 7}) == '@');
    PRUEFE(feldzeichen(s, (Punkt){1, 1}) == 'E');
    PRUEFE(feldzeichen(s, (Punkt){3, 5}) == 'k');
    PRUEFE(feldzeichen(s, (Punkt){6, 3}) == 'D');
    PRUEFE(feldzeichen(s, (Punkt){13, 1}) == 'T');
    PRUEFE(feldzeichen(s, (Punkt){11, 7}) == 'c');
    PRUEFE(feldzeichen(s, (Punkt){1, 5}) == 'W');
    PRUEFE(feldzeichen(s, (Punkt){9, 5}) == 'W');
    PRUEFE(feldzeichen(s, (Punkt){2, 5}) == '!');
    Spielstand kopie = s;
    kopie.zuege = 5;
    kopie.figur.x = 4;
    PRUEFE(kopie.zuege == 5 && kopie.figur.x == 4);
    PRUEFE(s.zuege == 0 && s.figur.x == 3);   /* Struktur wird kopiert */
    PRUEFE(sizeof(Spielstand) >= 2 * sizeof(Waechter));

    if (fehler == 0) {
        printf("Alle Tests bestanden.\n");
    }
    return fehler == 0 ? 0 : 1;
}
