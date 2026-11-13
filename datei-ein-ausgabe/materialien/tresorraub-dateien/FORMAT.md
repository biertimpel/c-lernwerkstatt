# Spielstandformat, Version 1

Ein Spielstand wird als lesbarer Text gespeichert. Kennwörter und Werte
stehen in der unten angegebenen Reihenfolge. Leerzeichen und Zeilenumbrüche
trennen die Werte. Groß- und Kleinschreibung der Namen zählt.

```
TRESORRAUB 1
FIGUR 3 7
ZUEGE 0
TUER 0
SCHLUESSEL 3 5 1
MUENZE 11 7 1
BEUTE 13 1 0 1
INVENTAR 0
ENDE
```

| Zeile        | Werte                                                                 |
|--------------|-----------------------------------------------------------------------|
| `TRESORRAUB` | Version des Formats, hier `1`                                          |
| `FIGUR`      | x y                                                                   |
| `ZUEGE`      | nicht negative ganze Zahl im Wertebereich von `int`                   |
| `TUER`       | offen: 0 oder 1                                                       |
| `SCHLUESSEL` | x y liegt_auf_dem_spielfeld (0 oder 1)                                |
| `MUENZE`     | x y liegt_auf_dem_spielfeld (0 oder 1)                                |
| `BEUTE`      | x y liegt_auf_dem_spielfeld ist_noch_im_tresor (je 0 oder 1)          |
| `INVENTAR`   | Anzahl 0 bis 3; danach genau so viele Namen vom Listenkopf zum Ende, je einer pro Zeile: `Schluessel`, `Beute` oder `Muenze` |
| `ENDE`       | Abschluss; danach folgt nur noch Leerraum oder das Dateiende          |

Auch Positionen von Gegenständen, die gerade nicht auf dem Feld liegen, werden
mitgespeichert. Grundriss, Ausgang, Position von Tür und Tresor und die Wächter
übernimmt das Programm aus dem bekannten Startzustand. Die Adressen der
Inventareinträge gehören nicht zum Format; nach dem Laden haben die Einträge
neue Adressen.

## Prüfregeln

- Jede Zahl ist eine ganze Zahl im Wertebereich von `int`; Ja/Nein-Werte sind
  0 oder 1.
- Figur und Gegenstandspositionen liegen im Spielfeld auf begehbaren Feldern.
  Die Figur steht nicht in der verschlossenen Tür.
- Ausgelegte Gegenstände liegen nicht auf Ausgang, Tür, Tresor oder einem
  Wächter und nicht auf demselben Feld wie ein anderer ausgelegter Gegenstand.
- Jeder Inventarname ist bekannt und höchstens einmal vorhanden.
- Der Schlüssel ist genau an einer Stelle: auf dem Feld, im Inventar oder bei
  offener Tür verbraucht. Die Münze liegt auf dem Feld oder ist im Inventar.
  Die Beute ist im Tresor, auf dem Feld oder im Inventar.

## Verhalten

F5 speichert `spielstand.txt`, F9 lädt `spielstand.txt`. Speichern und Laden
verbrauchen keinen Zug. Bei einem fehlgeschlagenen Laden bleibt der laufende
Spielstand einschließlich seiner Liste unverändert.
