# Planung: Lernstandscheck II „Licht aus“

Stand 13.11.2026. Verantwortung: Larissa (vom Kollegen übernommen,
Aufgabenidee von ihm). Stellung im Kurs: Ende von Block 2, nach „Spielende“.
Freiwillige Selbstkontrolle: keine Abgabe, keine Bewertung.
Grundlage ist das Ende von „Spielende“, Teil 2 (Module).

## Entscheidungen (13.11.)

- **Zeit über einen neuen Anschluss im Gerüst:** `programm.c` ruft in
  jedem Schleifendurchlauf `spiel_aktualisieren(SDL_GetTicks64())` auf und
  erneuert den Titel, wenn die Funktion 1 liefert. Bisher war das Gerüst
  in allen Kapiteln gleich; die Änderung gilt nur im Startmaterial des
  Lernstandschecks.
- **Positionen über die Config ergänzen die Studierenden selbst** (prüft
  Config-Kompetenz): Wächter, Schlüssel, Lichtschalter, dazu
  `licht_dauer`. Namen und Bereiche sind vorgegeben, damit `REGELN.md` und
  Rückmeldungen einheitlich bleiben. Gesamtprüfung der Aufstellung nach dem
  Lesen.
- **F5 nur bei Licht.** Licht und Restzeit werden nicht gespeichert; ein
  geladener Stand hat Licht. Der Schalter ist eine weitere Objektzeile im
  Format Version 2.
- **Lichtschalter wie die Tür:** Leertaste vom Nachbarfeld, Feld nicht
  betretbar; erneutes Betätigen startet die Dauer neu.
- **Test-Suite aus „Spielende“ bleibt Pflicht.** Die Prüfung der
  Objektzahl erlaubt 8 bis `MAX_OBJEKTE`, damit der Schalter sie nicht
  verletzt.

## Aufbau der Seite

Worum es geht · Materialien (drei Neuerungen) · Spielregeln für das Licht
(Schalter, Licht und Wächter, Anzeige, Speichern, Tabelle der neuen
Einträge mit Gesamtprüfung) · Aufgaben 1 Planen, 2 Schalter, 3 Config und
eigene Anordnung, 4 Zeit (Hinweis Zeitsteuerung), 5 Prüfen und beschreiben.

## Offen

- SDL-Version auf den Pool-Rechnern (≥ 2.0.18) prüfen.
- Bearbeitungszeit: geschätzt zwei Termine (rund 3 h), freiwillig neben
  oder nach Block 2.
