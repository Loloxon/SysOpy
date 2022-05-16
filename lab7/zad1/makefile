cc = gcc
flags = -Wall -w

all: main clean

main:
	$(cc) $(flags) cook.c -o cook.o
	$(cc) $(flags) deliverer.c -o deliver.o
	$(cc) $(flags) main.c -o main.o

clean:
	@ rm -f *.o *.so *.a

#CC=gcc
#CFLAGS=-Wall
#COOK=cook
#DELIVERER=deliverer
#MAIN=main
#VALGRIND=
#
#all: compile
#
#compile:
#	$(CC) $(CFLAGS) $(COOK).c -o $(COOK)
#	$(CC) $(CFLAGS) $(DELIVERER).c -o $(DELIVERER)
#	$(CC) $(CFLAGS) $(MAIN).c -o $(MAIN)
#
#clean:
#	rm -f *.o *.a *.so $(COOK) $(DELIVERER) $(MAIN)