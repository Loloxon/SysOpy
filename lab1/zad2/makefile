cc = gcc
flags = -Wall -O0

all: static shared clean

static:
	$(cc) $(flags) -c ../zad1/library.c
	ar rcs liblibrary.a library.o
	$(cc) $(flags) -o main ../zad2/main.c liblibrary.a

shared:
	$(cc) $(flags) -c -fPIC ../zad1/library.c
	$(cc) $(flags) -shared -fPIC -o library.so library.o
	$(cc) -o main main.c -L. library.so -Wl,-rpath=`pwd`

clean:
	rm -f *.o *.so *.a main

test_static:
	make clean
	make static
	./main `cat ../zad2/tests/arg_1_time.txt` >> ../zad2/raport2.txt

test_shared:
	make clean
	make shared
	./main `cat ../zad2/tests/arg_3.txt` >> ../zad2/raport2.txt


