CC = cc
CFLAGS = -std=c17 -Wall -Wextra -Wpedantic -g -O0

programm: programm.c
	$(CC) $(CFLAGS) -o $@ $<

.PHONY: clean
clean:
	rm -f programm
