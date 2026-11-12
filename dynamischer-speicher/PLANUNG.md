# Planung: Kapitel „Dynamischer Speicher und Listen“

Stand 12.11.2026. Stellung im Kurs: Block 2, Kapitel 6, nach „Pointer und
Datenaustausch“, vor „Datei-Ein- und -Ausgabe“. Stil wie „Datenstrukturen“
und „Pointer“: keine vorgegebenen Funktionsnamen und Signaturen,
Auswahlfragen dort, wo der Inhalt vermittelt wird, Hinweise und
Selbstkontrollen direkt bei der Aufgabe.

## Voraussetzungen

- „Datenstrukturen“: Arrays, Zeichenketten (`snprintf`, `strcmp`),
  Strukturen mit und ohne `typedef`, Rasterabstand, Kopie bei Übergabe
  als Wert
- „Manpages“: Funktionsverträge nachschlagen
- „Header und Quelldateien“: `static` für Variablen einer Datei
- „Pointer und Datenaustausch“: `&`, `*`, `->`, `NULL`, `const T *`,
  Lebensdauer des Ziels, laufender Spielstand als `static`-Variable,
  `figur_bewegen`, Statuszeile im Fenstertitel. Begriffe: „Pointer“,
  „zeigt auf“, „Ziel“, „Adresse“, „Felder“.

## Leitidee

Die Studierenden bauen das Inventar des Tresorraubs als einfach verkettete
Liste mit dynamisch angefordertem Speicher. Grundlage ist das Spiel am Ende
des Kapitels „Pointer“. Die Liste ersetzt im Spielstand die drei
`bool`-Werte. Mit der Leertaste nimmt die Figur Gegenstände auf ihrem Feld
auf und öffnet die Tür vom Nachbarfeld aus (Schlüssel wird verbraucht); mit
L legt sie alle Gegenstände auf freie Nachbarfelder ab. Das Inventar
erscheint unter dem Spielfeld. Wächter, Ausgang und Zuglimit folgen beim
Zusammenführen der Spielbausteine.

## Seiten

| Seite | Inhalt | Ergebnis | Auswahlfragen |
|---|---|---|---|
| 1 Lebensdauer und Speicherbereiche | automatische, statische, dynamische Lebensdauer; Speicherbild (Code, statische Daten, Stack, Heap); Versuch „Adresse einer lokalen Variablen zurückgeben“ | ausgefülltes Speicherbild | Lebensdauer; Pointer überlebt Ziel |
| 2 Speicher anfordern und freigeben | `malloc`/`free` nachschlagen; Grundmuster mit `sizeof(*p)`; Versuche zu Use-after-free, Double-free, Leck | Funktion, die einen Eintrag auf dem Heap anlegt, mit Fehlerpfad | Grundmuster; Speicherfehler |
| 3 Eine verkettete Liste | Knoten, Kopf, `NULL`-Ende; selbstbezüglicher Strukturtyp; Durchlaufen; vorne einfügen | zählen, enthält, ausgeben, aufnehmen nach Regeln | Listenaufbau; Durchlaufen; Einfügen |
| 4 Entfernen, aufräumen und Besitz | Aushängen vor Freigeben; Vorgänger merken; Liste leeren; Besitz und Ausleihen | Entfernen und Leeren, Verträge mit Besitz | Entfernen; Aufräumen; Besitz |
| 5 Das Inventar im Spiel | Inventar im Spielstand; Versuch „Kopie teilt die Liste“; Leertaste (aufnehmen, Tür); „Hier liegt“ im Titel; Zeichnen; Spielfall vs. Programmfehler | Spiel mit Aufnehmen und Tür ohne Speicherfehler | Kopie und Besitz; Leertaste; Fehlerfälle |
| 6 Gegenstände ablegen | freie Nachbarfelder im Uhrzeigersinn; Liste durchlaufen und dabei Einträge entfernen; Beute außerhalb des Tresors | Ablegen und Wiederaufnehmen | Ablegen; Entfernen beim Durchlaufen |

