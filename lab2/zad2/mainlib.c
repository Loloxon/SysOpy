#include "stdio.h"
#include "string.h"
#include "stdlib.h"
#include <stdbool.h>

#include "sys/times.h"
#include "unistd.h"

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


int main(int argc, char** argv){
    if(argc>3){
        printf("Too many arguments\n");
    }
    else if(argc<3){
        printf("You didnt give enough!!!\n");
    }
    else{
        char znak = argv[1][0];
        char* path = calloc(256, 1);
        strcpy(path,argv[2]);
        timerStart();
        FILE* fOut = fopen(path,"r");
        fseek(fOut, 0, SEEK_END);
        long size = ftell(fOut);
        fseek(fOut, 0, SEEK_SET);
        rewind(fOut);
        char content[size];
        fread(content, 1, size, fOut);
        fclose(fOut);
        bool exists = false;
        long counter1=0;
        long counter2=0;
        for(int i=0;i<size+1;i++){
            if(content[i]==znak){
                counter1++;
            }
            if(content[i]==znak){
                exists = true;
            }
            if(content[i] == '\n' && exists){
                counter2++;
                exists = false;
            }
        }
        printf("The \"%c\" char exists %ld times in file\n",znak,counter1);
        printf("The \"%c\" char exists in %ld lines in file\n",znak,counter2);
        printTimes();
        free(path);
    }
    return 0;
}
