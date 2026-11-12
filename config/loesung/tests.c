/* Tests zur Musterloesung "Config". Prueft das Einlesen von regeln.txt
   mit gueltigen und fehlerhaften Dateien und die Wirkung im Spiel.
   Legt im aktuellen Ordner Testdateien an und loescht sie wieder. */
#include "tresorraub.c"

static int fehler = 0;
#define PRUEFE(bedingung) do { \
        if (!(bedingung)) { \
            printf("FEHLGESCHLAGEN (Zeile %d): %s\n", __LINE__, #bedingung); \
            fehler++; \
        } \
    } while (0)

#define TESTDATEI "test_regeln.txt"
#define MELDUNGEN "test_meldungen.txt"

static void schreibe(const char *name, const char *inhalt) {
    FILE *f = fopen(name, "w");
    if (f == NULL) {
        printf("Testdatei %s nicht anlegbar\n", name);
        exit(1);
    }
    fputs(inhalt, f);
    fclose(f);
}

static bool gleich(Regeln a, Regeln b) {
    return a.zuglimit == b.zuglimit && a.reichweite[0] == b.reichweite[0]
        && a.reichweite[1] == b.reichweite[1] && a.inventar_groesse == b.inventar_groesse;
}

/* Laedt inhalt und liefert Rueckgabe; die Fehlermeldung landet in meldung. */
static int lade(const char *inhalt, Regeln *r, char meldung[], size_t groesse) {
    schreibe(TESTDATEI, inhalt);
    fflush(stderr);
    if (freopen(MELDUNGEN, "w", stderr) == NULL) {
        exit(1);
    }
    int ergebnis = regeln_laden(TESTDATEI, r);
    fflush(stderr);
    meldung[0] = '\0';
    FILE *f = fopen(MELDUNGEN, "r");
    if (f != NULL) {
        if (fgets(meldung, (int)groesse, f) == NULL) {
            meldung[0] = '\0';
        }
        fclose(f);
    }
    return ergebnis;
}

/* Prueft: Fehler, Regeln unveraendert, Meldung enthaelt teil. */
static void pruefe_fehler(int zeile_im_test, const char *inhalt, const char *teil) {
    Regeln r = regeln_standard();
    char meldung[160];
    int ergebnis = lade(inhalt, &r, meldung, sizeof(meldung));
    if (ergebnis != -1 || !gleich(r, regeln_standard()) || strstr(meldung, teil) == NULL) {
        printf("FEHLGESCHLAGEN (Fall aus Zeile %d): Ergebnis %d, Meldung: %s\n",
               zeile_im_test, ergebnis, meldung);
        fehler++;
    }
}

#define SICHERUNG "regeln.txt.testsicherung"

