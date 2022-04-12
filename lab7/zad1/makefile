cc = gcc
flags = -Wall -w

all: main clean

main:
	$(cc) $(flags) main.c -o main.o
	./main.o ./commands.txt

clean:
	@ rm -f *.o *.so *.a main
