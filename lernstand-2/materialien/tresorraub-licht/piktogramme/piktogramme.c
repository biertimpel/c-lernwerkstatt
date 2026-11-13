/* Optionale, frei verwendbare Kursbeispiele. Keine Spiellogik.
   Die Raster und die Hilfsfunktion sind vorbereitete Infrastruktur.
   Fuer diese Lektion muessen Arrays und Pointer intern noch nicht
   verstanden werden. Sichtbare Parameter duerfen selbst gewaehlt werden. */
#include "piktogramme.h"

static const char spielfigur_froh[] =
    "................"
    ".....KKKKKK....."
    "....KYYYYYYK...."
    "....KYYYYYYK...."
    "....KYKYYKYK...."
    "....KYYYYYYK...."
    "....KYKYYKYK...."
    "....KYYKKYYK...."
    ".....KKKKKK....."
    "...KBBBBBBBBK..."
    "...KBBHBBHBBK..."
    "...KBBBBBBBBK..."
    "....KBBKKBBK...."
    "....KBBKKBBK...."
    "....KKK..KKK...."
    "................";

static const char spielfigur_entdeckt[] =
    "................"
    ".....KKKKKK....."
    "....KYYYYYYK...."
    "....KYYYYYYK...."
    "....KYKYYKYK...."
    "....KYYYYYYK...."
    "....KYYKKYYK...."
    "....KYKYYKYK...."
    ".....KKKKKK....."
    "...KBBBBBBBBK..."
    "...KBBHBBHBBK..."
    "...KBBBBBBBBK..."
    "....KBBKKBBK...."
    "....KBBKKBBK...."
    "....KKK..KKK...."
    "................";

static const char waechter[] =
    "................"
    "....KKKKKKKK...."
    "....KRRRRRRK...."
    "...KKKKKKKKKK..."
    "....KRRYYRRK...."
    "....KYKYYKYK...."
    "....KYYYYYYK...."
    "....KYYKKYYK...."
    ".....KKKKKK....."
    "...KPPPPPPPPK..."
    "...KPPPYPPPPK..."
    "...KPPPPPPPPK..."
    "....KPPKKPPK...."
    "....KPPKKPPK...."
    "....KKK..KKK...."
    "................";

static const char schluessel[] =
    "................"
    "................"
    "................"
    "....YYYY........"
    "...YY..YY......."
    "..YY....YYYYYY.."
    "..YY....YYYYYY.."
    "...YY..YY..YY..."
    "....YYYY...YY..."
    "................"
    "................"
    "................"
    "................"
    "................"
    "................"
    "................";

static const char tuer_geschlossen[] =
    "................"
    "..KKKKKKKKKKKK.."
    "..KOOOOOOOOOOK.."
    "..KOOOOOOOOOOK.."
    "..KOOOOOOOOOOK.."
    "..KOOOOOOOOOOK.."
    "..KOOOOOOOOOOK.."
    "..KOOOOOOOOOOK.."
    "..KOOOOOOYOOOK.."
    "..KOOOOOOOOOOK.."
    "..KOOOOOOOOOOK.."
    "..KOOOOOOOOOOK.."
    "..KOOOOOOOOOOK.."
    "..KOOOOOOOOOOK.."
    "..KKKKKKKKKKKK.."
    "................";

static const char tuer_offen[] =
    "................"
    "..KKKKKKKKKKKK.."
    "..KSSSSSSSSSSK.."
    "..KKOOOOOKSSSK.."
    "..KKOOOOOKSSSK.."
    "..KKOOOOOKSSSK.."
    "..KKOOOOOKSSSK.."
    "..KKOOOOOKSSSK.."
    "..KKOOOYOKSSSK.."
    "..KKOOOOOKSSSK.."
    "..KKOOOOOKSSSK.."
    "..KKOOOOOKSSSK.."
    "..KKOOOOOKSSSK.."
    "..KSSSSSSSSSSK.."
    "..KKKKKKKKKKKK.."
    "................";

static const char tresor_geschlossen[] =
    "................"
    "................"
    "..KKKKKKKKKKKK.."
    "..KLLLLLLLLLLK.."
    "..KLSSSSSSSSLK.."
    "..KLSKKKKKKSLK.."
    "..KLSKSSSSKSLK.."
    "..KLSKSKKSKSLK.."
    "..KLSKSKKSKSLK.."
    "..KLSKSSSSKSLK.."
    "..KLSKKKKKKSLK.."
    "..KLSSSSSSSSLK.."
    "..KLLLLLLLLLLK.."
    "..KKKKKKKKKKKK.."
    "................"
    "................";

