cc = gcc -Wall

all:
	@printf "" > ../zad1/pomiar_zad_1.txt
	make clean
	make main_lib
	make main_sys

main_lib:
	$(cc) mainlib.c -o mainlib.o
	@printf "		Lib small:\n" >> ../zad2/pomiar_zad_2.txt
	./mainlib.o  c ../zad1/test_small.txt >> ../zad2/pomiar_zad_2.txt
	@printf "		Lib big:\n" >> ../zad2/pomiar_zad_2.txt
	./mainlib.o c ../zad1/test_big.txt >> ../zad2/pomiar_zad_2.txt
	@printf "________________________\n" >> ../zad2/pomiar_zad_2.txt

main_sys:
	$(cc) mainsys.c -o mainsys.o
	@printf "		Sys small:\n" >> ../zad2/pomiar_zad_2.txt
	./mainsys.o c ../zad1/test_small.txt >> ../zad2/pomiar_zad_2.txt
	@printf "		Sys big:\n" >> ../zad2/pomiar_zad_2.txt
	./mainsys.o c ../zad1/test_big.txt >> ../zad2/pomiar_zad_2.txt
	@printf "________________________\n" >> ../zad2/pomiar_zad_2.txt

clean:
	rm -f *.o
