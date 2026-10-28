# Spielregeln der Grundversion

## Auftrag

Sie betreten ein kleines Museum. Holen Sie den Museumsschlüssel, öffnen Sie die Verbindungstür, sichern Sie die Tresorbeute und kehren Sie zum Ausgang zurück. Die Bonusmünze ist ein freiwilliges Zusatzziel.

Die Browser-Vorschau zeigt die nachfolgenden Spielobjekte als Piktogramme. Überwachte Felder sind dort ohne Ausrufezeichen farbig hinterlegt und schraffiert. Die lächelnde Spielfigur wird bei Entdeckung traurig, Wächter schauen böse. Die Zeichen in der Tabelle gelten für die C-Terminalfassung.

Die Karte hat 15 Spalten und 9 Zeilen. Die Koordinaten beginnen links oben bei `(0, 0)`. Die Spielfigur startet bei `(3, 7)`.

| Zeichen | Bedeutung | Verhalten beim Betreten |
|---|---|---|
| @ | Spielfigur | Aktuelle Position |
| # | Wand | Bewegung blockiert |
| . / leeres Feld | Boden | Frei betretbar |
| k | Schlüssel bei (3, 5) | Landet im Inventar und verschwindet von der Karte |
| D | Tür bei (6, 3) | Benötigt und verbraucht den Schlüssel |
| / | Geöffnete Tür | Bleibt frei passierbar |
| T | Tresor bei (13, 1) | Beute landet im Inventar |
| t | Bereits geleerter Tresor | Kein erneuter Gegenstand |
| E | Ausgang bei (1, 1) | Mit Beute gewonnen, ohne Beute noch kein Sieg |
| c | Bonusmünze bei (11, 7) | Landet im Inventar |
| W | Wächter | Unbeweglich |
| ! | Überwachtes Feld | Betreten führt zur Entdeckung |

Der linke Wächter steht bei `(1, 5)` und überwacht Entfernung 1. Der rechte steht bei `(9, 5)` und überwacht Entfernung 2.

## Überwachung

Die Entfernung ist die Anzahl horizontaler plus vertikaler Schritte:

`|spieler.x - waechter.x| + |spieler.y - waechter.y|`

Ein Feld ist überwacht, wenn diese Entfernung höchstens dem Radius des Wächters entspricht. Der Wächterstandort selbst ist ebenfalls überwacht. Die Grenzlinie gehört zum Bereich.

Das ist eine bewusst einfache Erkennungszone. Es gibt keine Blickrichtung, keine Patrouille und keine Sichtlinienberechnung. Wände schirmen diesen Bereich in der Grundversion nicht ab. Der gesamte Bereich ist vorab sichtbar. Das Spiel ist ein Wegfindungsrätsel und reagiert ausschließlich auf Spielaktionen.

## Züge und Spielende

- Erfolgreiche Bewegung um ein Feld: ein Zug.
- Warten: ein Zug.
- Wand, verschlossene Tür ohne Schlüssel oder unbekannter Befehl: kein Zug.
- Inventar, Hilfe, Speichern und Laden: kein zusätzlicher Zug.
- Nach dem Laden gilt wieder der Zugzähler aus dem Spielstand.
- Nach dem 60. Zug ohne Sieg endet das Spiel durch Zeitablauf.
- Ein Sieg im 60. Zug ist gültig.
- Entdeckung hat Vorrang vor Sieg und Zeitablauf. In dieser Karte liegt der Ausgang außerhalb aller Überwachungsbereiche.

Schlüsselaufnahme, Türöffnung, Tresoröffnung und Münzaufnahme erfolgen automatisch mit dem Betreten des Feldes. Es gibt keine zusätzliche Interaktionstaste. Schlüssel und Beute sind verschiedene Gegenstände. Beim Öffnen der Tür wird der Schlüssel aus der dynamischen Inventarliste entfernt und freigegeben.

Ein gewonnenes, verlorenes oder zeitlich abgelaufenes Spiel verarbeitet keine weiteren Bewegungen. Speichern ist nur während eines laufenden Spiels möglich.

## Gewinnroute zum Nachvollziehen

Diese Route ist für die Lehrenden und zum Testen gedacht. Für einen ersten Spielversuch muss sie nicht gelesen werden.

- Hinweg vom Start zum Tresor: `wwddwwddwwdddddd` (16 Züge).
- Rückweg vom Tresor zum Ausgang: `aaaaaassaawwaaaa` (16 Züge).
- Zusammen: **32 Züge**.
- Für die Bonusmünze nach dem Tresor zusätzlich `ssssssaaddwwwwww`, dann den Rückweg nehmen. Zusammen: **48 Züge**.

In der C-Fassung wird jeder Buchstabe einzeln mit Enter bestätigt. In der Browser-Vorschau genügt der Tastendruck.

## Bewusste Umfangsgrenzen

Eine Karte, zwei feste Wächter, eine Tür, ein Schlüssel, ein Tresor, ein Ausgang und eine optionale Münze. Kartengenerator, Echtzeitgrafik, Gegner-KI und mehrere Level gehören nicht zur Grundversion. Die Kartenausgabe ist austauschbar: Der Spielzustand und die Regeln hängen nicht von einer bestimmten Anzeige ab.