static const char tresor_leer[] =
    "................"
    "................"
    "..KKKKKKKKKKKK.."
    "..KLLLLLLLLLLK.."
    "..KLKKKKKKKKLK.."
    "..KLK......KLK.."
    "..KLK......KLK.."
    "..KLK......KLK.."
    "..KLK......KLK.."
    "..KLK......KLK.."
    "..KLK......KLK.."
    "..KLKKKKKKKKLK.."
    "..KLLLLLLLLLLK.."
    "..KKKKKKKKKKKK.."
    "................"
    "................";

static const char ausgang[] =
    "................"
    "..KKKKKKKKKKKK.."
    "..KGGGGGGGGGGK.."
    "..KGGGGGGGGGGK.."
    "..KGGGGWGGGGGK.."
    "..KGGGGWWGGGGK.."
    "..KGGGGWWWGGGK.."
    "..KGWWWWWWWGGK.."
    "..KGWWWWWWWWGK.."
    "..KGWWWWWWWGGK.."
    "..KGGGGWWWGGGK.."
    "..KGGGGWWGGGGK.."
    "..KGGGGWGGGGGK.."
    "..KGGGGGGGGGGK.."
    "..KKKKKKKKKKKK.."
    "................";

static const char muenze[] =
    "................"
    "................"
    ".....OOOOOO....."
    "....OYYYYYYO...."
    "...OYYYYYYYYO..."
    "..OYYYYKKYYYYO.."
    "..OYYYKYYKYYYO.."
    "..OYYYKYYYYYYO.."
    "..OYYYKYYYYYYO.."
    "..OYYYKYYKYYYO.."
    "..OYYYYKKYYYYO.."
    "...OYYYYYYYYO..."
    "....OYYYYYYO...."
    ".....OOOOOO....."
    "................"
    "................";

static const char wand[] =
    "KKKKKKKKKKKKKKKK"
    "KLLLLLLKLLLLLLLK"
    "KLLLLLLKLLLLLLLK"
    "KLLLLLLKLLLLLLLK"
    "KKKKKKKKKKKKKKKK"
    "KLLKLLLLLLLKLLLK"
    "KLLKLLLLLLLKLLLK"
    "KLLKLLLLLLLKLLLK"
    "KKKKKKKKKKKKKKKK"
    "KLLLLLLKLLLLLLLK"
    "KLLLLLLKLLLLLLLK"
    "KLLLLLLKLLLLLLLK"
    "KKKKKKKKKKKKKKKK"
    "KLLKLLLLLLLKLLLK"
    "KLLKLLLLLLLKLLLK"
    "KKKKKKKKKKKKKKKK";

static const char boden[] =
    "SSSSSSSSSSSSSSSS"
    "SSSSSSSSSSSSSSSS"
    "SSLSSSSSSSSSSLSS"
    "SSSSSSSSSSSSSSSS"
    "SSSSSSSSSSSSSSSS"
    "SSSSSSSSSSSSSSSS"
    "SSSSSSSLSSSSSSSS"
    "SSSSSSSSSSSSSSSS"
    "SSSSSSSSSSSSSSSS"
    "SSSSSSSSSSSSSSSS"
    "SSSSLSSSSSSSSSSS"
    "SSSSSSSSSSSSSSSS"
    "SSSSSSSSSSSSSSSS"
    "SSSSSSSSSSSSLSSS"
    "SSSSSSSSSSSSSSSS"
    "SSSSSSSSSSSSSSSS";

static const char ueberwachung[] =
    "KKKKKKKKKKKKKKKK"
    "KRRSSRRSSRRSSRRK"
    "KSSRRSSRRSSRRSSK"
    "KSRRSSRRSSRRSSRK"
    "KRRSSRRSSRRSSRRK"
    "KSSRRSSKKSSRRSSK"
    "KSRRSSRKKSRRSSRK"
    "KRRSSRRKKRRSSRRK"
    "KSSRRSSKKSSRRSSK"
    "KSRRSSRKKSRRSSRK"
    "KRRSSRRSSRRSSRRK"
    "KSSRRSSKKSSRRSSK"
    "KSRRSSRKKSRRSSRK"
    "KRRSSRRSSRRSSRRK"
    "KSSRRSSRRSSRRSSK"
    "KKKKKKKKKKKKKKKK";

