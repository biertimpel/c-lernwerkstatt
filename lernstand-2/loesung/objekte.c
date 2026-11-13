/* Objektarten mit ihren Methoden, Startzustand und Hilfsfunktionen. */
#include "objekte.h"
#include "inventar.h"
#include "spiellogik.h"
#include "piktogramme/piktogramme.h"
#include <stdio.h>

/* ---------- Objektarten und ihre Methoden ---------- */

/* Nimmt einen Gegenstand ins Inventar auf und meldet das Ergebnis. Bei
   AUFGENOMMEN liegt er nicht mehr auf dem Feld, und ein Zug ist gezaehlt. */
static Aufnahme gegenstand_aufnehmen(Objekt *gegenstand, Spielstand *s) {
    const char *name = gegenstand->art->name;
    Aufnahme ergebnis = inventar_aufnehmen(&s->inventar, gegenstand,
                                           (size_t)s->regeln.inventar_groesse);
    if (ergebnis == FEHLER_SPEICHER) {
        printf("Fehler: %s konnte nicht aufgenommen werden.\n", name);
    } else if (ergebnis == INVENTAR_VOLL) {
        printf("Inventar ist voll, %s bleibt liegen.\n", name);
    } else {
        gegenstand->auf_feld = false;
        zug_zaehlen(s);
        printf("%s aufgenommen.\n", name);
    }
    return ergebnis;
}

/* Gegenstaende: aufnehmen, wenn die Figur auf ihrem Feld steht. */
static Benutzung gegenstand_benutzen(Objekt *selbst, Spielstand *s, int abstand) {
    if (abstand != 0) {
        return NICHTS_ZU_TUN;
    }
    return gegenstand_aufnehmen(selbst, s) == FEHLER_SPEICHER ? BENUTZEN_FEHLER : ERLEDIGT;
}

static const Objektart ART_SCHLUESSEL = {
    "Schluessel", true, 'k', 'k', zeichne_schluessel, zeichne_schluessel,
    gegenstand_benutzen, NULL, NULL
};
static const Objektart ART_MUENZE = {
    "Muenze", true, 'c', 'c', zeichne_muenze, zeichne_muenze,
    gegenstand_benutzen, NULL, NULL
};
static const Objektart ART_BEUTE = {
    "Beute", true, 'b', 'b', zeichne_beute, zeichne_beute,
    gegenstand_benutzen, NULL, NULL
};

/* Tuer: vom Nachbarfeld aus mit dem Schluessel oeffnen; der Schluessel
   wird verbraucht. */
static Benutzung tuer_benutzen(Objekt *selbst, Spielstand *s, int abstand) {
    if (abstand != 1 || selbst->offen) {
        return NICHTS_ZU_TUN;
    }
    Objekt *schluessel = inventar_finden(&s->inventar, &ART_SCHLUESSEL);
    if (schluessel == NULL) {
        printf("Ohne Schluessel bleibt die Tuer zu.\n");
        return ERLEDIGT;
    }
    inventar_entfernen(&s->inventar, schluessel);
    selbst->offen = true;
    zug_zaehlen(s);
    printf("Tuer geoeffnet, Schluessel verbraucht.\n");
    return ERLEDIGT;
}

/* Tueren aller Arten: nur offen betretbar. */
static bool tuer_betretbar(const Objekt *selbst) {
    return selbst->offen;
}

/* Tresor: Steht die Figur darauf, wird der Inhalt aufgenommen. */
static Benutzung tresor_benutzen(Objekt *selbst, Spielstand *s, int abstand) {
    if (abstand != 0 || selbst->inhalt == NULL) {
        return NICHTS_ZU_TUN;
    }
    Aufnahme ergebnis = gegenstand_aufnehmen(selbst->inhalt, s);
    if (ergebnis == FEHLER_SPEICHER) {
        return BENUTZEN_FEHLER;
    }
    if (ergebnis == AUFGENOMMEN) {
        selbst->inhalt = NULL;
        selbst->offen = true;
    }
    return ERLEDIGT;
}

