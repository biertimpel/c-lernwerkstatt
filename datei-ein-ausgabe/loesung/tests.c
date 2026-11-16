#include <SDL.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
static int fail_after = -1, live = 0, files_open = 0, close_fail = 0;
static void *pr_malloc(size_t size) {
    if (fail_after == 0) return NULL;
    if (fail_after > 0) fail_after--;
    void *p = malloc(size); if (p) live++; return p;
}
static void pr_free(void *p) { if (p) live--; free(p); }
static FILE *pr_fopen(const char *p, const char *m) {
    FILE *f = fopen(p,m); if (f) files_open++; return f;
}
static int pr_fclose(FILE *f) {
    int result = fclose(f); files_open--;
    if (close_fail) {close_fail = 0; return EOF;} return result;
}
#define malloc pr_malloc
#define free pr_free
#define fopen pr_fopen
#define fclose pr_fclose
#include "tresorraub.c"
#undef malloc
#undef free
#undef fopen
#undef fclose
static int checks = 0;
#define CHECK(c) do {checks++; if (!(c)) {fprintf(stderr,"Fehler in Zeile %d: %s\n",__LINE__,#c); exit(1);}} while(0)
static void pfad(char *out, size_t size, const char *name) {
    int n=snprintf(out,size,"../materialien/tresorraub-dateien/beispiele/%s",name);
    CHECK(n>=0 && (size_t)n<size);
}
static int laden(Spielstand *s, const char *name) {
    char p[256];pfad(p,sizeof(p),name);return spielstand_laden(s,p);
}
int main(void) {
    Spielstand a=startzustand(), b=startzustand();
    CHECK(laden(&a,"start.txt")==0);
    CHECK(a.figur.x==3 && a.figur.y==7 && a.zuege==0 && a.inventar.erster==NULL);
    CHECK(laden(&a,"inventar.txt")==0);
    CHECK(a.zuege==23 && a.tuer_offen);
    CHECK(strcmp(a.inventar.erster->name,"Beute")==0);
    CHECK(strcmp(a.inventar.erster->naechster->name,"Muenze")==0);
    FILE *tmp=tmpfile(); CHECK(tmp!=NULL);
    /* tmpnam wird nicht verwendet: Der Testpfad liegt im jeweiligen Testordner. */
    CHECK(fclose(tmp)==0);
    CHECK(spielstand_speichern(&a,"qs-spielstand.txt")==0);
    CHECK(spielstand_laden(&b,"qs-spielstand.txt")==0);
    CHECK(b.inventar.erster != a.inventar.erster);
    CHECK(strcmp(b.inventar.erster->name,"Beute")==0);
    CHECK(strcmp(b.inventar.erster->naechster->name,"Muenze")==0);
    CHECK(b.zuege==a.zuege && b.tuer_offen==a.tuer_offen);
    const char *bad[]={"ungueltig-position.txt","ungueltig-zuege.txt","ungueltig-flag.txt",
        "ungueltig-unvollstaendig.txt","ungueltig-name.txt","ungueltig-doppelt.txt",
        "ungueltig-widerspruch.txt","ungueltig-ueberlauf.txt","ungueltig-anzahl.txt",
        "ungueltig-version.txt","ungueltig-rest.txt","nicht-vorhanden.txt"};
    for(size_t i=0;i<sizeof(bad)/sizeof(bad[0]);i++) {
        Spielstand vorher=b; int vorher_live=live;
        CHECK(laden(&b,bad[i])<0);
        CHECK(memcmp(&vorher,&b,sizeof(b))==0 && live==vorher_live && files_open==0);
    }
    for(int i=0;i<2;i++) {
        Spielstand vorher=b; int vorher_live=live; fail_after=i;
        CHECK(laden(&b,"inventar.txt")==-3); fail_after=-1;
        CHECK(memcmp(&vorher,&b,sizeof(b))==0 && live==vorher_live && files_open==0);
    }
    {Spielstand vorher=b;int vorher_live=live;close_fail=1;
     CHECK(laden(&b,"inventar.txt")<0);
     CHECK(memcmp(&vorher,&b,sizeof(b))==0 && live==vorher_live && files_open==0);}
    close_fail=1;CHECK(spielstand_speichern(&b,"qs-spielstand.txt")<0);CHECK(files_open==0);
#ifdef __linux__
    CHECK(spielstand_speichern(&b,"/dev/full")<0);CHECK(files_open==0);
#endif
    CHECK(spielstand_speichern(&b,"nicht-existenter-ordner/datei.txt")<0);
    CHECK(laden(&b,"abgelegt.txt")==0);
    CHECK(b.inventar.erster==NULL && b.zuege==24 && b.beute_liegt && b.muenze_liegt);
    CHECK(b.beute.x==5 && b.beute.y==2 && b.muenze.x==5 && b.muenze.y==4);
    CHECK(!b.schluessel_liegt && !b.beute_im_tresor);
    inventar_leeren(&a.inventar);inventar_leeren(&b.inventar);
    CHECK(SDL_Init(SDL_INIT_VIDEO)==0);
    SDL_Window *win=SDL_CreateWindow("QS",0,0,640,480,SDL_WINDOW_HIDDEN);
    CHECK(win!=NULL);SDL_Renderer *ren=SDL_CreateRenderer(win,-1,SDL_RENDERER_SOFTWARE);CHECK(ren!=NULL);
    CHECK(spiel_starten()==0);CHECK(taste_gedrueckt(SDLK_RIGHT)==0);
    CHECK(taste_gedrueckt(SDLK_F5)==0);CHECK(taste_gedrueckt(SDLK_RIGHT)==0);
    CHECK(spielstand.figur.x==5);CHECK(taste_gedrueckt(SDLK_F9)==0);
    CHECK(spielstand.figur.x==4 && spielstand.zuege==1);
    CHECK(zeichne(ren)==0);
    char status[128];CHECK(spiel_status(status,sizeof(status))==0);
    CHECK(strcmp(status,"Position (4, 7) | Zuege 1/60")==0);
    CHECK(remove("spielstand.txt")==0);CHECK(taste_gedrueckt(SDLK_F9)==0);
    CHECK(spielstand.figur.x==4 && spielstand.zuege==1);
    aufraeumen();SDL_DestroyRenderer(ren);SDL_DestroyWindow(win);SDL_Quit();
    CHECK(live==0 && files_open==0);CHECK(remove("qs-spielstand.txt")==0);
    printf("%d Pruefungen bestanden: Formate, Reihenfolge, Ablegen, Fehler, Speicherbesitz, Geruest und Bild.\n",checks);
    return 0;
}