static const char beute[] =
    "................"
    "................"
    "................"
    "................"
    "......YYYY......"
    ".....YYYYYY....."
    "....OOOOOOOO...."
    "................"
    "..YYYY....YYYY.."
    ".YYYYYY..YYYYYY."
    "OOOOOOOOOOOOOOOO"
    "................"
    "................"
    "................"
    "................"
    "................";

static const char lichtschalter[] =
    "................"
    "................"
    "....KKKKKKKK...."
    "....KSSSSSSK...."
    "....KSSSSSSK...."
    "....KSSKKSSK...."
    "....KSKYYKSK...."
    "....KSKYYKSK...."
    "....KSKLLKSK...."
    "....KSKLLKSK...."
    "....KSSKKSSK...."
    "....KSSSSSSK...."
    "....KSSSSSSK...."
    "....KKKKKKKK...."
    "................"
    "................";

/* Farben: K dunkel, W weiss, B blau, H hellblau, Y gelb, O braun,
   R rot, G gruen, P violett, L grau, S hellgrau, . transparent.
   Die Farbwerte duerfen fuer eigene Versuche geaendert werden.
   Recherche-Beispiel: SDL_SetRenderDrawColor und SDL_RenderDrawLine. */
static int farbe_setzen(SDL_Renderer *renderer, char farbe) {
    switch (farbe) {
        case 'K': return SDL_SetRenderDrawColor(renderer, 23, 37, 59, 255);
        case 'W': return SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        case 'B': return SDL_SetRenderDrawColor(renderer, 36, 75, 204, 255);
        case 'H': return SDL_SetRenderDrawColor(renderer, 190, 216, 255, 255);
        case 'Y': return SDL_SetRenderDrawColor(renderer, 242, 189, 57, 255);
        case 'O': return SDL_SetRenderDrawColor(renderer, 148, 96, 57, 255);
        case 'R': return SDL_SetRenderDrawColor(renderer, 194, 60, 75, 255);
        case 'G': return SDL_SetRenderDrawColor(renderer, 37, 128, 93, 255);
        case 'P': return SDL_SetRenderDrawColor(renderer, 115, 86, 168, 255);
        case 'L': return SDL_SetRenderDrawColor(renderer, 116, 129, 150, 255);
        case 'S': return SDL_SetRenderDrawColor(renderer, 220, 226, 235, 255);
        default: return SDL_SetError("Unbekanntes Farbzeichen im Piktogramm.");
    }
}

static int zeichne_muster(SDL_Renderer *renderer, const char *muster,
                         int x, int y, int skalierung) {
    if (renderer == NULL || skalierung < 1 || skalierung > 16
        || x < -10000 || x > 10000 || y < -10000 || y > 10000) {
        return SDL_SetError("Ungueltige Piktogramm-Parameter.");
    }
    for (int zeile = 0; zeile < 16; zeile++) {
        for (int spalte = 0; spalte < 16; spalte++) {
            char farbe = muster[zeile * 16 + spalte];
            if (farbe != '.') {
                if (farbe_setzen(renderer, farbe) < 0) {
                    return -1;
                }
                for (int pixelzeile = 0; pixelzeile < skalierung; pixelzeile++) {
                    int links = x + spalte * skalierung;
                    int oben = y + zeile * skalierung + pixelzeile;
                    if (SDL_RenderDrawLine(renderer, links, oben,
                                           links + skalierung - 1, oben) < 0) {
                        return -1;
                    }
                }
            }
        }
    }
    return 0;
}

