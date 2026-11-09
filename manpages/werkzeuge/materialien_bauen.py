"""SVG-Vorschauen aus den C-Rastern ableiten und Kursarchiv erzeugen.

Vom Repository aus: python3 manpages/werkzeuge/materialien_bauen.py
Keine SDL2-Installation erforderlich. Nur generierte Vorschauen/ZIP werden
geschrieben, keine fachlichen Quelldateien oder fremden Materialien.
"""
from html import escape
from pathlib import Path
import re
from zipfile import ZipFile, ZIP_DEFLATED


def main():
    kapitel = Path(__file__).resolve().parent.parent
    material = kapitel / "materialien" / "sdl2-start"
    quelle = (material / "piktogramme" / "piktogramme.c").read_text()
    muster = re.findall(r'static const char (\w+)\[\] =\s*((?:"[^"]*"\s*)+);', quelle)
    farben = {
        code: "#{:02x}{:02x}{:02x}".format(int(r), int(g), int(b))
        for code, r, g, b in re.findall(
            r"case '(.)': return SDL_SetRenderDrawColor\(renderer, (\d+), (\d+), (\d+), 255\);",
            quelle,
        )
    }
    if len(muster) != 14 or len(farben) != 11:
        raise ValueError("Unerwartete Anzahl Motive oder Farben")
    ziel = material / "piktogramme" / "vorschau"
    ziel.mkdir(exist_ok=True)
    for name, daten in muster:
        zeilen = re.findall(r'"([^"]*)"', daten)
        if len(zeilen) != 16 or any(len(z) != 16 for z in zeilen):
            raise ValueError("Motiv muss 16 x 16 sein: " + name)
        teile = [
            '<svg xmlns="http://www.w3.org/2000/svg" width="256" height="256" viewBox="0 0 16 16" shape-rendering="crispEdges" role="img">',
            "<title>" + escape(name.replace("_", " ")) + "</title>",
            '<rect width="16" height="16" fill="#f5f6f9"/>',
        ]
        for y, zeile in enumerate(zeilen):
            for x, farbe in enumerate(zeile):
                if farbe != ".":
                    if farbe not in farben:
                        raise ValueError("Unbekannte Farbe in " + name)
                    teile.append('<rect x="{}" y="{}" width="1" height="1" fill="{}"/>'.format(x, y, farben[farbe]))
        teile.append("</svg>")
        (ziel / (name + ".svg")).write_text("\n".join(teile) + "\n")
    dateien = sorted(
        p for p in material.rglob("*")
        if p.is_file() and (p.name == "Makefile" or p.suffix in {".c", ".h", ".md", ".html", ".svg"})
        and not any(teil.startswith(".") for teil in p.relative_to(material).parts)
    )
    with ZipFile(kapitel / "materialien" / "sdl2-start.zip", "w", ZIP_DEFLATED) as archiv:
        for datei in dateien:
            archiv.write(datei, datei.relative_to(material.parent))
    print("14 SVG-Vorschauen und ZIP mit {} Dateien erzeugt.".format(len(dateien)))


if __name__ == "__main__":
    main()
