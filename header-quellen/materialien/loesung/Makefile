rechner: main.o calc.o
	gcc main.o calc.o -o rechner

main.o: main.c calc.h
	gcc -c main.c -o main.o

calc.o: calc.c calc.h
	gcc -c calc.c -o calc.o
