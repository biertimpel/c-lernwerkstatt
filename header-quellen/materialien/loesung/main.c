#include <stdio.h>
#include "calc.h"

int main(void) {
    int a = 4;
    int b = 7;
    int summe = addiere(a, b);
    int produkt = multipliziere(a, b);

    printf("Summe: %d\n", summe);
    printf("Produkt: %d\n", produkt);

    return 0;
}