int main(void) {
    /* Eine vorhandene regeln.txt sichern; die Tests legen eigene an. */
    bool gesichert = rename(REGELDATEI, SICHERUNG) == 0;
    char meldung[160];
    Regeln r = regeln_standard();
    PRUEFE(r.zuglimit == 60 && r.reichweite[0] == 1 && r.reichweite[1] == 2
           && r.inventar_groesse == 3);

    /* Datei fehlt */
    remove(TESTDATEI);
    PRUEFE(regeln_laden(TESTDATEI, &r) == 1 && gleich(r, regeln_standard()));

    /* Vollstaendige Datei */
    r = regeln_standard();
    PRUEFE(lade("zuglimit = 40\nreichweite_waechter_1 = 0\nreichweite_waechter_2 = 3\n"
                "inventar_groesse = 1\n", &r, meldung, sizeof(meldung)) == 0);
    PRUEFE(r.zuglimit == 40 && r.reichweite[0] == 0 && r.reichweite[1] == 3
           && r.inventar_groesse == 1);

    /* Teilweise, mit Kommentaren, Leerzeilen, Leerraum, CRLF, ohne letztes '\n' */
    r = regeln_standard();
    PRUEFE(lade("# Kommentar\n\n   # eingerueckter Kommentar\n  zuglimit=45  \r\n"
                "\treichweite_waechter_2 =\t5", &r, meldung, sizeof(meldung)) == 0);
    PRUEFE(r.zuglimit == 45 && r.reichweite[0] == 1 && r.reichweite[1] == 5
           && r.inventar_groesse == 3);
    /* Leere Datei: alles Standard */
    r = regeln_standard();
    PRUEFE(lade("", &r, meldung, sizeof(meldung)) == 0 && gleich(r, regeln_standard()));
    /* Grenzwerte */
    r = regeln_standard();
    PRUEFE(lade("zuglimit = 1\ninventar_groesse = 3\n", &r, meldung, sizeof(meldung)) == 0);
    PRUEFE(lade("zuglimit = 999\n", &r, meldung, sizeof(meldung)) == 0 && r.zuglimit == 999);
    /* Zeile mit genau 80 Zeichen ist erlaubt */
    char lang[200];
    memset(lang, '#', 80);
    lang[80] = '\n';
    lang[81] = '\0';
    r = regeln_standard();
    PRUEFE(lade(lang, &r, meldung, sizeof(meldung)) == 0);

    /* Fehler: Meldung mit Zeilennummer und Grund, Regeln unveraendert */
    pruefe_fehler(__LINE__, "zuglimit 40\n", "Zeile 1: kein \"=\"");
    pruefe_fehler(__LINE__, "# ok\nzuglimt = 40\n", "Zeile 2: unbekannte Regel \"zuglimt\"");
    pruefe_fehler(__LINE__, "zug limit = 40\n", "Zeile 1: vor \"=\" muss genau ein Name stehen");
    pruefe_fehler(__LINE__, " = 40\n", "Zeile 1: vor \"=\" muss genau ein Name stehen");
    pruefe_fehler(__LINE__, "zuglimit = 40\n\nzuglimit = 50\n", "Zeile 3: Regel \"zuglimit\" steht doppelt");
    pruefe_fehler(__LINE__, "zuglimit = 6O\n", "Zeile 1: Wert von \"zuglimit\" ist keine ganze Zahl");
    pruefe_fehler(__LINE__, "zuglimit = 40 Zuege\n", "keine ganze Zahl");
    pruefe_fehler(__LINE__, "zuglimit =\n", "keine ganze Zahl");
    pruefe_fehler(__LINE__, "zuglimit = 4.5\n", "keine ganze Zahl");
    pruefe_fehler(__LINE__, "zuglimit = 0\n", "muss zwischen 1 und 999 liegen");
    pruefe_fehler(__LINE__, "zuglimit = 1000\n", "muss zwischen 1 und 999 liegen");
    pruefe_fehler(__LINE__, "zuglimit = 99999999999999999999\n", "muss zwischen 1 und 999 liegen");
    pruefe_fehler(__LINE__, "reichweite_waechter_1 = -1\n", "muss zwischen 0 und 5 liegen");
    pruefe_fehler(__LINE__, "reichweite_waechter_2 = 6\n", "muss zwischen 0 und 5 liegen");
    pruefe_fehler(__LINE__, "inventar_groesse = 4\n", "muss zwischen 1 und 3 liegen");
    pruefe_fehler(__LINE__, "zuglimit = 40\ninventar_groesse = 0\n", "Zeile 2:");
    memset(lang, '#', 81);
    lang[81] = '\n';
    lang[82] = '\0';
    pruefe_fehler(__LINE__, lang, "Zeile 1: Zeile ist laenger als 80 Zeichen");

    /* Wirkung im Spiel */
    Regeln weit = regeln_standard();
    weit.reichweite[1] = 3;
    Spielstand s = startzustand(&weit);
    PRUEFE(ueberwacht((Punkt){12, 5}, s.waechter, ANZAHL_WAECHTER));
    r = regeln_standard();
    Spielstand std = startzustand(&r);
    PRUEFE(!ueberwacht((Punkt){12, 5}, std.waechter, ANZAHL_WAECHTER));
    Regeln klein = regeln_standard();
    klein.zuglimit = 40;
    klein.inventar_groesse = 1;
    s = startzustand(&klein);
    char zeile[96];
    PRUEFE(spielstand_lesen(&s, zeile, sizeof(zeile)) == 0
           && strcmp(zeile, "Position (3, 7) | Zuege 0/40") == 0);
    s.tuer_offen = true;
    s.figur = (Punkt){11, 7};
    PRUEFE(handeln(&s) == 0 && inventar_anzahl(&s.inventar) == 1 && s.zuege == 1);
    s.figur = (Punkt){13, 1};
    PRUEFE(handeln(&s) == 0 && inventar_anzahl(&s.inventar) == 1);   /* voll */
    PRUEFE(s.beute_im_tresor && s.zuege == 1);
    inventar_leeren(&s.inventar);

    /* spiel_starten liest regeln.txt im aktuellen Ordner */
    remove(REGELDATEI);
    PRUEFE(spiel_starten() == 0 && spielstand.regeln.zuglimit == 60);
    schreibe(REGELDATEI, "zuglimit = 40\n");
    PRUEFE(spiel_starten() == 0);
    PRUEFE(spiel_status(zeile, sizeof(zeile)) == 0
           && strcmp(zeile, "Position (3, 7) | Zuege 0/40") == 0);
    schreibe(REGELDATEI, "zuglimit = viele\n");
    PRUEFE(spiel_starten() < 0);
    PRUEFE(text_ausgeben() < 0);
    remove(REGELDATEI);
    aufraeumen();

    remove(TESTDATEI);
    remove(MELDUNGEN);
    if (gesichert) {
        rename(SICHERUNG, REGELDATEI);
    }
    if (fehler == 0) {
        printf("Alle Tests bestanden.\n");
    }
    return fehler == 0 ? 0 : 1;
}
