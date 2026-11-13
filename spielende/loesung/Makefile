# Musterloesung: verwendet Geruest, Piktogramme, ablauf.c, Header und
# tests.c aus dem Startmaterial.
START = ../materialien/tresorraub-ende
SDL2_CONFIG ?= sdl2-config
SDL_CFLAGS := $(shell $(SDL2_CONFIG) --cflags 2>/dev/null)
SDL_LIBS := $(shell $(SDL2_CONFIG) --libs 2>/dev/null)
CFLAGS ?= -std=c17 -Wall -Wextra -Wpedantic -g
PIKTO = $(START)/piktogramme/piktogramme.c
MODULE = $(addprefix module/,spielfeld.c regeln.c inventar.c objekte.c \
         spiellogik.c speicherstand.c tresorraub.c)
EIN_MODULE = -Imodule -I$(START)/module -I$(START)

.PHONY: all test vergleich clean
all: programm-teil1 ablauf-teil1 programm ablauf tests

# Teil 1: eine Datei
programm-teil1: teil1/tresorraub.c $(START)/programm.c $(PIKTO)
	$(CC) $(CFLAGS) $(SDL_CFLAGS) -I$(START) -o $@ $(START)/programm.c teil1/tresorraub.c $(PIKTO) $(SDL_LIBS)

ablauf-teil1: teil1/tresorraub.c $(START)/ablauf.c $(PIKTO)
	$(CC) $(CFLAGS) $(SDL_CFLAGS) -I$(START) -o $@ $(START)/ablauf.c teil1/tresorraub.c $(PIKTO) $(SDL_LIBS)

# Teil 2: Module
programm: $(MODULE) $(START)/programm.c $(PIKTO)
	$(CC) $(CFLAGS) $(SDL_CFLAGS) $(EIN_MODULE) -o $@ $(START)/programm.c $(MODULE) $(PIKTO) $(SDL_LIBS)

ablauf: $(MODULE) $(START)/ablauf.c $(PIKTO)
	$(CC) $(CFLAGS) $(SDL_CFLAGS) $(EIN_MODULE) -o $@ $(START)/ablauf.c $(MODULE) $(PIKTO) $(SDL_LIBS)

tests: $(MODULE) $(START)/module/tests.c $(PIKTO)
	$(CC) $(CFLAGS) $(SDL_CFLAGS) $(EIN_MODULE) -o $@ $(START)/module/tests.c $(MODULE) $(PIKTO) $(SDL_LIBS)

test: tests
	./tests

# Teil 2 darf das Verhalten von Teil 1 nicht aendern
vergleich: ablauf-teil1 ablauf
	cp $(START)/regeln.txt .
	./ablauf-teil1 > teil1.txt
	./ablauf > module.txt
	diff teil1.txt module.txt && echo "Vergleichslauf: gleich"

clean:
	rm -f programm-teil1 ablauf-teil1 programm ablauf tests *.exe teil1.txt module.txt spielstand.txt regeln.txt
