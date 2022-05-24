cc = gcc
flags = -Wall
threads = -lpthread

all: main clean

main:
	echo "Times of program" > times.txt
	$(cc) $(flags) main.c -o main.o $(threads)
	for numbers in 1 2 4 8 16; do \
		echo "***		THREADS NUMBER: " $$numbers " 	***" >> times.txt; \
		for mode in "numbers" "blocks"; do \
          	echo "MODE: " $$mode >> times.txt; \
			./main.o $$numbers $$mode "pepper.ascii.pgm" "pepper_reversed.ascii.pgm" >> times.txt; \
		done \
	done

clean:
	@ rm -f *.o *.so *.a main
