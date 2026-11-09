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
        || zeichne_beute(renderer, 192, 368, 4) < 0) {
        return -1;
    }
    return 0;
}
