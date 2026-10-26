#include <stdio.h>

int main(void) {
    int a = 3;
    double b = 9.50;
    double c = 2.50;
    double d = a * b + c;
    printf("Gesamtpreis: %.2f Euro\n", d);
    return 0;
}