int zeichne_spielfigur_froh(SDL_Renderer *r, int x, int y, int s) {
    return zeichne_muster(r, spielfigur_froh, x, y, s);
}
int zeichne_spielfigur_entdeckt(SDL_Renderer *r, int x, int y, int s) {
    return zeichne_muster(r, spielfigur_entdeckt, x, y, s);
}
int zeichne_waechter(SDL_Renderer *r, int x, int y, int s) {
    return zeichne_muster(r, waechter, x, y, s);
}
int zeichne_schluessel(SDL_Renderer *r, int x, int y, int s) {
    return zeichne_muster(r, schluessel, x, y, s);
}
int zeichne_tuer_geschlossen(SDL_Renderer *r, int x, int y, int s) {
    return zeichne_muster(r, tuer_geschlossen, x, y, s);
}
int zeichne_tuer_offen(SDL_Renderer *r, int x, int y, int s) {
    return zeichne_muster(r, tuer_offen, x, y, s);
}
int zeichne_tresor_geschlossen(SDL_Renderer *r, int x, int y, int s) {
    return zeichne_muster(r, tresor_geschlossen, x, y, s);
}
int zeichne_tresor_leer(SDL_Renderer *r, int x, int y, int s) {
    return zeichne_muster(r, tresor_leer, x, y, s);
}
int zeichne_ausgang(SDL_Renderer *r, int x, int y, int s) {
    return zeichne_muster(r, ausgang, x, y, s);
}
int zeichne_muenze(SDL_Renderer *r, int x, int y, int s) {
    return zeichne_muster(r, muenze, x, y, s);
}
int zeichne_wand(SDL_Renderer *r, int x, int y, int s) {
    return zeichne_muster(r, wand, x, y, s);
}
int zeichne_boden(SDL_Renderer *r, int x, int y, int s) {
    return zeichne_muster(r, boden, x, y, s);
}
int zeichne_ueberwachung(SDL_Renderer *r, int x, int y, int s) {
    return zeichne_muster(r, ueberwachung, x, y, s);
}

int zeichne_beute(SDL_Renderer *r, int x, int y, int s) {
    return zeichne_muster(r, beute, x, y, s);
}
int zeichne_lichtschalter(SDL_Renderer *r, int x, int y, int s) {
    return zeichne_muster(r, lichtschalter, x, y, s);
}

int zeichne_galerie(SDL_Renderer *renderer) {
    if (zeichne_spielfigur_froh(renderer, 48, 32, 4) < 0
        || zeichne_spielfigur_entdeckt(renderer, 192, 32, 4) < 0
        || zeichne_waechter(renderer, 336, 32, 4) < 0
        || zeichne_schluessel(renderer, 480, 32, 4) < 0
        || zeichne_tuer_geschlossen(renderer, 48, 144, 4) < 0
        || zeichne_tuer_offen(renderer, 192, 144, 4) < 0
        || zeichne_tresor_geschlossen(renderer, 336, 144, 4) < 0
        || zeichne_tresor_leer(renderer, 480, 144, 4) < 0
        || zeichne_ausgang(renderer, 48, 256, 4) < 0
        || zeichne_muenze(renderer, 192, 256, 4) < 0
        || zeichne_wand(renderer, 336, 256, 4) < 0
        || zeichne_boden(renderer, 480, 256, 4) < 0
        || zeichne_ueberwachung(renderer, 48, 368, 4) < 0
        || zeichne_beute(renderer, 192, 368, 4) < 0
        || zeichne_lichtschalter(renderer, 336, 368, 4) < 0) {
        return -1;
    }
    return 0;
}

/* ---------- Stempel "WASTED" ---------- */

/* Buchstaben im Raster 5 x 7. */
static const char *const STEMPEL_TEXT[6][7] = {
    {"X...X", "X...X", "X...X", "X.X.X", "X.X.X", "XX.XX", "X...X"},   /* W */
    {".XXX.", "X...X", "X...X", "XXXXX", "X...X", "X...X", "X...X"},   /* A */
    {".XXXX", "X....", "X....", ".XXX.", "....X", "....X", "XXXX."},   /* S */
    {"XXXXX", "..X..", "..X..", "..X..", "..X..", "..X..", "..X.."},   /* T */
    {"XXXXX", "X....", "X....", "XXXX.", "X....", "X....", "XXXXX"},   /* E */
    {"XXXX.", "X...X", "X...X", "X...X", "X...X", "X...X", "XXXX."}    /* D */
};

#define STEMPEL_SPALTEN 35       /* 6 Buchstaben zu 5, dazwischen je 1 frei */
#define STEMPEL_WINKEL (-0.21f)  /* etwa -12 Grad, im Bogenmass */

typedef struct {
    float mx, my;               /* Drehpunkt */
    float cos_w, sin_w;
} Drehung;

