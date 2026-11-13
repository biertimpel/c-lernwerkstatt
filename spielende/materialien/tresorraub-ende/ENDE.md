# Spielende

Diese Datei beschreibt, wie ein Spiel des Tresorraubs endet. Sie enthält
keinen Code. Alle übrigen Regeln aus `SPIELFELD.md`, `INVENTAR.md`,
`OBJEKTE.md`, `REGELN.md` und `FORMAT.md` gelten weiter.

## Zustände eines Spiels

| Zustand         | Name im Programm  | Wann                                                   |
|-----------------|-------------------|--------------------------------------------------------|
| läuft           | `LAEUFT`          | nach dem Start, nach N und nach erfolgreichem Laden    |
| gewonnen        | `GEWONNEN`        | Die Figur betritt den Ausgang mit der Beute im Inventar |
| entdeckt        | `ENTDECKT`        | Die Figur steht nach einem Zug auf einem überwachten Feld |
| Zeit abgelaufen | `ZEIT_ABGELAUFEN` | Nach einem Zug ist das Zuglimit erreicht, ohne Sieg     |

Die Namen im Programm brauchen Sie spätestens in Teil 2: Die vorgegebenen
Header verwenden einen Aufzählungstyp `Spielzustand` mit diesen vier Werten
und ein Feld `zustand` im Spielstand.

## Gewinnen

- Betritt die Figur den Ausgang auf (1, 1) und ist die Beute im Inventar,
  ist das Spiel gewonnen. Meldung im Terminal: `Mit der Beute entkommen!`
- Betritt sie den Ausgang ohne Beute, geht das Spiel weiter. Meldung:
  `Ohne Beute geht es noch nicht hinaus.`
- Das Betreten zählt wie jeder Schritt als Zug. Ein Sieg im letzten
  erlaubten Zug, bei Zuglimit 60 also im 60. Zug, zählt als Sieg.

## Verlieren

Nach jedem gezählten Zug wird geprüft, in dieser Reihenfolge:

1. Steht die Figur auf einem überwachten Feld, ist sie entdeckt. Überwacht
   ist jedes Feld, dessen Rasterabstand zu einem Wächter höchstens dessen
   Reichweite ist, auch das Feld des Wächters selbst. Meldung: `Entdeckt!`
2. Sonst: Ist die Zahl der Züge das Zuglimit aus `regeln.txt` oder mehr,
   ist die Zeit abgelaufen. Meldung: `Die Zeit ist abgelaufen.`

Ist das Spiel schon gewonnen, wird nicht mehr geprüft.

## Nach dem Spielende

- Pfeiltasten, Leertaste, L und F5 bewirken nichts mehr und zählen keinen
  Zug. Meldung: `Das Spiel ist vorbei. N: neues Spiel, F9: Spielstand laden.`
- F9 lädt wie bisher `spielstand.txt`. Danach läuft das Spiel weiter.
- N startet jederzeit ein neues Spiel mit den Regeln aus `regeln.txt`,
  auch während eines laufenden Spiels. Meldung: `Neues Spiel.`

## Anzeige

Der Fenstertitel bekommt nach dem Spielende am Ende einen Zusatz, wie die
anderen Teile der Statuszeile durch einen senkrechten Strich abgetrennt:

| Zustand         | Zusatz                    |
|-----------------|---------------------------|
| gewonnen        | `Entkommen!`              |
| entdeckt        | `WASTED: entdeckt`        |
| Zeit abgelaufen | `WASTED: Zeit abgelaufen` |

Beispiel: `Position (9, 3) | Zuege 22/60 | WASTED: entdeckt`

Im Fenster:

- Entdeckt: Die Figur wird mit `zeichne_spielfigur_entdeckt` gezeichnet.
  Zuletzt wird mit `zeichne_wasted` der Stempel WASTED quer über das ganze
  Spielfeld gesetzt.
- Zeit abgelaufen: Die Figur bleibt, wie sie ist. Zuletzt wird mit
  `zeichne_wasted` der Stempel WASTED quer über das ganze Spielfeld gesetzt.
- Gewonnen: Wie Sie den Sieg im Fenster zeigen, entscheiden Sie selbst.

## Speichern und Laden

- Ein beendetes Spiel wird nicht gespeichert (siehe oben: F5 nach dem
  Spielende).
- Laden lehnt Spielstände ab, in denen das Spiel schon zu Ende wäre: Die
  Figur steht auf einem überwachten Feld oder auf dem Ausgang, oder die Zahl
  der Züge erreicht das Zuglimit aus `regeln.txt`. Wie bei jedem anderen
  Fehler bleibt dann der bisherige Spielstand erhalten.
- Ein geladener Spielstand läuft.
