cc = gcc -Wall

all:
	@printf "" > ../zad3/pomiar_zad_3.txt
	make clean
	make main_dir

main_dir:
	$(cc) maindir.c -o maindir.o
	./maindir.o  ../zad3 >> ../zad3/pomiar_zad_3.txt
	@printf "________________________\n" >> ../zad3/pomiar_zad_3.txt

clean:
	rm -f *.o
