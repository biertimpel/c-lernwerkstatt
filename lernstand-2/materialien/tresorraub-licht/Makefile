SDL2_CONFIG ?= sdl2-config
SDL_CFLAGS := $(shell $(SDL2_CONFIG) --cflags 2>/dev/null)
SDL_LIBS := $(shell $(SDL2_CONFIG) --libs 2>/dev/null)
CFLAGS ?= -std=c17 -Wall -Wextra -Wpedantic -g

MODULE = spielfeld.c regeln.c inventar.c objekte.c spiellogik.c speicherstand.c
KOPFDATEIEN = spielfeld.h regeln.h spiel.h inventar.h objekte.h spiellogik.h \
              speicherstand.h tresorraub.h piktogramme/piktogramme.h
SPIEL = $(MODULE) tresorraub.c piktogramme/piktogramme.c

.PHONY: all test clean sdl2-pruefen
all: programm tests

programm: programm.c $(SPIEL) $(KOPFDATEIEN) | sdl2-pruefen
	$(CC) $(CFLAGS) $(SDL_CFLAGS) -I. -o $@ programm.c $(SPIEL) $(SDL_LIBS)

# Test-Suite aus dem Kapitel Spielende
tests: tests.c $(SPIEL) $(KOPFDATEIEN) | sdl2-pruefen
	$(CC) $(CFLAGS) $(SDL_CFLAGS) -I. -o $@ tests.c $(SPIEL) $(SDL_LIBS)

test: tests
	./tests

clean:
	rm -f programm programm.exe tests tests.exe

sdl2-pruefen:
	@command -v "$(SDL2_CONFIG)" >/dev/null 2>&1 || { echo "SDL2-Entwicklungspaket fehlt: sdl2-config nicht gefunden. Bitte README.md lesen." >&2; exit 1; }
	@case "$$($(SDL2_CONFIG) --version)" in 2.*) ;; *) echo "Dieses Geruest benoetigt SDL2, nicht SDL3." >&2; exit 1 ;; esac
