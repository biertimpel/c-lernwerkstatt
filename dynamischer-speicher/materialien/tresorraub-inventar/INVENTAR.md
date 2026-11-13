# Inventar für das Kapitel „Dynamischer Speicher und Listen“

Diese Datei beschreibt die Regeln für das Inventar des Tresorraubs. Sie
enthält keinen Code. Wie Sie das Inventar im Programm darstellen, entscheiden
Sie selbst, solange es eine verkettete Liste mit dynamisch angefordertem
Speicher ist. Grundriss und Startpositionen stehen in `SPIELFELD.md`.

## Gegenstände

Es gibt drei Gegenstände. Ihre Namen im Programm sind:

| Gegenstand | Name im Programm | Startposition          | Piktogramm           |
|------------|------------------|------------------------|----------------------|
| Schlüssel  | `Schluessel`     | (3, 5)                 | `zeichne_schluessel` |
| Beute      | `Beute`          | im Tresor auf (13, 1)  | `zeichne_beute`      |
| Münze      | `Muenze`         | (11, 7)                | `zeichne_muenze`     |

## Regeln für das Inventar

- Das Inventar ist zu Beginn leer.
- Jeder Gegenstand kann höchstens einmal im Inventar sein.
- Das Inventar fasst höchstens drei Gegenstände.
- Ein neu aufgenommener Gegenstand steht im Inventar **vorne**. Die Anzeige
  zeigt die Gegenstände in dieser Reihenfolge von links nach rechts.
- Kann ein Gegenstand nicht aufgenommen werden, weil er schon im Inventar ist
  oder das Inventar voll ist, bleibt alles unverändert. Das ist kein
  Programmfehler, sondern ein normaler Spielfall mit einer Meldung.

## Tasten im Fenster

| Taste       | Wirkung                                             | Seite |
|-------------|-----------------------------------------------------|-------|
| Pfeiltasten | Figur bewegen, wie im Kapitel „Pointer“             | –     |
| Leertaste   | Gegenstand aufnehmen oder Tür öffnen                | 5     |
| L           | alle Gegenstände ablegen                            | 6     |
| Escape      | Programm beenden                                    | –     |

Nach der Leertaste und nach L gibt Ihr Programm im Terminal eine kurze
Meldung aus, zum Beispiel was aufgenommen wurde oder warum nicht.

## Leertaste: aufnehmen und Tür öffnen (Seite 5)

1. Liegt auf dem Feld der Figur ein Gegenstand, wird er aufgenommen. Er
   liegt danach nicht mehr auf dem Feld. Die Beute nimmt die Figur auf dem
   Feld des Tresors auf; der Tresor ist danach leer.
2. Sonst: Steht die Figur direkt neben der verschlossenen Tür, also auf
   (5, 3) oder (7, 3), und ist der Schlüssel im Inventar, öffnet sich die
   Tür. Der Schlüssel wird dabei verbraucht und aus dem Inventar entfernt.
   Ohne Schlüssel bleibt die Tür zu.
3. Sonst passiert nichts.

Nur eine erfolgreiche Handlung zählt als Zug: aufnehmen oder Tür öffnen.

## Taste L: ablegen (Seite 6)

- Die Gegenstände werden in der Reihenfolge des Inventars abgelegt, also von
  vorne nach hinten.
- Jeder Gegenstand kommt auf das erste **freie Nachbarfeld** der Figur. Die
  acht Nachbarfelder werden im Uhrzeigersinn geprüft, beginnend oben:
  oben, rechts oben, rechts, rechts unten, unten, links unten, links,
  links oben.
- Ein Feld ist frei, wenn es begehbar ist und dort weder Tür, Tresor,
  Ausgang, Wächter, die Figur noch ein anderer Gegenstand ist. Jedes Feld
  fasst höchstens einen Gegenstand.
- Findet ein Gegenstand kein freies Feld, bleibt er im Inventar, und es gibt
  eine Meldung.
- Ablegen zählt als ein Zug, wenn mindestens ein Gegenstand abgelegt wurde.
- Abgelegte Gegenstände können mit der Leertaste wieder aufgenommen werden.
  Die Beute kommt dabei nicht in den Tresor zurück.

## Anzeige

- Gegenstände, die auf einem Feld liegen, zeigt das Spielfeld mit ihrem
  Piktogramm. In der Textausgabe steht `b` für die Beute auf einem Feld.
- Steht die Figur auf einem Feld mit einem Gegenstand, zeigt der
  Fenstertitel zusätzlich `| Hier liegt: <Name>`, zum Beispiel
  `Position (3, 5) | Zuege 2/60 | Hier liegt: Schluessel`.
- Das Inventar erscheint unter dem Spielfeld ab der Position `INVENTAR_X`,
  `INVENTAR_Y` aus `tresorraub.c`, jeder Gegenstand auf einem eigenen Platz
  der Breite `FELDGROESSE`. Ein leeres Inventar zeigt nichts an.
