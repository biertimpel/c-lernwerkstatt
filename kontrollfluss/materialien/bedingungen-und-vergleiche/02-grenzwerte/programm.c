#include <stdbool.h>
#include <stdio.h>

int main(void) {
    int alter = 17;
    int menge = 9;

    /* TODO: Ersetzen Sie false jeweils durch einen passenden Vergleich. */
    bool volljaehrig = false;   /* Ab 18 Jahren. */
    bool menge_erlaubt = false; /* Hoechstens 10 Stueck. */

    printf("Alter: %d, volljaehrig: %d\n", alter, volljaehrig);
    printf("Menge: %d, Menge erlaubt: %d\n", menge, menge_erlaubt);
    return 0;
}
