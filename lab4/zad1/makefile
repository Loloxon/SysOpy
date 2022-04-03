cc = gcc
flags = -Wall

all: main clean

main:
	$(cc) $(flags) main.c -o main.o
	$(cc) $(flags) child.c -o child.o
	for command in ignore handler mask pending; do \
  		for mode in fork exec; do \
	  		./main.o $$mode $$command; \
	  	done\
  	done

clean:
	rm -f *.o *.so *.a