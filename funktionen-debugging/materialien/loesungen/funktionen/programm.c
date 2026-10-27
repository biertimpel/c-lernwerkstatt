#include <stdio.h>

const double lieferkosten = 2.50;

double berechne_warenwert(int anzahl, double einzelpreis) {
    return anzahl * einzelpreis;
}

double berechne_lieferpreis(int anzahl, double einzelpreis) {
    double warenwert = berechne_warenwert(anzahl, einzelpreis);
    return warenwert + lieferkosten;
}

double berechne_abholpreis(int anzahl, double einzelpreis) {
    double warenwert = berechne_warenwert(anzahl, einzelpreis);
    return warenwert * 0.90;
}

void gib_preise_aus(double lieferpreis, double abholpreis) {
    printf("Lieferpreis: %.2f Euro\n", lieferpreis);
    printf("Abholpreis: %.2f Euro\n", abholpreis);
}

int main(void) {
    int anzahl = 3;
    double einzelpreis = 9.50;

    double lieferpreis = berechne_lieferpreis(anzahl, einzelpreis);
    double abholpreis = berechne_abholpreis(anzahl, einzelpreis);
    gib_preise_aus(lieferpreis, abholpreis);
    return 0;
}