/* Geheimtuer: sieht geschlossen wie eine Wand aus und geht vom
   Nachbarfeld aus ohne Schluessel auf. */
static Benutzung geheimtuer_benutzen(Objekt *selbst, Spielstand *s, int abstand) {
    if (abstand != 1 || selbst->offen) {
        return NICHTS_ZU_TUN;
    }
    selbst->offen = true;
    zug_zaehlen(s);
    printf("Geheimtuer geoeffnet.\n");
    return ERLEDIGT;
}

/* Lichtschalter: von einem Nachbarfeld aus mit der Leertaste. Schaltet
   das Licht aus; ist es schon aus, beginnt die Dauer von vorn. */
static Benutzung lichtschalter_benutzen(Objekt *selbst, Spielstand *s, int abstand) {
    (void)selbst;
    if (abstand != 1) {
        return NICHTS_ZU_TUN;
    }
    s->licht_an = false;
    s->licht_aus_seit_ms = s->zeit_ms;
    printf("Licht aus.\n");
    zug_zaehlen(s);
    return ERLEDIGT;
}

/* Auf das Feld des Lichtschalters kann die Figur nicht. */
static bool nie_betretbar(const Objekt *selbst) {
    (void)selbst;
    return false;
}

/* Ausgang: Wer ihn mit der Beute betritt, hat gewonnen. */
static void ausgang_betreten(Objekt *selbst, Spielstand *s) {
    (void)selbst;
    if (inventar_finden(&s->inventar, &ART_BEUTE) != NULL) {
        s->zustand = GEWONNEN;
        printf("Mit der Beute entkommen!\n");
    } else {
        printf("Ohne Beute geht es noch nicht hinaus.\n");
    }
}

static const Objektart ART_TUER = {
    "Tuer", false, 'D', '/', zeichne_tuer_geschlossen, zeichne_tuer_offen,
    tuer_benutzen, tuer_betretbar, NULL
};
static const Objektart ART_TRESOR = {
    "Tresor", false, 'T', 't', zeichne_tresor_geschlossen, zeichne_tresor_leer,
    tresor_benutzen, NULL, NULL
};
static const Objektart ART_AUSGANG = {
    "Ausgang", false, 'E', 'E', zeichne_ausgang, zeichne_ausgang, NULL, NULL,
    ausgang_betreten
};
static const Objektart ART_GEHEIMTUER = {
    "Geheimtuer", false, '#', '/', zeichne_wand, zeichne_tuer_offen,
    geheimtuer_benutzen, tuer_betretbar, NULL
};
static const Objektart ART_LICHTSCHALTER = {
    "Lichtschalter", false, 'S', 'S', zeichne_lichtschalter, zeichne_lichtschalter,
    lichtschalter_benutzen, nie_betretbar, NULL
};

/* ---------- Startzustand ---------- */

/* Haengt ein Objekt an und liefert einen Pointer darauf. */
static Objekt *objekt_hinzufuegen(Spielstand *s, const Objektart *art,
                                  int x, int y, bool auf_feld) {
    Objekt *o = &s->objekte[s->anzahl_objekte++];
    *o = (Objekt){art, {x, y}, auf_feld, false, NULL};
    return o;
}

/* Fuellt *s mit dem Startzustand. Die Reihenfolge der Objekte legt fest,
   welches Zeichen gewinnt, wenn mehrere auf einem Feld stehen. */
void startzustand(Spielstand *s, const Regeln *regeln) {
    *s = (Spielstand){
        .figur = {3, 7},
        .licht_an = true,
        .regeln = *regeln,
        .waechter = {{regeln->waechter[0], regeln->reichweite[0]},
                     {regeln->waechter[1], regeln->reichweite[1]}}
    };
    objekt_hinzufuegen(s, &ART_TUER, 6, 3, true);
    Objekt *tresor = objekt_hinzufuegen(s, &ART_TRESOR, 13, 1, true);
    objekt_hinzufuegen(s, &ART_SCHLUESSEL, regeln->schluessel.x, regeln->schluessel.y, true);
    objekt_hinzufuegen(s, &ART_MUENZE, 11, 7, true);
    tresor->inhalt = objekt_hinzufuegen(s, &ART_BEUTE, 13, 1, false);
    objekt_hinzufuegen(s, &ART_AUSGANG, 1, 1, true);
    objekt_hinzufuegen(s, &ART_GEHEIMTUER, 6, 6, true);
    objekt_hinzufuegen(s, &ART_MUENZE, 12, 7, true);
    objekt_hinzufuegen(s, &ART_LICHTSCHALTER, regeln->lichtschalter.x,
                       regeln->lichtschalter.y, true);
}

