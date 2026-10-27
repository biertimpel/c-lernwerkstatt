#include <stdio.h>

int main(void) {
    int a = 4;
    int b = a;
    a = 9;
    printf("a: %d\n", a);
    printf("b: %d\n", b);
    b = a + 2;
    printf("b danach: %d\n", b);
    return 0;
}
