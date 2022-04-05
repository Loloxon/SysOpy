cc = gcc
flags = -Wall

all: make clean

make:
	$(cc) $(flags) -o catcher.o catcher.c
	$(cc) $(flags) -o sender.o sender.c

clean:
	@rm -f *.o *.so *.a main