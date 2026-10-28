CC = cc
CPPFLAGS = -Iinclude
CFLAGS = -std=c17 -Wall -Wextra -Wpedantic -Werror -g
SOURCES = src/spiel.c src/objekt.c src/inventar.c src/eingabe.c src/speichern.c src/anzeige.c
OBJECTS = $(SOURCES:src/%.c=build/%.o)

.PHONY: all test test-alloc clean sanitize
all: tresorraub

tresorraub: $(OBJECTS) build/main.o
	$(CC) $(CFLAGS) $^ -o $@

build/%.o: src/%.c
	@mkdir -p build
	$(CC) $(CPPFLAGS) $(CFLAGS) -MMD -MP -c $< -o $@

build/test_spiel: tests/test_spiel.c $(OBJECTS)
	$(CC) $(CPPFLAGS) $(CFLAGS) $^ -o $@

test: build/test_spiel
	./build/test_spiel

test-alloc:
	@mkdir -p build
	$(CC) $(CPPFLAGS) $(CFLAGS) -DTEST_ALLOC -include tests/alloc_probe.h tests/test_spiel.c tests/alloc_probe.c $(SOURCES) -o build/test_alloc
	./build/test_alloc

sanitize:
	@mkdir -p build
	$(CC) $(CPPFLAGS) $(CFLAGS) -fsanitize=address,undefined -fno-omit-frame-pointer tests/test_spiel.c $(SOURCES) -o build/test_sanitize
	./build/test_sanitize

clean:
	rm -rf build tresorraub

-include $(OBJECTS:.o=.d) build/main.d
