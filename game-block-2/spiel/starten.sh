#!/bin/sh
set -eu
cd "$(dirname "$0")"
if command -v cc >/dev/null 2>&1; then compiler=cc
elif command -v gcc >/dev/null 2>&1; then compiler=gcc
elif command -v clang >/dev/null 2>&1; then compiler=clang
else
  echo "Kein C-Compiler gefunden. Alternativ START_HIER.html fuer die Browser-Vorschau oeffnen."
  exit 1
fi
"$compiler" -std=c17 -Wall -Wextra -Wpedantic -Iinclude src/main.c src/spiel.c src/objekt.c src/inventar.c src/eingabe.c src/speichern.c src/anzeige.c -o tresorraub
./tresorraub
