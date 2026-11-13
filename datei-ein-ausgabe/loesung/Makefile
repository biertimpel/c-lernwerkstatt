START = ../materialien/tresorraub-dateien
SDL2_CONFIG ?= sdl2-config
SDL_CFLAGS := $(shell $(SDL2_CONFIG) --cflags)
SDL_LIBS := $(shell $(SDL2_CONFIG) --libs)
CFLAGS ?= -std=c17 -Wall -Wextra -Wpedantic -Werror -g
.PHONY: all test clean
all: programm tests
programm: tresorraub.c $(START)/programm.c $(START)/tresorraub.h $(START)/piktogramme/piktogramme.c
	$(CC) $(CFLAGS) $(SDL_CFLAGS) -I$(START) -o $@ $(START)/programm.c tresorraub.c $(START)/piktogramme/piktogramme.c $(SDL_LIBS)
tests: tests.c tresorraub.c $(START)/tresorraub.h $(START)/piktogramme/piktogramme.c
	$(CC) $(CFLAGS) $(SDL_CFLAGS) -I$(START) -o $@ tests.c $(START)/piktogramme/piktogramme.c $(SDL_LIBS)
test: tests
	SDL_VIDEODRIVER=dummy ./tests
clean:
	rm -f programm programm.exe tests tests.exe
