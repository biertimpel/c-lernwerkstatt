# Spielstandformat Version 1

Es wird ein Spielstand als lesbarer Text gespeichert. Die Kennwoerter und
Werte folgen der unten angegebenen Reihenfolge. Leerzeichen und Zeilenumbrueche
trennen die Werte. Die Namen sind gross-/kleinschreibungsabhaengig.

    TRESORRAUB 1
    FIGUR 3 7
    ZUEGE 0
    TUER 0
    SCHLUESSEL 3 5 1
    MUENZE 11 7 1
    BEUTE 13 1 0 1
    INVENTAR 0
    ENDE

- FIGUR: x y.
- ZUEGE: nicht negative Ganzzahl im Wertebereich von int.
- TUER: offen (0 oder 1).
- SCHLUESSEL und MUENZE: x y liegt_auf_dem_spielfeld (0 oder 1).
- BEUTE: x y liegt_auf_dem_spielfeld ist_noch_im_tresor (je 0 oder 1).
- INVENTAR: Anzahl, 0 bis 3. Danach folgen genau so viele Namen, vom Kopf
  zum Listenende, einer pro Zeile: Schluessel, Beute oder Muenze.
- ENDE: Abschluss. Danach folgt nur noch Leerraum oder das Dateiende.

Auch Positionen momentan nicht ausgelegter Gegenstaende werden mitgespeichert.
Grundriss, Ausgang, Tuer-/Tresorposition und Waechter werden aus dem bekannten
Startzustand uebernommen. Einstellungen werden erst im Kapitel Config Thema.
Die Beispieldateien stammen aus dem Zustand am Ende von Dynamischer Speicher.

Pruefregeln:
- Figur und Gegenstandspositionen innerhalb des Spielfelds und auf begehbaren
  Feldern. Figur nicht in der verschlossenen Tuer.
- Ausgelegte Gegenstaende nicht auf Ausgang, Tuer, Tresor oder einem Waechter
  und nicht auf demselben Feld wie ein anderer ausgelegter Gegenstand.
- Jeder Inventarname bekannt und hoechstens einmal vorhanden.
- Schluessel genau einmal vorhanden: auf dem Feld, im Inventar oder bei offener
  Tuer verbraucht. Muenze auf dem Feld oder im Inventar. Beute im Tresor,
  auf dem Feld oder im Inventar. Keine widerspruechlichen Mehrfachvorkommen.

F5 speichert spielstand.txt, F9 laedt spielstand.txt. L bleibt Ablegen.
Speichern und Laden verbrauchen keinen Zug. Bei einem fehlgeschlagenen Laden
bleibt der laufende Spielstand einschliesslich seiner Liste unveraendert.
Nach erfolgreichem Laden besitzen die neuen Listeneintraege eigene Adressen.
Diese Adressen sind nicht Bestandteil des Formats.
