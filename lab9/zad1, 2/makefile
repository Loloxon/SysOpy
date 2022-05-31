cc = gcc
flags = -Wall -w
threads = -lpthread


all: main clean

main:
	$(cc) $(flags) main.c -o main.o $(threads)
	./main.o

clean:
	@ rm -f *.o *.so *.a main
