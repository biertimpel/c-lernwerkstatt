#include <stdio.h>

const double lieferkosten = 2.50;

double berechne_warenwert(int anzahl, double einzelpreis) {
    if (anzahl <= 0) {
        return -1;
    }
    if (einzelpreis < 0.0) {
        return -2;
    }
    return anzahl * einzelpreis;
}

double berechne_lieferpreis(int anzahl, double einzelpreis) {
    double warenwert = berechne_warenwert(anzahl, einzelpreis);
    if (warenwert < 0.0) {
        printf("[FEHLER Lieferpreis] Warenwertberechnung meldet Fehlercode %.0f\n", warenwert);
        return warenwert;
    }
    return warenwert + lieferkosten;
}

double berechne_abholpreis(int anzahl, double einzelpreis) {
    double warenwert = berechne_warenwert(anzahl, einzelpreis);
    if (warenwert < 0.0) {
        printf("[FEHLER Abholpreis] Warenwertberechnung meldet Fehlercode %.0f\n", warenwert);
        return warenwert;
    }
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
    if (lieferpreis < 0.0) {
        printf("Abbruch: Lieferpreis konnte nicht berechnet werden (%.0f).\n", lieferpreis);
        return 1;
    }

    double abholpreis = berechne_abholpreis(anzahl, einzelpreis);
    if (abholpreis < 0.0) {
        printf("Abbruch: Abholpreis konnte nicht berechnet werden (%.0f).\n", abholpreis);
        return 1;
    }

    gib_preise_aus(lieferpreis, abholpreis);
    return 0;
}
