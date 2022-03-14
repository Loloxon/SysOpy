cc = gcc
flags = -Wall -O0

all: static shared

static:
	$(cc) $(flags) -c ../zad1/library.c
	ar rcs liblibrary.a library.o
	$(cc) $(flags) -o main ../zad2/main.c liblibrary.a
	./main `cat ../zad2/tests/arg_1.txt`

shared:
	$(cc) $(flags) -c -fPIC ../zad1/library.c
	$(cc) $(flags) -shared -fPIC -o library.so library.o
	$(cc) -o main main.c -L. library.so -Wl,-rpath=`pwd`

clean:
	rm -f *.o *.so *.a main *.txt

test_static:
	make clean
	make static

test_shared:
	make clean
	make shared