Seite 3 löst eine Ankündigung aus „Datenstrukturen“ ein (dort Seite 4:
Strukturen, die auf eine Struktur desselben Typs verweisen).

## Material

- `materialien/tresorraub-inventar.zip`: einheitliches Programmgerüst für
  Block 2 (`programm.c`, `tresorraub.h`), Piktogramme, Makefile,
  `SPIELFELD.md`, `INVENTAR.md` (Regeln, Tasten, Anzeige), `README.md` mit
  Speicherprüfung für Linux, macOS und Windows und `tresorraub.c` mit dem
  Stand am Ende des Kapitels „Pointer“ (dessen Musterlösung ohne die
  Versuche), ergänzt um `INVENTAR_X` und `INVENTAR_Y`.
  Nach Änderungen im Ordner das ZIP neu erzeugen (ohne `programm`,
  `programm.exe` und versteckte Dateien).
- `loesung/`: Musterlösung, Tests (mit kontrolliert fehlschlagendem
  `malloc`, Spielablauf über `taste_gedrueckt`, `zeichne` per
  Software-Renderer), Makefile, README mit Erwartungshorizont. Nicht im ZIP,
  nicht verlinkt, aber im öffentlichen Repository sichtbar.

## Entscheidungen

- **Inventar im Spielstand:** Der laufende Spielstand bleibt die eine
  `static`-Variable aus „Pointer“ und besitzt das Inventar. Eine Kopie des
  Spielstands teilt sich die Liste; das zeigt der Versuch auf Seite 5.
- **Leertaste:** Aufnehmen nur auf dem Feld des Gegenstands (Beute auf dem
  Tresorfeld), Tür vom Nachbarfeld aus mit Schlüssel. Nur erfolgreiche
  Handlungen zählen als Zug. Steht die Figur auf einem Gegenstand, zeigt
  der Fenstertitel „Hier liegt: …“.
- **Taste L:** Gegenstände in Listenreihenfolge auf das erste freie der
  acht Nachbarfelder, im Uhrzeigersinn ab oben. Frei: begehbar, keine Tür,
  kein Tresor, kein Ausgang, kein Wächter, nicht die Figur, kein anderer
  Gegenstand. Was keinen Platz findet, bleibt im Inventar (im Spiel nicht
  erreichbar, weil höchstens zwei Gegenstände gleichzeitig im Inventar
  sind). Zählt als ein Zug, wenn mindestens ein Gegenstand abgelegt wurde.
- **Name statt `enum`:** Ein Eintrag speichert den Gegenstand als
  Zeichenkette. Das nutzt `snprintf` und `strcmp` aus „Datenstrukturen“.
  `enum` wird erst im Kapitel „Objektorientierung in C“ eingeführt.
- **Vorne einfügen:** einfachster Fall, kein Sonderfall für die leere Liste.
- **Höchstens drei Einträge:** wird im Spiel nie erreicht; bleibt bewusst
  als Regel für einen testbaren Ablehnungsfall mit einem erfundenen Namen
  (`Kompass`). Sie übt, Regeln vor dem `malloc` zu prüfen.
- **Alte Platzhalter entfernt:** `speicheraufbau/` und `verkettete-listen/`
  sind in diesem Kapitel aufgegangen und wurden gelöscht.
- **Speicherprüfung:** AddressSanitizer findet unter macOS keine Lecks; dort
  `leaks --atExit`. Unter MSYS2 gibt es beides nicht.

## Zeitbedarf (Schätzung)

Zwei Termine: Seiten 1–3 im ersten, Seiten 4–6 im zweiten. Bei nur einem
Termin: Seite 1 kürzen, Entfernen (Seite 4, Abschnitt 1) gemeinsam
besprechen statt selbst implementieren, Seite 6 als Vertiefung.

## Offene Entscheidungen

- Zwei Termine oder kompakte Fassung? Ist Seite 6 Pflicht oder Vertiefung?
  (wird später entschieden)