/* Zeichnet ein um den Drehpunkt gedrehtes Rechteck. Koordinaten relativ
   zum Drehpunkt, ungedreht. */
static int gedrehtes_rechteck(SDL_Renderer *renderer, const Drehung *d,
                              float x, float y, float b, float h, SDL_Color farbe) {
    const float ecken[4][2] = {{x, y}, {x + b, y}, {x + b, y + h}, {x, y + h}};
    SDL_Vertex v[4];
    for (int i = 0; i < 4; i++) {
        float ex = ecken[i][0];
        float ey = ecken[i][1];
        v[i].position.x = d->mx + ex * d->cos_w - ey * d->sin_w;
        v[i].position.y = d->my + ex * d->sin_w + ey * d->cos_w;
        v[i].color = farbe;
        v[i].tex_coord.x = 0.0f;
        v[i].tex_coord.y = 0.0f;
    }
    static const int dreiecke[6] = {0, 1, 2, 0, 2, 3};
    return SDL_RenderGeometry(renderer, NULL, v, 4, dreiecke, 6);
}

int zeichne_wasted(SDL_Renderer *renderer, int x, int y, int breite, int hoehe) {
    if (renderer == NULL || breite <= 0 || hoehe <= 0) {
        return SDL_SetError("zeichne_wasted: ungueltige Parameter");
    }
    SDL_BlendMode vorher;
    if (SDL_GetRenderDrawBlendMode(renderer, &vorher) < 0
        || SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND) < 0) {
        return -1;
    }
    int ergebnis = 0;
    /* Das Spielfeld abdunkeln */
    SDL_Rect flaeche = {x, y, breite, hoehe};
    if (SDL_SetRenderDrawColor(renderer, 0, 0, 0, 110) < 0
        || SDL_RenderFillRect(renderer, &flaeche) < 0) {
        ergebnis = -1;
    }
    /* Gedrehter Stempel in der Mitte */
    float zelle = (float)breite * 0.7f / STEMPEL_SPALTEN;
    float text_b = zelle * STEMPEL_SPALTEN;
    float text_h = zelle * 7;
    float rand = zelle * 1.5f;
    float linie = zelle * 0.6f;
    Drehung d = {x + breite / 2.0f, y + hoehe / 2.0f,
                 SDL_cosf(STEMPEL_WINKEL), SDL_sinf(STEMPEL_WINKEL)};
    const SDL_Color band = {0, 0, 0, 170};
    const SDL_Color rot = {205, 20, 30, 235};
    float bx = -text_b / 2 - rand;
    float by = -text_h / 2 - rand;
    float bb = text_b + 2 * rand;
    float bh = text_h + 2 * rand;
    if (ergebnis == 0) {
        ergebnis = gedrehtes_rechteck(renderer, &d, bx, by, bb, bh, band);
    }
    /* Rahmen des Stempels */
    if (ergebnis == 0) {
        ergebnis = gedrehtes_rechteck(renderer, &d, bx, by, bb, linie, rot);
    }
    if (ergebnis == 0) {
        ergebnis = gedrehtes_rechteck(renderer, &d, bx, by + bh - linie, bb, linie, rot);
    }
    if (ergebnis == 0) {
        ergebnis = gedrehtes_rechteck(renderer, &d, bx, by, linie, bh, rot);
    }
    if (ergebnis == 0) {
        ergebnis = gedrehtes_rechteck(renderer, &d, bx + bb - linie, by, linie, bh, rot);
    }
    /* Buchstaben */
    for (int b = 0; ergebnis == 0 && b < 6; b++) {
        for (int zeile = 0; ergebnis == 0 && zeile < 7; zeile++) {
            for (int spalte = 0; ergebnis == 0 && spalte < 5; spalte++) {
                if (STEMPEL_TEXT[b][zeile][spalte] != 'X') {
                    continue;
                }
                float px = -text_b / 2 + (float)(b * 6 + spalte) * zelle;
                float py = -text_h / 2 + (float)zeile * zelle;
                ergebnis = gedrehtes_rechteck(renderer, &d, px, py, zelle, zelle, rot);
            }
        }
    }
    if (SDL_SetRenderDrawBlendMode(renderer, vorher) < 0) {
        ergebnis = -1;
    }
    return ergebnis < 0 ? -1 : 0;
}
