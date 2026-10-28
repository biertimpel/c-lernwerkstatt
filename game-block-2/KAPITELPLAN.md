# Block 2: Zielbild und Kapitelzuordnung

## Grundidee

Die Lehrenden planen zuerst die kleine vollständige Spielgrundversion. Die Studierenden bearbeiten zunächst eigenständig verständliche Aufgaben. Dabei entstehen Bausteine mit passenden Datentypen, Funktionssignaturen und geprüften Ergebnissen. Beim Thema Header und Quelldateien werden diese Bausteine verbunden. Hier entsteht die spielbare Gesamtversion.

Die nachfolgenden Vorschläge sind **Arbeitsgegenstände und Zielzustände**, keine fertigen Aufgabenstellungen. Reihenfolge, Umfang, Hilfen und konkrete Szenarien werden von Jonathan und Larissa gemeinsam festgelegt. Eine in der Tabelle genannte Datei ist ein Wegweiser im fertigen Referenzcode. Sie muss im entsprechenden frühen Kapitel noch nicht als eigenständiges Modul vorliegen.

## Überkapitel

| Überkapitel | Was erarbeitet werden könnte | Zunächst eigenständiger Kontext | Beitrag im fertigen Spiel |
|---|---|---|---|
| **Vom Problem zum Programm** | Anforderungen verstehen, Zuständigkeiten abgrenzen, Schnittstellen notieren, eine vertikale erste Version bauen, Änderungen kontrolliert einarbeiten, Ergebnisse erklären und übergeben | Kleines Verwaltungs- oder Rastersystem mit klaren Ein- und Ausgaben | Arbeitsweise für alle späteren Bausteine. Die sechs Schritte werden auch beim Zusammenbauen wieder aufgenommen. |
| **Manpages** | Funktionsbeschreibung lesen, Parameter und Rückgabewert unterscheiden, Fehlerfälle und benötigte Header herausfinden. Bibliotheksfunktion in einem kleinen eigenen Versuch prüfen. | Eingabezeilen verarbeiten, Zeichenketten untersuchen, Zahlen lesen | `fgets`, `strtol`, `snprintf`, `strlen`, `strcmp`, `strcspn`, `malloc`, `free`; später die Dateioperationen. Die robustere Eingabe darf zunächst bereitgestellt werden. Nicht alle Funktionen müssen auf einmal behandelt werden. |
| **Datenstrukturen** | Eindimensionale und zweidimensionale Arrays, gültige Indizes, Zeichenketten und Nullterminierung, `struct` für zusammengehörige Daten | Markiertes Raster, Koordinaten, beschriftete Datensätze | `Punkt`, Kartenarray, Meldungstext, Spielerdaten. Eine einfache Liste kann zunächst als Modell besprochen werden. Der konkrete Listenaufbau folgt nach Pointern und dynamischem Speicher. |
| **Pointer und Datenaustausch** | Adressen und Dereferenzierung, Zustandsänderung über Funktionsparameter, lesender Zugriff mit `const`, Arrays und Pointer auseinanderhalten | Position einer Einheit verändern, Datensatz aktualisieren, einen Rastereintrag prüfen | Funktionen verändern `Spiel *` bzw. `Spieler *`. Abfragen erhalten `const Spiel *`. Grenzen werden vor einem Zugriff geprüft. Keine Notwendigkeit, hier bereits alle OOP-Typumwandlungen zu erklären. |
| **Dynamischer Speicher** | Lebensdauer, Stack und Heap, `malloc`/`calloc`, Fehler beim Anlegen, Eigentum, `free`, Verkettung, Einfügen und Entfernen | Veränderliche Sammlung beschrifteter Einträge | Dynamische Inventarliste und Weltobjektliste. Ein Schlüssel wird aufgenommen, beim Verwenden entfernt und freigegeben. Sämtliche Objekte werden beim Spielende aufgeräumt. `inventar.c` und der Lebenszyklus in `objekt.c`. |
| **Objektorientierung in C – zusätzlich einzuplanen** | Daten und zugehöriges Verhalten, gemeinsame Basis, eingebettete Strukturen, Funktionspointer, gemeinsame Methodentabelle, dynamischer Methodenaufruf | Verschiedene Einträge reagieren auf denselben Aufruf unterschiedlich | `Objekt` als gemeinsame Basis; konkrete Typen `Aufhebbar`, `Tuer`, `Tresor`, `Waechter`, `Ausgang`. Der generische Bewegungscode ruft `objekt->methoden->betreten(...)` auf. Ein Schlüssel landet im Inventar, eine Tür prüft einen Schlüssel und ein Tresor liefert Beute. |
| **Makefiles** | Übersetzen und Linken auseinanderhalten, mehrere Quellen bauen, Abhängigkeiten von Headern erkennen, nachvollziehbare Build-Ziele | Zunächst zwei kleine Quelldateien mit gemeinsamer Schnittstelle | Die einzelnen Objektdateien werden zu `tresorraub` gelinkt. Der Referenz-Makefile enthält automatische Header-Abhängigkeiten; das kann als Hilfestellung gegeben werden. Ein kurzer eigener einfacher Makefile sollte dem Komfort-Makefile vorausgehen. |
| **Header, Quelldateien und Schnittstellen** | Deklaration und Implementierung trennen, Include-Schutz, Verantwortlichkeiten, fremde Schnittstellen korrekt verwenden, Testtreiber vom eigentlichen `main` trennen | Bisherige geprüfte Bausteine zusammenführen | Spielmodell, Objekttypen, Inventar, Eingabe, Anzeige und Speicherung werden getrennt gebaut. **Hier wird die Überraschung sichtbar: Die Aufgaben ergeben ein spielbares Spiel.** Integration erst durchführen, wenn die benötigten Kapitelbausteine vorliegen. |
| **Datei-Ein- und -Ausgabe – zusätzlicher Unterabschnitt** | Textdatei öffnen, schreiben, lesen und schließen; alle Rückgaben prüfen; Dateiformat beschreiben; ungültige Daten abweisen; Zustand wiederherstellen | Kleinen vollständigen Datenbestand sichern und wiederherstellen | `speichern.c` schreibt explizite Werte statt Speicherabbildern. Laden baut einen neuen Zustand auf und übernimmt ihn erst nach erfolgreicher Prüfung. Dieser Inhalt muss ausdrücklich eingeplant werden: Er ergibt sich nicht automatisch aus OOP oder Headern. |
| **Lernstandscheck II** | Eine bekannte vollständige Grundversion um eine klar begrenzte Anforderung erweitern, Schnittstellen nutzen, Tests begründen | Bereits zusammengeführtes Spiel mit geprüften Referenzbausteinen | Neue Verhaltensweise oder neuer interaktiver Objekttyp. Dafür wird vorher ein analoger kleiner Erweiterungsschritt geübt. Die finale Aufgabenstellung ist noch offen. |

