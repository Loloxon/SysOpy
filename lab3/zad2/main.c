#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include "stdlib.h"
#include "sys/wait.h"

#include "string.h"
#include "sys/times.h"

#include "math.h"
/*
 Toszkę zmieniłem treść zadania, jako że uznałem że jest sprzeczna sama ze sobą (narzucony przedział 0-1, jednakże
 dowolność jak chodzi o liczbę procesów oraz szerokość prostokątóœ). Przyjąłem, że liczba procesów jednoznacznie
 narzuca szerokość prostokątów, a co za tym idzie dokładność obliczeń - zatem jedynym parametrem wejścia jest
 ilość procesów.
*/
clock_t timer_start, timer_end;
struct tms timer_start_tms, timer_end_tms;

void timerStart(){
    timer_start = times(&timer_start_tms);
}
double timeDiff(clock_t s, clock_t e){
    return (double)(e-s)/ (double)sysconf(_SC_CLK_TCK);
}

void printTimes(){
    timer_end = times(&timer_end_tms);
    printf("Czas rzeczywisty: %.3fs\n",
           timeDiff(timer_start,timer_end));
}

long double f(long double x){
    return 4/(x*x+1);
}

int main(int argc, char** argv){
    timerStart();
    int n = atoi(argv[1]);
    long double width = 1.0/n;
    long double x = 0.5/n;

    if(n>20000){
        printf("Too many processes! Sorry, i won't risk burning my PC\n");
        return 0;
    }

    for(int i=0;i<n;i++) {
        pid_t child_pid;
        child_pid = fork();
//        wait();
        if (child_pid == 0) {
            char fileName[32];
            snprintf(fileName,32,"../zad2/w's/w%d.txt",i+1);
            FILE* fptr = fopen(fileName,"w");
            fprintf(fptr, "%Lf",f(x)*width);
            fclose(fptr);
            exit(0);
        }
        x+= (1.0/n);
    }
    for(int i=0;i<n;i++)
        wait(NULL);

    long double ans = 0, buff;
    for(int i=0;i<n;i++){
        char fileName[32];
        snprintf(fileName,32,"../zad2/w's/w%d.txt",i+1);
        FILE* fptr = fopen(fileName,"r");
        fscanf(fptr,"%Lf",&buff);
        fclose(fptr);
        ans+=buff;
    }
    printf("Result: %Lf\n",ans);
    printf("Difference between result and pi: %Lf\n",ans-(long double)M_PI);
    printTimes();
    return 0;
}