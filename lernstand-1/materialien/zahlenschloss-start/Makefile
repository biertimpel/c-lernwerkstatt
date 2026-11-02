CC = cc
CFLAGS = -std=c17 -Wall -Wextra -Wpedantic -g
PYTHON = python3
.PHONY: all run test clean
all: tresor
tresor: tresor.c
	$(CC) $(CFLAGS) $< -o $@
run: tresor
	./tresor
test: tresor
	$(PYTHON) test_tresor.py ./tresor
clean:
	rm -f tresor tresor.exe