## Wo Objektorientierung beginnt

Bei Datenstrukturen werden zusammengehörige Eigenschaften zunächst in `struct` gebündelt. Beim Pointerkapitel arbeiten Funktionen mit konkreten Instanzen. Dynamischer Speicher ergänzt die Lebensdauer der Instanzen. Das eigene OOP-Kapitel baut darauf auf und führt Funktionspointer, Struktureinbettung und unterschiedliche Implementierungen derselben Schnittstelle ein. Es liegt vor dem Zusammenbauen und vor Lernstandscheck II.

OOP soll dabei ein sichtbares Problem lösen: Die zentrale Bewegung soll nicht jedes Interaktionsdetail jedes Gegenstands kennen müssen. Ein kleines vorher/nachher-Beispiel mit zwei Typen genügt als Einstieg. Die gesamte Sieben-Dateien-Referenz wird erst als späterer Endstand betrachtet.

## Die sechs Unterkapitel „Vom Problem zum Programm“

| Unterkapitel | Möglicher Arbeitsstand ohne vorzeitige Auflösung des Spiels |
|---|---|
| Die Aufgabe verstehen | Eingaben, Ausgaben, gültige Zustände und Sonderfälle an einem kleinen Auftrag festhalten. Anforderungen in eigene Worte fassen. |
| Teilaufgaben und Verantwortlichkeiten finden | Fachliche Berechnung, Datenhaltung und Ausgabe auseinanderhalten. Datenbesitz und Veränderungsrechte zunächst sprachlich beschreiben. |
| Schnittstellen vereinbaren | Benötigte Parameter, Rückgabewerte, Fehlercodes und Voraussetzungen notieren. Zwei kleine Bausteine über eine abgesprochene Schnittstelle koppeln. |
| Eine erste vollständige Version bauen | Einen kleinen Weg von der Eingabe bis zur Ausgabe vollständig umsetzen. Eine lauffähige Minimalversion und prüfbare Zwischenstände herstellen. |
| Anforderungen erweitern, Entwurf überprüfen | Eine zusätzliche Anforderung ergänzen und überlegen, welche Zuständigkeit davon betroffen ist. Gemeinsamen Code bündeln. |
| Prüfen, erklären und übergeben | Testfälle mit Erwartungen dokumentieren, Änderungen nachvollziehbar erklären, eine Anleitung für die Nutzung eines Bausteins verfassen. |

