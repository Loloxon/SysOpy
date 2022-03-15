cc = gcc
flags = -Wall
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
	printf "static:\n" > ../zad3b/raport3b.txt

	for O in O0 O1 O2 O3 Ofast Os ; do \
  		printf "$$O:\n" >> ../zad3b/raport3b.txt ; \
		make static OPTIMAZATION=-$$O ; \
		make testsReports ; \
	done

	printf "shared:\n" >> ../zad3b/raport3b.txt

	for O in O0 O1 O2 O3 Ofast Os ; do \
		printf "$$O:\n" >> ../zad3b/raport3b.txt ; \
		make shared OPTIMAZATION=-$$O ; \
		make testsReports ; \
	done

testsReports:
	@printf "small test:\n" >> ../zad3b/raport3b.txt
	./main `cat $(path_tests)arg_1.txt` >> ../zad3b/raport3b.txt
	@printf "few small files:\n" >> ../zad3b/raport3b.txt
	./main `cat $(path_tests)arg_2.txt` >> ../zad3b/raport3b.txt
	@printf "some big files and deletes:\n" >> ../zad3b/raport3b.txt
	./main `cat $(path_tests)arg_3.txt` >> ../zad3b/raport3b.txt
	@printf "many big files:\n" >> ../zad3b/raport3b.txt
	./main `cat $(path_tests)arg_4.txt` >> ../zad3b/raport3b.txt
