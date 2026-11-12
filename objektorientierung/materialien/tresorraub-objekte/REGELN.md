# Spielregeln des Tresorraubs

Einige Spielregeln stehen nicht mehr fest im Quelltext, sondern in der Datei
`regeln.txt`. Diese Datei beschreibt das Format und das erwartete Verhalten.
Sie enthält keinen Code.

## Regeln

| Name                    | Bedeutung                              | Standard | erlaubt  |
|-------------------------|----------------------------------------|----------|----------|
| `zuglimit`              | Zahl der Züge im Fenstertitel          | 60       | 1 bis 999 |
| `reichweite_waechter_1` | Reichweite des Wächters auf (1, 5)     | 1        | 0 bis 5  |
| `reichweite_waechter_2` | Reichweite des Wächters auf (9, 5)     | 2        | 0 bis 5  |
| `inventar_groesse`      | höchste Zahl der Einträge im Inventar  | 3        | 1 bis 3  |

Steht eine Regel nicht in der Datei, gilt ihr Standardwert.

## Format von `regeln.txt`

- Jede Zeile ist leer, ein Kommentar oder eine Regel.
- Ein Kommentar beginnt mit `#`; davor dürfen Leerzeichen stehen.
- Eine Regel hat die Form `name = wert`. Vor und nach dem Namen, dem `=`
  und dem Wert dürfen Leerzeichen stehen. Der Name selbst enthält keine
  Leerzeichen.
- Der Wert ist eine ganze Zahl. Nach der Zahl darf nur noch Leerraum folgen.
- Jede Regel darf höchstens einmal vorkommen.
- Eine Zeile ist höchstens 80 Zeichen lang.

Beispiel:

```
# Spielregeln für den Tresorraub
zuglimit = 40
reichweite_waechter_2 = 3
```

## Verhalten

- Das Programm liest `regeln.txt` aus dem Ordner, in dem es gestartet wird:
  im Fenster in `spiel_starten`, im Textmodus zu Beginn von
  `text_ausgeben`.
- **Datei fehlt:** Das Programm meldet das im Terminal und spielt mit den
  Standardwerten.
- **Datei fehlerhaft:** Das Programm meldet den ersten Fehler mit
  Dateiname, Zeilennummer und Grund, zum Beispiel
  `regeln.txt, Zeile 3: unbekannte Regel "zuglimt"`. Es übernimmt dann
  keinen einzigen Wert aus der Datei und startet nicht: `spiel_starten`
  bzw. `text_ausgeben` liefert einen negativen Wert.
- Fehler sind: eine Zeile ohne `=`, ein unbekannter Name, ein Wert, der
  keine ganze Zahl ist oder nach dem noch etwas folgt, ein Wert außerhalb
  des erlaubten Bereichs, kein oder mehr als ein Wort vor dem `=`, eine
  doppelte Regel und eine zu lange Zeile.
