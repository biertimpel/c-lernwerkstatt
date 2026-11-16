# Spielstandformat

Das Startmaterial speichert und lädt Spielstände im Format Version 2 aus dem
Kapitel „Objektorientierung in C“. Es beschreibt jedes Objekt in einer
eigenen Zeile. Das Format bleibt in diesem Kapitel gleich; neu sind nur die
Regeln für das Spielende (siehe unten und `ENDE.md`).

## Version 2

```
TRESORRAUB 2
FIGUR 3 7
ZUEGE 0
OBJEKTE 6
Tuer 6 3 1 0 -1
Tresor 13 1 1 0 4
Schluessel 3 5 1 0 -1
Muenze 11 7 1 0 -1
Beute 13 1 0 0 -1
Ausgang 1 1 1 0 -1
INVENTAR 0
ENDE
```

Das Beispiel zeigt den Startzustand ohne Geheimtür und zweite Münze; mit
ihnen stehen 8 Objekte in der Datei. Die Reihenfolge der Objekte
im Beispiel ist die der Musterlösung; Ihre Reihenfolge darf anders sein, muss
aber beim Speichern und Laden dieselbe sein.

| Zeile        | Werte                                                              |
|--------------|--------------------------------------------------------------------|
| `TRESORRAUB` | Version des Formats: `2`                                            |
| `FIGUR`      | x y                                                                |
| `ZUEGE`      | nicht negative ganze Zahl im Wertebereich von `int`                |
| `OBJEKTE`    | Anzahl der Objekte des Spiels; danach je Objekt eine Zeile          |
| je Objekt    | Name der Art, x, y, liegt_auf_dem_feld (0/1), offen (0/1), Nummer des enthaltenen Objekts oder `-1` |
| `INVENTAR`   | Anzahl; danach je Eintrag die Nummer des Objekts, vom Listenkopf zum Ende, je eine pro Zeile |
| `ENDE`       | Abschluss; danach folgt nur noch Leerraum oder das Dateiende       |

Objekte werden über ihre **Nummer** angesprochen: ihre Stelle in der Liste
der Objekte, gezählt ab 0. Im Beispiel enthält der Tresor (Nummer 1) die
Beute (Nummer 4). Adressen gehören nicht in die Datei.

## Prüfregeln für Version 2

- Version 2. Anzahl und Reihenfolge der Arten stimmen mit den Objekten des
  Spiels überein. Version 1 wird abgelehnt.
- Jede Zahl ist eine ganze Zahl im Wertebereich von `int`; Ja/Nein-Werte
  sind 0 oder 1. Die Zugzahl ist nicht negativ.
- Alle Positionen liegen im Spielfeld auf begehbaren Feldern. Objekte, die
  nicht ins Inventar können (Tür, Tresor, Ausgang), stehen auf dem Feld an
  ihrer Startposition.
- Ein enthaltenes Objekt ist ein Gegenstand, der nicht auf dem Feld liegt.
- Inventarnummern bezeichnen Gegenstände. Jeder Gegenstand ist höchstens an
  einer Stelle: auf dem Feld, im Inventar oder in einem anderen Objekt. Ist
  er nirgends, wurde er verbraucht.
- Auf jedem Feld liegt höchstens ein Objekt, keines auf einem Wächter.
- Die Figur steht auf einem Feld, das sie betreten darf, also nicht in einer
  geschlossenen Tür.
- Das Inventar hat höchstens so viele Einträge, wie `inventar_groesse` in
  `regeln.txt` erlaubt. Sonst bricht das Laden mit einer Meldung ab.
- Das Spiel wäre nicht schon zu Ende: Die Figur steht nicht auf einem
  überwachten Feld und nicht auf dem Ausgang, und die Zahl der Züge ist
  kleiner als `zuglimit` in `regeln.txt`. Ein geladener Spielstand läuft.

## Version 1

Format und Prüfregeln von Version 1 stehen im Kapitel „Datei-Ein- und
-Ausgabe“ in `FORMAT.md`.

## Verhalten

F5 speichert `spielstand.txt`, F9 lädt `spielstand.txt`. Nach dem
Spielende speichert F5 nicht mehr; F9 lädt weiterhin. Speichern und Laden
verbrauchen keinen Zug. Bei einem fehlgeschlagenen Laden bleibt der laufende
Spielstand einschließlich seiner Liste unverändert.
