#include <stdio.h>

int addiere(int a, int b) {
    return a + b;
}

int multipliziere(int a, int b) {
    return a * b;
}

int main(void) {
    int a = 4;
    int b = 7;
    int summe = addiere(a, b);
    int produkt = multipliziere(a, b);

    printf("Summe: %d\n", summe);
    printf("Produkt: %d\n", produkt);

    return 0;
}