/* Meldet eine Stelle, an der nichts stehen darf. */
static int aufstellung_fehler(const char *was, Punkt p, const char *grund) {
    fprintf(stderr, "%s: %s auf (%d, %d) %s.\n", REGELDATEI, was, p.x, p.y, grund);
    return -1;
}

int aufstellung_pruefen(const Regeln *regeln) {
    Spielstand s;
    startzustand(&s, regeln);
    /* Alles, was ein eigenes Feld braucht: Figur, Waechter, Objekte */
    Punkt stelle[1 + ANZAHL_WAECHTER + MAX_OBJEKTE];
    const char *name[1 + ANZAHL_WAECHTER + MAX_OBJEKTE];
    size_t anzahl = 0;
    stelle[anzahl] = s.figur;
    name[anzahl++] = "Startfeld der Figur";
    for (size_t i = 0; i < ANZAHL_WAECHTER; i++) {
        stelle[anzahl] = s.waechter[i].position;
        name[anzahl++] = "Waechter";
    }
    for (size_t i = 0; i < s.anzahl_objekte; i++) {
        if (s.objekte[i].auf_feld) {
            stelle[anzahl] = s.objekte[i].position;
            name[anzahl++] = s.objekte[i].art->name;
        }
    }
    for (size_t i = 0; i < anzahl; i++) {
        if (!begehbar(stelle[i])) {
            return aufstellung_fehler(name[i], stelle[i], "ist kein Boden");
        }
        for (size_t j = 0; j < i; j++) {
            if (punkt_gleich(stelle[i], stelle[j])) {
                fprintf(stderr, "%s: %s und %s stehen beide auf (%d, %d).\n",
                        REGELDATEI, name[j], name[i], stelle[i].x, stelle[i].y);
                return -1;
            }
        }
    }
    if (ueberwacht(s.figur, s.waechter, ANZAHL_WAECHTER)) {
        return aufstellung_fehler("Startfeld der Figur", s.figur, "ist ueberwacht");
    }
    return 0;
}

/* ---------- Hilfsfunktionen ---------- */

/* Erstes Objekt auf dem Feld p, sonst NULL. */
const Objekt *objekt_auf(const Spielstand *s, Punkt p) {
    for (size_t i = 0; i < s->anzahl_objekte; i++) {
        const Objekt *o = &s->objekte[i];
        if (o->auf_feld && punkt_gleich(o->position, p)) {
            return o;
        }
    }
    return NULL;
}

/* Steht auf p ein Waechter? */
bool waechter_auf(const Spielstand *s, Punkt p) {
    for (size_t i = 0; i < ANZAHL_WAECHTER; i++) {
        if (punkt_gleich(p, s->waechter[i].position)) {
            return true;
        }
    }
    return false;
}

/* Gegenstand, den die Figur auf p aufnehmen koennte, sonst NULL. Der
   Inhalt eines Tresors zaehlt dazu. */
const Objekt *gegenstand_auf(const Spielstand *s, Punkt p) {
    const Objekt *o = objekt_auf(s, p);
    if (o == NULL) {
        return NULL;
    }
    if (o->art->aufnehmbar) {
        return o;
    }
    return o->inhalt;
}

/* Zeichen und Piktogramm eines Objekts, je nach Zustand. */
char objekt_zeichen(const Objekt *o) {
    return o->offen ? o->art->zeichen_offen : o->art->zeichen;
}

Piktogramm objekt_bild(const Objekt *o) {
    return o->offen ? o->art->bild_offen : o->art->bild;
}
