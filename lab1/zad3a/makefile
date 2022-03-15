cc = gcc
flags = -Wall -O0
path_libs = ../zad1/
path_main = ../zad2/main.c
path_tests = ../zad2/tests/

all: clean testsAll

static:
	$(cc) $(flags) -c $(path_libs)library.c
	ar rcs liblibrary.a library.o
	$(cc) $(flags) -o main $(path_main) liblibrary.a

shared:
	$(cc) $(flags) -c -fPIC $(path_libs)library.c
	$(cc) $(flags) -shared -fPIC -o library.so library.o
	$(cc) -o main $(path_main) -L. library.so -Wl,-rpath=`pwd`

clean:
	rm -f *.o *.so *.a main

testsAll:
	printf "static:\n" > ../zad3a/raport3a.txt
	make static
	make testsReports

	printf "shared:\n" >> ../zad3a/raport3a.txt
	make shared
	make testsReports

testsReports:
	@printf "small test:\n" >> ../zad3a/raport3a.txt
	./main `cat $(path_tests)arg_1.txt` >> ../zad3a/raport3a.txt
	@printf "few small files:\n" >> ../zad3a/raport3a.txt
	./main `cat $(path_tests)arg_2.txt` >> ../zad3a/raport3a.txt
	@printf "some big files and deletes:\n" >> ../zad3a/raport3a.txt
	./main `cat $(path_tests)arg_3.txt` >> ../zad3a/raport3a.txt
	@printf "many big files:\n" >> ../zad3a/raport3a.txt
	./main `cat $(path_tests)arg_4.txt` >> ../zad3a/raport3a.txt
