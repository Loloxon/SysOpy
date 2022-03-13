cc = gcc -Wall -std=c11 -ggdb

all: static shared clean

static:
	$(cc) -c library.c
	ar rcs liblibrary.a library.o

shared:
	$(cc) -c -fPIC library.c
	$(cc) -shared -fPIC -o library.so library.o

clean:
	rm -f *.o *.so *.a



