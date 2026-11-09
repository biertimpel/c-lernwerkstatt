# Prüfung der SDL2-Lektion

## Ausgeführt

- SDL2 wurde ausschließlich in einem temporären Verzeichnis gebaut; keine
  Installation oder Änderung der Systembibliotheken. Verwendet wurde der
  SDL2-Entwicklungszweig mit gemeldeter Version 2.33.0, nicht SDL3.
- Das Fenstergerüst und die Piktogramm-Beispiele wurden unter macOS mit
  `-std=c17 -Wall -Wextra -Wpedantic -Werror` übersetzt und gelinkt.
- Programmstarts mit `--version`, `--testlauf`, `--galerie --testlauf`,
  `--diagnose --testlauf`, unbekannter Option und `--hilfe` geprüft.
- Reguläres Schließen über ein eingespeistes SDL_QUIT-Ereignis,
  Weitergabe eines Fehlers aus dem studentischen Zeichenbereich und die
  Übernahme eines Piktogramm-Aufrufs geprüft. Die Fensterläufe verwendeten
  den SDL-Dummy-Videotreiber und öffneten keine sichtbaren Desktopfenster.
- Alle 14 Motive mit Skalierungen 1, 4 und 16 in echte SDL-Softwareflächen
  gezeichnet. 10752 gerenderte Rasterzellen mit ihren erwarteten Farben
  verglichen; transparente Zellen erhalten die Hintergrundfarbe.
- Für jedes Motiv fünf ungültige Parameterfälle geprüft, insgesamt 70:
  fehlender Renderer, Skalierung 0 beziehungsweise 17 und Koordinaten
  außerhalb der vereinbarten Grenzen.
- Die Pixel- und Parameterprüfungen bestanden auch mit
  UndefinedBehaviorSanitizer. Ein gesonderter AddressSanitizer-Lauf blieb
  in dieser Umgebung in einem Timeout; daraus wird kein bestandener
  AddressSanitizer- oder Leak-Test abgeleitet.
- Eine Galerie aus dem tatsächlichen SDL-Rendering exportiert und visuell
  kontrolliert. Die Beute ist ein eigenes Motiv, getrennt von der Münze.
- Beide MC-Fragen mit allen 24 Auswahlkombinationen, leerer Auswahl und
  Zurücksetzen der Rückmeldung geprüft.
- Lokale HTML-Links, Abschnittsziele, Bildbeschreibungen und SVG-Struktur
  geprüft. SVG-Vorschauen und ZIP werden aus den C-Rastern erzeugt.

- Überarbeitung vom 09.11.2026: `programm.c` mit nur noch einer
  Aufräummarke `ende`, Deklarationen vor dem ersten `goto` und
  reihenfolgeunabhängiger Optionsauswertung. Mit SDL 2.30.0 unter Linux
  mit `-std=c17 -Wall -Wextra -Wpedantic -Werror -Wjump-misses-init`
  übersetzt; `--testlauf`, `--galerie --testlauf`, `--diagnose --testlauf`
  (Status 1), `--version`, `--hilfe`, unbekannte Optionen in beliebiger
  Reihenfolge (Status 2) und die Weitergabe eines Fehlers aus `zeichne`
  mit dem Dummy-Videotreiber geprüft. ZIP neu erzeugt.

- Desktop-Sichtprüfung der Lektion im Browser sowie Fensterziehen,
  Minimieren, Escape und Schließen mit der Maus am 09.11.2026 durchgeführt;
  keine Auffälligkeiten.

## Selbst nachprüfen

Das Material entpacken, dann bei eingerichteter SDL2-Entwicklungsumgebung:

```sh
make
./programm --version
./programm --galerie
./programm --diagnose
echo $?
```

Bei vorhandenem Dummy-Videotreiber sind selbständig endende Läufe möglich:

```sh
SDL_VIDEODRIVER=dummy ./programm --testlauf
SDL_VIDEODRIVER=dummy ./programm --galerie --testlauf
SDL_VIDEODRIVER=dummy ./programm --diagnose --testlauf
```

Die ersten beiden Läufe erwarten Status 0, der Diagnoseversuch Status 1
und die ausdrücklich simulierte Fehlermeldung. Ein Fehler schon beim
Fensteraufbau muss von diesem Diagnoseversuch unterschieden werden.

Nach Änderungen an den Rasterdaten aus dem Repository heraus ausführen:

```sh
python3 manpages/werkzeuge/materialien_bauen.py
```

Das Werkzeug prüft Rastermaße und Farbzeichen und erneuert die SVG-Dateien
und das ZIP. Es verpackt weder erzeugte Programme noch Editor-Metadaten.

## Noch praktisch zu prüfen

Linux- und Windows-Einrichtungshinweise sind vorbereitet,
auf tatsächlichen Rechnern dieser Systeme aber hier nicht erprobt.

Der optionale Motivcode verwendet intern Arrays und Pointer. Diese Technik
ist vorbereitete Infrastruktur, kein vorgezogener Pflichtstoff. Die
Lernleistung ist API-Recherche, begründete Verwendung und Fehlerbehandlung,
nicht das Nachbauen einer identischen Grafik.
