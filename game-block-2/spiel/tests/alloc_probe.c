#include "alloc_probe.h"
#undef malloc
#undef calloc
#undef free
static long verbleibend = -1;
static long offen = 0;
void probe_fehler_nach(long n) { verbleibend = n; }
long probe_offene_bloecke(void) { return offen; }
static int erlauben(void) {
    if (verbleibend == 0) return 0;
    if (verbleibend > 0) verbleibend--;
    return 1;
}
void *probe_malloc(size_t n) {
    if (!erlauben()) return NULL;
    void *p = malloc(n);
    if (p) offen++;
    return p;
}
void *probe_calloc(size_t n, size_t groesse) {
    if (!erlauben()) return NULL;
    void *p = calloc(n, groesse);
    if (p) offen++;
    return p;
}
void probe_free(void *p) {
    if (p) offen--;
    free(p);
}
