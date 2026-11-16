# Musterloesung: verwendet Geruest und Piktogramme aus dem Startmaterial.
START = ../materialien/tresorraub-daten
SDL2_CONFIG ?= sdl2-config
SDL_CFLAGS := $(shell $(SDL2_CONFIG) --cflags 2>/dev/null)
SDL_LIBS := $(shell $(SDL2_CONFIG) --libs 2>/dev/null)
CFLAGS ?= -std=c17 -Wall -Wextra -Wpedantic -g
EINBINDEN = -I$(START)

.PHONY: all test clean
all: programm tests

programm: $(START)/programm.c tresorraub.c $(START)/piktogramme/piktogramme.c $(START)/tresorraub.h
	$(CC) $(CFLAGS) $(SDL_CFLAGS) $(EINBINDEN) -o $@ $(START)/programm.c tresorraub.c $(START)/piktogramme/piktogramme.c $(SDL_LIBS)

tests: tests.c tresorraub.c $(START)/piktogramme/piktogramme.c $(START)/tresorraub.h
	$(CC) $(CFLAGS) $(SDL_CFLAGS) $(EINBINDEN) -o $@ tests.c $(START)/piktogramme/piktogramme.c $(SDL_LIBS)

test: tests
	./tests

clean:
	rm -f programm programm.exe tests tests.exe
