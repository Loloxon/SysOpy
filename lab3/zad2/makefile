cc = gcc -Wall

all: clean main

main:
	printf ""  > "../zad2/raport.txt"
	$(cc) main.c -o main.o
	for i in 1 2 5 10 50 100 500 1000 5000 20000 ; do \
  		printf "Done for $$i processes:\n" >> ../zad2/raport.txt; \
		./main.o $$i >> ../zad2/raport.txt; \
 		printf "\n" >> ../zad2/raport.txt; \
	done
#	@printf("")
	#./main.o 10 >> ../zad2/raport.txt

clean:
	rm -f *.o
