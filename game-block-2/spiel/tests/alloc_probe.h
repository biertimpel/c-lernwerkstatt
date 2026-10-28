#ifndef ALLOC_PROBE_H
#define ALLOC_PROBE_H
#include <stdlib.h>
void *probe_malloc(size_t n);
void *probe_calloc(size_t n, size_t groesse);
void probe_free(void *p);
void probe_fehler_nach(long n);
long probe_offene_bloecke(void);
#define malloc probe_malloc
#define calloc probe_calloc
#define free probe_free
#endif
