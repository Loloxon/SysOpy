cc = gcc
flags = -Wall -w

all: clean main

main:
	$(cc) $(flags) cook.c -o cook
	$(cc) $(flags) deliverer.c -o deliver
	$(cc) $(flags) main.c -o main
	#./main 5 5

clean:
	@ rm -f *.o *.so *.a cook deliver main

