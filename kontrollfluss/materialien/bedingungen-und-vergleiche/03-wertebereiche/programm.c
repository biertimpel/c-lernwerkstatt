#include <stdbool.h>
#include <stdio.h>

int main(void) {
    int temperatur = 21;

    /* TODO: Ersetzen Sie false durch passende Bedingungen. */
    bool angenehm = false;   /* Von 18 bis 24 einschliesslich. */
    bool ausserhalb = false; /* Ausserhalb dieses Bereichs. */

    printf("Temperatur: %d\n", temperatur);
    printf("Angenehm: %d\n", angenehm);
    printf("Ausserhalb: %d\n", ausserhalb);
    return 0;
}
