cc = gcc
flags = -Wall

all: main clean

main:
	$(cc) $(flags) main.c -o main.o
	./main.o ./commands.txt

clean:
	@ rm -f *.o *.so *.a main
