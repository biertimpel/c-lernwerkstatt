#include <stdbool.h>
#include <stdio.h>

int main(void) {
    /* Verwenden Sie kleine, nichtnegative Werte. */
    int zahl = 10;
    int teiler = 0;

    /* TODO: Teiler ungleich 0 UND ganzzahliger Quotient groesser als 2.
       Der Schutz vor Division durch 0 muss links stehen. */
    bool quotient_gross = false;

    printf("Zahl: %d, Teiler: %d\n", zahl, teiler);
    printf("Teiler ungleich 0 und Quotient groesser als 2: %d\n",
           quotient_gross);
    return 0;
}
