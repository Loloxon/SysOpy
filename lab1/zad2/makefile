cc = gcc -Wall -Wno-unused-variable
#-std=c11 -ggdb

all: static clean

#main:
#	gcc -c main.c
# -L -liblibrary -o main
#	gcc main.c -L. -Wl,-rpath=. -llibrary -o main

static:
	make -C ../zad1 static
#	$(cc) -c library.c
#	ar rcs liblibrary.a library.o

#shared:
#	$(cc) -c -fPIC library.c
#	$(cc) -shared -fPIC -o library.so library.o

clean:
	rm -f *.o *.so *.a



