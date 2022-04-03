cc = gcc
flags = -Wall

all: main conclusions clean

main:
	printf ""  > "./raport2.txt"
	$(cc) $(flags) main.c -o main.o
	$(cc) $(flags) child.c -o child.o
	for command in ignore handler mask pending; do \
  		for mode in fork exec; do \
	  		./main.o $$mode $$command >> "./raport2.txt"; \
	  	done\
  	done

conclusions:
	@printf "\n\nW przypadku uzywania forka zarowno ignorowanie, chwytanie jak i maskowanie jest dziedziczone " >> "./raport2.txt";
	@printf "przez potomka,\n  ale sygnały oczekujące nie są dziedziczone przez potomka\n\n" >> "./raport2.txt";
	@printf "W przypadku uzywania exec'a ignorowanie, chwytanie, maskowanie jak i sygnaly oczekujące\n" >> "./raport2.txt";
	@printf "  są dziedziczone przez potomka\n\n" >> "./raport2.txt";

clean:
	rm -f *.o *.so *.a