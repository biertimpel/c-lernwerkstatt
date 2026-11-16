# Makefile fuer Teil 2: das Spiel in Modulen.
# Geruest, Piktogramme und Vergleichslauf liegen eine Ebene hoeher.
OBEN = ..
SDL2_CONFIG ?= sdl2-config
SDL_CFLAGS := $(shell $(SDL2_CONFIG) --cflags 2>/dev/null)
SDL_LIBS := $(shell $(SDL2_CONFIG) --libs 2>/dev/null)
CFLAGS ?= -std=c17 -Wall -Wextra -Wpedantic -g
EINBINDEN = -I. -I$(OBEN)

MODULE = spielfeld.c regeln.c inventar.c objekte.c spiellogik.c speicherstand.c
KOPFDATEIEN = spielfeld.h regeln.h spiel.h inventar.h objekte.h spiellogik.h \
              speicherstand.h $(OBEN)/tresorraub.h $(OBEN)/piktogramme/piktogramme.h
SPIEL = $(MODULE) tresorraub.c $(OBEN)/piktogramme/piktogramme.c

.PHONY: all test clean sdl2-pruefen
all: programm ablauf tests

programm: $(OBEN)/programm.c $(SPIEL) $(KOPFDATEIEN) | sdl2-pruefen
	$(CC) $(CFLAGS) $(SDL_CFLAGS) $(EINBINDEN) -o $@ $(OBEN)/programm.c $(SPIEL) $(SDL_LIBS)

# Vergleichslauf wie in Teil 1
ablauf: $(OBEN)/ablauf.c $(SPIEL) $(KOPFDATEIEN) | sdl2-pruefen
	$(CC) $(CFLAGS) $(SDL_CFLAGS) $(EINBINDEN) -o $@ $(OBEN)/ablauf.c $(SPIEL) $(SDL_LIBS)

# Test-Suite: prueft jedes Modul ueber seinen Header
tests: tests.c $(SPIEL) $(KOPFDATEIEN) | sdl2-pruefen
	$(CC) $(CFLAGS) $(SDL_CFLAGS) $(EINBINDEN) -o $@ tests.c $(SPIEL) $(SDL_LIBS)

test: tests
	./tests

clean:
	rm -f programm programm.exe ablauf ablauf.exe tests tests.exe

sdl2-pruefen:
	@command -v "$(SDL2_CONFIG)" >/dev/null 2>&1 || { echo "SDL2-Entwicklungspaket fehlt: sdl2-config nicht gefunden. Bitte ../README.md lesen." >&2; exit 1; }
	@case "$$($(SDL2_CONFIG) --version)" in 2.*) ;; *) echo "Dieses Geruest benoetigt SDL2, nicht SDL3." >&2; exit 1 ;; esac
