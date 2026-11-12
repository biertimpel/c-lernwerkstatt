# Spielfeld des Tresorraubs

Diese Datei beschreibt den Startzustand des Tresorraubs. Sie enthält nur Daten
und Regeln, keinen Code. Wie Sie die Daten im Programm darstellen, entscheiden
Sie selbst.

## Grundriss

Das Spielfeld ist 15 Felder breit und 9 Felder hoch. `#` ist eine Wand,
`.` ein freies Feld.

```
###############
#.....#.......#
#.##..#..###..#
#.............#
#.##..#.......#
#.....#.......#
#.....#.......#
#.....#.......#
###############
```

## Koordinaten

- (0, 0) ist das Feld links oben.
- x wächst nach rechts (0 bis 14), y wächst nach unten (0 bis 8).
- Eine Zeile des Grundrisses gehört zu einem festen y-Wert.

## Objekte im Startzustand

Alle Objekte stehen auf freien Feldern des Grundrisses.

| Objekt                 | Position (x, y) | Zustand beim Start                     |
|------------------------|-----------------|----------------------------------------|
| Spielfigur             | (3, 7)          | Inventar leer, 0 Züge verbraucht       |
| Ausgang                | (1, 1)          | –                                      |
| Schlüssel              | (3, 5)          | liegt auf dem Feld                     |
| Tür                    | (6, 3)          | verschlossen                           |
| Tresor mit Beute       | (13, 1)         | Beute liegt im Tresor                  |
| Münze                  | (11, 7)         | liegt auf dem Feld                     |
| Wächter 1              | (1, 5)          | Reichweite 1                           |
| Wächter 2              | (9, 5)          | Reichweite 2                           |

Das Zuglimit beträgt 60 Züge. Das Inventar kann Schlüssel, Beute und Münze
enthalten.

## Regel für Überwachungsbereiche

Der Rasterabstand zweier Felder ist die Summe aus waagerechtem und senkrechtem
Abstand. Ein Feld ist überwacht, wenn sein Rasterabstand zu mindestens einem
Wächter höchstens dessen Reichweite beträgt. Das Feld des Wächters gehört dazu.
Wände spielen keine Rolle.

## Zeichen für die Ausgabe

Die Zeichen für Spielfigur, Objekte und überwachte Felder legen Sie selbst fest.
Dokumentieren Sie Ihre Legende in der Projektnotiz.
