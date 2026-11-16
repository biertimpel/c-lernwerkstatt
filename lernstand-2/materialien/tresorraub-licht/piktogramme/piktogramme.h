#ifndef PIKTOGRAMME_H
#define PIKTOGRAMME_H
#include <SDL.h>

/* Alle Motive: 16 x 16 Rasterzellen, skalierung 1 bis 16 Pixel pro Zelle.
   x/y: linke obere Ecke in logischen Pixeln, jeweils -10000 bis 10000.
   Transparente Zellen lassen den Hintergrund unveraendert.
   Erfolg 0, Fehler negativ mit SDL-Fehlermeldung. Renderer nur geliehen. */
int zeichne_spielfigur_froh(SDL_Renderer *renderer, int x, int y, int skalierung);
int zeichne_spielfigur_entdeckt(SDL_Renderer *renderer, int x, int y, int skalierung);
int zeichne_waechter(SDL_Renderer *renderer, int x, int y, int skalierung);
int zeichne_schluessel(SDL_Renderer *renderer, int x, int y, int skalierung);
int zeichne_tuer_geschlossen(SDL_Renderer *renderer, int x, int y, int skalierung);
int zeichne_tuer_offen(SDL_Renderer *renderer, int x, int y, int skalierung);
int zeichne_tresor_geschlossen(SDL_Renderer *renderer, int x, int y, int skalierung);
int zeichne_tresor_leer(SDL_Renderer *renderer, int x, int y, int skalierung);
int zeichne_ausgang(SDL_Renderer *renderer, int x, int y, int skalierung);
int zeichne_muenze(SDL_Renderer *renderer, int x, int y, int skalierung);
int zeichne_wand(SDL_Renderer *renderer, int x, int y, int skalierung);
int zeichne_boden(SDL_Renderer *renderer, int x, int y, int skalierung);
int zeichne_ueberwachung(SDL_Renderer *renderer, int x, int y, int skalierung);
int zeichne_beute(SDL_Renderer *renderer, int x, int y, int skalierung);
int zeichne_lichtschalter(SDL_Renderer *renderer, int x, int y, int skalierung);

/* Stempel "WASTED" fuer ein gescheitertes Spiel: dunkelt das Rechteck ab
   und stempelt den Schriftzug leicht gedreht in seine Mitte. Erwartet
   x, y, breite, hoehe in logischen Pixeln (breite und hoehe groesser 0).
   Braucht SDL 2.0.18 oder neuer (SDL_RenderGeometry). */
int zeichne_wasted(SDL_Renderer *renderer, int x, int y, int breite, int hoehe);

/* Reihenfolge wie in der HTML-Galerie: vier Spalten, zeilenweise. */
int zeichne_galerie(SDL_Renderer *renderer);

#endif
