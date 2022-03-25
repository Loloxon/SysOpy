cc = gcc -Wall

all: clean main

main:
	$(cc) main.c -o main.o
	$(cc) child.c -o child.o
	./main.o 10

clean:
	rm -f *.o
