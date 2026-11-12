# Neue Objekte für Seite 5

Diese Datei beschreibt zwei Ergänzungen des Tresorraubs für die Aufgabe
„Eine neue Objektart“. Sie enthält keinen Code. Alle übrigen Regeln aus
`SPIELFELD.md` und `INVENTAR.md` gelten weiter.

## Geheimtür

| Eigenschaft        | Wert                                                    |
|--------------------|---------------------------------------------------------|
| Position           | (6, 6). Im Grundriss wird dieses Feld zu Boden (`.`).   |
| Zustand beim Start | geschlossen                                             |
| Aussehen geschlossen | wie eine Wand: Textausgabe `#`, Piktogramm `zeichne_wand` |
| Aussehen offen     | wie die offene Tür: `/`, `zeichne_tuer_offen`           |

- Geschlossen kann die Figur das Feld nicht betreten.
- Leertaste auf einem direkten Nachbarfeld, also auf (5, 6) oder (7, 6):
  Die Geheimtür öffnet sich, **ohne** Schlüssel. Das zählt als ein Zug.
  Meldung im Terminal: `Geheimtuer geoeffnet.`
- Offen ist das Feld betretbar. Die Leertaste bewirkt dann an der
  Geheimtür nichts mehr.
- Auf das Feld der Geheimtür kann nichts abgelegt werden, auch wenn sie
  offen ist.
- Liegt auf dem Feld der Figur ein Gegenstand, wird wie bisher zuerst er
  aufgenommen.

## Zweite Münze

| Eigenschaft        | Wert                    |
|--------------------|-------------------------|
| Position           | (12, 7)                 |
| Zustand beim Start | liegt auf dem Feld      |

Sie verhält sich genau wie die erste Münze und heißt im Programm ebenfalls
`Muenze`. Beide Münzen können gleichzeitig im Inventar sein; die Anzeige
lautet dann zum Beispiel `Inventar (2): Muenze Muenze`.