Ein „vollständiges Programm“ in diesem frühen Kapitel kann ein kleines eigenständiges Programm sein. Es muss noch nicht das Museumsspiel sein. So braucht dieses Kapitel keine noch unbekannten Pointer oder Header vorauszusetzen.

## Wie die Aufgaben zunächst unabhängig wirken können

- Entfernungsprüfung zwischen zwei Messpunkten → später Überwachungsradius.
- Gültige Position und markiertes Raster → später Karte und Bewegung.
- Veränderliche Materialsammlung → später Inventar.
- Unterschiedliche Datensätze reagieren auf denselben Verarbeitungsschritt → später Objektverhalten.
- Speicherung beschrifteter Zustandsdaten → später Spielstand.

Die Lernziele, Anforderungen und Bewertung bleiben transparent. Nur die spätere gemeinsame Anwendung wird zunächst zurückgehalten. Für alle Kapitel benötigen wir vorab konsistente Schnittstellen. Die endgültigen neutralen Namen und Übungskontexte sind noch abzustimmen. Das finale Paket benennt die Dinge offen, weil es eine Lehrendenreferenz ist.

## Was vorbereitet sein sollte

1. Gemeinsames kleines Daten- und Schnittstellenblatt, damit die Bausteine später passen.
2. Ein eigenständig ausführbarer Testtreiber pro Übungsbaustein. Bei der Integration erhält das Spiel genau ein `main`.
3. Geprüfte Referenzstände, damit ein ungelöstes frühes Kapitel den weiteren Kurs nicht blockiert.
4. Eine kleine Integrationsprobe vor dem eigentlichen Termin. Die Überraschung sollte schnell in ein funktionierendes Ergebnis münden.
5. Vergleichbare Pflichtanforderungen und ausdrücklich freiwillige Erweiterungen.

## Möglicher Zuschnitt von Lernstandscheck II

Als Kandidat eignet sich ein **einmalig betretbares Alarmfeld**: eigener Objekttyp, eigenes Zeichen und eigene Reaktion bei `betreten`. Die genaue Konsequenz und ein überschaubares Testziel werden vorab festgelegt. Dafür müssten die Studierenden bereits eine vergleichbare Typerweiterung geübt haben.

Wichtig: Das Spielstandformat der Referenz ist auf die feste Grundversion zugeschnitten. Ein neuer persistenter Objekttyp erfordert eine Formatänderung. Deshalb sollten ein neuer Typ und seine vollständige Speicherformatmigration nicht ohne Erprobung gemeinsam als Pflichtumfang angesetzt werden. Für den Check können die angepassten Serialisierungsstellen vorbereitet werden; alternativ wird das Speicherformat als eigener bereits geübter Erweiterungsauftrag gewählt. Es liegt noch keine verbindliche Prüfungsaufgabe vor.

## Grenzen und offene Abstimmung

Es gibt noch keine belastbare Zeitplanung. Der vollständige Referenzcode ist mehr Stoff als eine einzelne Übung. Insbesondere Funktionspointer, Einbettung, Listen und robustes Dateilesen brauchen jeweils eigene Lernschritte. Ob alles in den vorhandenen Terminen Platz findet, muss gegen die verbleibende Kurszeit geprüft werden. Dieses Paket ersetzt diese Entscheidung nicht.
