# Vorschlag zur gemeinsamen Abstimmung

## Worum es geht

Die Idee ist, Block 2 über ein gemeinsames kleines Endprodukt zu verbinden. Zunächst werden verständliche, für sich ausführbare Aufgaben bearbeitet. Die Bausteine erhalten vorab passende Schnittstellen. Beim Kapitel über Header und Quelldateien werden sie zusammengeführt: Aus den Einzelaufgaben entsteht die Grundversion von Tresorraub.

Der Prototyp dient dazu, das Endprodukt schon einmal spielen und seine Größenordnung einschätzen zu können. Jonathan und Larissa können anschließend für ihre jeweiligen Kapitel selbst festlegen, wie die passenden Konzepte erarbeitet werden. Fertige Kapitelaufgaben sind bewusst noch nicht ausgearbeitet.

Das Beispiel ist ein unverbindlicher Vorschlag. Es gibt keine Entscheidung, dass dieses Spiel verbindlich für alle Kapitel oder spätere Gruppenprojekte eingesetzt werden muss.

## Warum dieses Spiel

Der Kern ist ein kleines Wegfindungsrätsel ohne Echtzeitdruck. Eine feste Karte, wenige Gegenstände und unbewegliche Wächter begrenzen die Komplexität. Trotzdem gibt es fachliche Anknüpfungspunkte für Arrays, Zeichenketten, Strukturen, Pointer, dynamische Sammlungen, Lebensdauer, Objektverhalten, Header, Makefiles und explizites Speichern und Laden.

Die vollständige Referenz ist bereits der späte Endstand. Sie soll nicht am Anfang als großes unbekanntes Programm ausgeteilt werden. Vor allem robuste Eingabeverarbeitung und das vollständige Ladeprogramm können als bereitgestellte Infrastruktur dienen, während die Studierenden zunächst kleine Datenbestände selbst speichern und lesen.

## Gestaltungsspielraum erhalten

- Ein gemeinsamer Mindestvertrag beschreibt, was ein Baustein leisten muss. Die konkrete Implementierung, weitere Hilfsfunktionen und Tests bleiben eigenständige Entscheidungen.
- Bei der Anzeige, Gestaltung der Karte, Namen und freiwilligen Objekten gibt es Spielraum. Für die erste Integration braucht es eine gemeinsame Referenzkonfiguration.
- Studierende können funktionierende eigene Bausteine einsetzen oder an einer geprüften Referenz weiterarbeiten. Frühere Schwierigkeiten blockieren keine späteren Kapitel.
- Die Erweiterung im Lernstandscheck nutzt vertraute Mechanismen in einer neuen Anforderung. Sie sollte vorher vergleichbar geübt und im Umfang erprobt werden.
- Die Entscheidung über freie spätere Gruppenprojekte bleibt eigenständig. Das gemeinsame Übungsspiel kann ein möglicher Anknüpfungspunkt sein.

## Risiken, die wir gemeinsam prüfen sollten

1. Werden die Schnittstellen so genau vorgegeben, dass zu wenig eigener Entwurf übrig bleibt? Wir sollten nur die tatsächlich gemeinsam benötigten Schnittstellen festlegen.
2. Verbraucht die Integration mehr Zeit als das eigentliche Fachthema? Ein kleiner getesteter Integrationsstand und Testtreiber pro Baustein helfen.
3. Wird aus dem überraschenden Zusammenhang eine Abhängigkeit von jeder vorherigen Lösung? Dafür brauchen wir Referenzstände.
4. Passen OOP und Datei-Ein-/Ausgabe in die verbleibenden Termine? Beides braucht eigene Lernziele. Dateioperationen sind in der bisherigen Überschriftenliste noch nicht ausdrücklich enthalten.
5. Wie viel vom robusten finalen Referenzcode sollen Anfänger selbst schreiben? Nicht jede technische Absicherung muss gleichzeitig eigenständige Pflichtleistung sein.

## Vorschlag für die nächste Besprechung

- Gefällt uns dieses Endprodukt und die Idee der späteren Zusammenführung?
- Welche Kapitel übernimmt wer, und welche Schnittstellen werden verbindlich?
- Wie klein muss die erste spielbare Version bleiben?
- Wo bekommt Datei-Ein-/Ausgabe einen ausdrücklichen Platz?
- Was ist ein passender, vorher erprobbarer Erweiterungsumfang für Lernstandscheck II?

Der bereits entworfene Lernstandscheck I zum dreistelligen Codeschloss gehört weiterhin zu Block 1 und ist unabhängig von dieser Entscheidung.
