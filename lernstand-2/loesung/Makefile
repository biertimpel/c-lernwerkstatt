# Musterloesung: verwendet Geruest, Piktogramme und tests.c aus dem
# Startmaterial. Die Dateien hier ersetzen die gleichnamigen dort.
START = ../materialien/tresorraub-licht
SDL2_CONFIG ?= sdl2-config
SDL_CFLAGS := $(shell $(SDL2_CONFIG) --cflags 2>/dev/null)
SDL_LIBS := $(shell $(SDL2_CONFIG) --libs 2>/dev/null)
CFLAGS ?= -std=c17 -Wall -Wextra -Wpedantic -g
EINBINDEN = -I. -I$(START)
SPIEL = spielfeld.c regeln.c inventar.c objekte.c spiellogik.c speicherstand.c \
        tresorraub.c $(START)/piktogramme/piktogramme.c

.PHONY: all test clean
all: programm tests tests_licht

programm: $(SPIEL) $(START)/programm.c *.h
	$(CC) $(CFLAGS) $(SDL_CFLAGS) $(EINBINDEN) -o $@ $(START)/programm.c $(SPIEL) $(SDL_LIBS)

# tests.c wird kopiert: Sonst bindet es die Header neben sich im
# Startmaterial ein statt die geaenderten Header der Loesung.
tests_start.c: $(START)/tests.c
	cp $(START)/tests.c $@

tests: $(SPIEL) tests_start.c *.h
	$(CC) $(CFLAGS) $(SDL_CFLAGS) $(EINBINDEN) -o $@ tests_start.c $(SPIEL) $(SDL_LIBS)

tests_licht: $(SPIEL) tests_licht.c *.h
	$(CC) $(CFLAGS) $(SDL_CFLAGS) $(EINBINDEN) -o $@ tests_licht.c $(SPIEL) $(SDL_LIBS)

test: tests tests_licht
	./tests
	./tests_licht

clean:
	rm -f programm programm.exe tests tests.exe tests_licht tests_licht.exe tests_start.c
