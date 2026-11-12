# Ideen für den aufbauenden Kurs zur Vorlesung Rechnerarchitektur

Hier sammeln wir Themen und Materialien für einen aufbauenden C-Kurs, der die
Vorlesung Rechnerarchitektur begleitet. Viele Ideen entstehen dort, wo der
jetzige Kurs ein Thema bewusst ausklammert, um Anfängerinnen und Anfänger
nicht zu überfordern.

## Ideensammlung

### Byte-Reihenfolge: Little Endian und Big Endian

- **Idee:** Zeigen, in welcher Reihenfolge die Bytes einer Zahl im Speicher
  liegen. Little Endian (niedrigstes Byte zuerst, so arbeiten x86-64 und
  ARM, also auch Apple Silicon) gegenüber Big Endian (höchstes Byte zuerst,
  wie man eine Zahl liest; z. B. Netzwerkprotokolle, ältere Architekturen).
- **Betroffenes Kapitel:** Darstellung von Daten im Speicher; Anknüpfung an
  „Pointer und Datenaustausch“ aus dem Grundkurs.
- **Nutzen für die Studierenden:** Verstehen, warum die Bytes einer Zahl im
  Speicher scheinbar „rückwärts“ stehen, und warum das beim Austausch von
  Binärdaten zwischen Systemen eine Rolle spielt.
- **Mögliche Umsetzung:**
  - Auf die **Speicheransicht der Pointer-Lektion** verweisen
    (`pointer/02-kopie-und-original.html`, Abschnitt 1). Ihre bytegenaue
    Darstellung (Variante A) zeigt die Zahl 20 als `14 00 00 00`, also
    Little Endian, mit Adresse und Bytewert jedes einzelnen Bytes. Damit
    lässt sich der Unterschied direkt erklären: Bei Big Endian stünde dort
    `00 00 00 14`.
  - Falls in der Pointer-Lektion nur Variante B (Werte statt Bytes) bleibt:
    `speicheransicht.js` kann die bytegenaue Darstellung weiterhin (ohne
    `data-memory-mode="werte"`). Die Ansicht lässt sich also für diesen
    Kurs wiederverwenden, eventuell mit einem Schalter zwischen Little und
    Big Endian.
  - Versuch in C: die Bytes eines `int` über einen `unsigned char *`
    ausgeben und die Reihenfolge auf dem eigenen Rechner bestimmen.
- **Offene Fragen:** Wird Hexadezimalschreibweise in der Vorlesung
  vorher eingeführt? (Im Grundkurs bisher nicht, nur Binärdarstellung auf
  der Seite „Datentypen“.)

## Vorlage für eine Idee

- **Idee:**
- **Betroffenes Kapitel:**
- **Nutzen für die Studierenden:**
- **Mögliche Umsetzung:**
- **Offene Fragen:**
