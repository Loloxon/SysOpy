cc = gcc
flags = -Wall -O0

all: make main clean

main:
	./catcher.o

make:
	$(cc) $(flags) -o catcher.o catcher.c
	$(cc) $(flags) -o sender.o sender.c

clean:
	@rm -f *.o *.so *.a main