cc = gcc -Wall

all: clean main_lib main_sys

main_lib:
	$(cc) mainlib.c -o mainlib.o
	@printf "Pomiar czasow lib small:\n" >> ../zad1/pomiar_zad_1.txt
	./mainlib.o ../zad1/test_small.txt ../zad1/buffor.txt >> ../zad1/pomiar_zad_1.txt
	@printf "Pomiar czasow lib big:\n" >> ../zad1/pomiar_zad_1.txt
	./mainlib.o ../zad1/test_big.txt ../zad1/buffor.txt >> ../zad1/pomiar_zad_1.txt
	@printf "________________________\n" >> ../zad1/pomiar_zad_1.txt

noargs_lib:
	$(cc) mainlib.c -o mainlib.o
	@printf "Pomiar czasow lib:\n" >> ../zad1/pomiar_zad_1.txt
	./mainlib.o >> ../zad1/pomiar_zad_1.txt

main_sys:
	$(cc) mainsys.c -o mainsys.o
	@printf "Pomiar czasow sys small:\n" >> ../zad1/pomiar_zad_1.txt
	./mainsys.o ../zad1/test_small.txt ../zad1/buffor.txt >> ../zad1/pomiar_zad_1.txt
	@printf "Pomiar czasow sys big:\n" >> ../zad1/pomiar_zad_1.txt
	./mainsys.o ../zad1/test_big.txt ../zad1/buffor.txt >> ../zad1/pomiar_zad_1.txt
	@printf "________________________\n" >> ../zad1/pomiar_zad_1.txt

noargs_sys:
	$(cc) mainsys.c -o mainsys.o
	@printf "Pomiar czasow sys:\n" >> ../zad1/pomiar_zad_1.txt
	./mainsys.o >> ../zad1/pomiar_zad_1.txt

clean:
	rm -f *.o
	@printf "" > ../zad1/pomiar_zad_1.txt
