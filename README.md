# C-Programmierkurs – erster Pages-Entwurf

Kleines, eigenständiges Gerüst mit **Aufgabe 1A (Hello World)** und
**Aufgabe 1B (vier Grundrechenarten)**. Weitere Aufgaben, Kapitel,
Semesterplanung und Bewertungsregeln sind noch nicht enthalten.

## Direkt ausprobieren

`index.html` im Browser öffnen. Es gibt keine Installation, externen
Bibliotheken oder einen Build-Schritt. Die ganze Seite steckt in dieser Datei.

## Auf GitHub Pages veröffentlichen

1. Ein öffentliches Repository, zum Beispiel `c-kurs`, anlegen.
2. `index.html`, `.nojekyll` und diese `README.md` in den Hauptordner hochladen.
3. Unter **Settings → Pages → Build and deployment** die Quelle
   **Deploy from a branch** auswählen.
4. Branch **main** und Ordner **/(root)** auswählen und speichern.
5. Nach erfolgreicher Veröffentlichung die von GitHub angezeigte Website-Adresse öffnen.

Bei einem anderen Standardbranch diesen statt `main` wählen.
Die GitHub-Anleitung: https://docs.github.com/en/pages/getting-started-with-github-pages/configuring-a-publishing-source-for-your-github-pages-site

## Was die Vorschau macht

Die Vorschau ist ausdrücklich **eine begrenzte Simulation, kein C-Compiler**.
Sie liest das kleine Gerüst, erzeugt dessen simulierte Ausgabe und vergleicht
die Ausgabe mit dem Ziel der Aufgabe. Unterschiedliche unterstützte
Schreibweisen können damit zum gleichen richtigen Ergebnis führen.

Unterstützt werden `#include <stdio.h>`, `int main(void)` bzw. `int main()`,
initialisierte `int`-Variablen, Zuweisungen, dezimale Ganzzahlen, Klammern,
`+`, `-`, `*`, `/`, `%`, `printf` mit einfachen Texten, `%d` und `%%`,
die Escape-Sequenzen `\n`, `\r`, `\t`, `\\`, `\"`, Kommentare und `return`.
Die Simulation begrenzt Zahlen auf vorzeichenbehaftete 32-Bit-Werte.
Ganzzahldivision schneidet Richtung null ab. Division durch null und
Überläufe ergeben Hinweise statt einer Ausgabe.

Gültiger C-Code außerhalb dieses kleinen Umfangs kann von der Vorschau
abgelehnt werden. Daher ist die Rückmeldung kein vollständiger Nachweis,
dass ein Programm in C korrekt ist. Ein echter Compiler kann später
eingebunden werden, wenn wir uns dafür entscheiden.

Der eingegebene Code bleibt im aktuellen Browserfenster. Er wird nicht
übertragen, dauerhaft gespeichert oder zwischen Personen synchronisiert.
Änderungen an der Website können gemeinsam im Repository bearbeitet und
über Issues oder Pull Requests besprochen werden.

## Dateien

- `index.html`: Seitenstruktur, Gestaltung, Aufgabe 1 und Vorschau.
- `.nojekyll`: veröffentlicht die statische Seite ohne Jekyll-Verarbeitung.
- `README.md`: kurze Anleitung für den Entwurf.
