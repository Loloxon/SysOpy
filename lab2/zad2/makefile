cc = gcc -Wall

all: clean main_lib main_sys

main_lib:
	$(cc) mainlib.c -o mainlib.o
	@printf "Pomiar czasow lib small:\n" >> ../zad2/pomiar_zad_2.txt
	./mainlib.o ../zad2/test_small.txt ../zad2/buffor.txt >> ../zad2/pomiar_zad_2.txt
	@printf "Pomiar czasow lib big:\n" >> ../zad2/pomiar_zad_2.txt
	./mainlib.o ../zad2/test_big.txt ../zad2/buffor.txt >> ../zad2/pomiar_zad_2.txt
	@printf "________________________\n" >> ../zad2/pomiar_zad_2.txt

noargs_lib:
	$(cc) mainlib.c -o mainlib.o
	@printf "Pomiar czasow lib:\n" >> ../zad2/pomiar_zad_2.txt
	./mainlib.o >> ../zad2/pomiar_zad_2.txt

main_sys:
	$(cc) mainsys.c -o mainsys.o
	@printf "Pomiar czasow sys small:\n" >> ../zad2/pomiar_zad_2.txt
	./mainsys.o ../zad2/test_small.txt ../zad2/buffor.txt >> ../zad2/pomiar_zad_2.txt
	@printf "Pomiar czasow sys big:\n" >> ../zad2/pomiar_zad_2.txt
	./mainsys.o ../zad2/test_big.txt ../zad2/buffor.txt >> ../zad2/pomiar_zad_2.txt
	@printf "________________________\n" >> ../zad2/pomiar_zad_2.txt

noargs_sys:
	$(cc) mainsys.c -o mainsys.o
	@printf "Pomiar czasow sys:\n" >> ../zad2/pomiar_zad_2.txt
	./mainsys.o >> ../zad2/pomiar_zad_2.txt

clean:
	rm -f *.o
	@printf "" > ../zad2/pomiar_zad_2.txt
