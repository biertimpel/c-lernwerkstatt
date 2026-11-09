SDL2_CONFIG ?= sdl2-config
SDL_CFLAGS := $(shell $(SDL2_CONFIG) --cflags 2>/dev/null)
SDL_LIBS := $(shell $(SDL2_CONFIG) --libs 2>/dev/null)
CFLAGS ?= -std=c17 -Wall -Wextra -Wpedantic

.PHONY: all sdl2-pruefen
all: programm

programm: programm.c zeichnung.c zeichnung.h piktogramme/piktogramme.c piktogramme/piktogramme.h | sdl2-pruefen
	$(CC) $(CFLAGS) $(SDL_CFLAGS) -o $@ programm.c zeichnung.c piktogramme/piktogramme.c $(SDL_LIBS)

sdl2-pruefen:
	@command -v "$(SDL2_CONFIG)" >/dev/null 2>&1 || { echo "SDL2-Entwicklungspaket fehlt: sdl2-config nicht gefunden. Bitte README.md lesen." >&2; exit 1; }
	@case "$$($(SDL2_CONFIG) --version)" in 2.*) ;; *) echo "Dieses Geruest benoetigt SDL2, nicht SDL3." >&2; exit 1 ;; esac
