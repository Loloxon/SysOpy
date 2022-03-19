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
    printf("rzeczywisty: %.3fs użytkownika: %.3fs systemowy: %.3fs\n",
           timeDiff(timer_start,timer_end),
           timeDiff(timer_start_tms.tms_cutime,timer_end_tms.tms_cutime),
           timeDiff(timer_start_tms.tms_cstime,timer_end_tms.tms_cstime));
}


int main(int argc, char** argv){
    if(argc>3){
        printf("Too many arguments\n");
    }
    else if(argc==2){
        printf("Cant get just one file >:(\n");
    }
    else{
        char* pathOut = calloc(156, 1);
        char* pathIn = calloc(256, 1);
        if(argc<3){
            printf("Gimme name of files bruddah!\n");
            scanf("%s256",pathIn);
            scanf("%s256",pathOut);
        }
        else{
            strcpy(pathOut,argv[1]);
            strcpy(pathIn,argv[2]);
        }
        timerStart();
        FILE* fOut = fopen(pathOut,"r");
        fseek(fOut, 0, SEEK_END);
        long size = ftell(fOut);
        fseek(fOut, 0, SEEK_SET);
        rewind(fOut);
        char content[size];
        fread(content, 1, size, fOut);
        fclose(fOut);

        char* newContent = calloc(size,1);

        int lineP = 0;
        bool toDelete = true;
        int p=0;
        for(int i=0;i<size+1;i++){
            if(content[i]!='\n' && content[i]!=' ' && content[i]!='\t'){
                toDelete = false;
            }
            if(!toDelete) {
                for (;lineP <= i; lineP++) {
                    newContent[p]=content[lineP];
                    p+=1;
                }
                toDelete = true;
            }
            if(content[i]=='\n') {
                if(p==0)
                    lineP=i+1;
                else
                   lineP = i;
            }
        }

        FILE* fIn = fopen(pathIn, "w");
        fwrite(newContent, 1, p-1, fIn);
        fclose(fIn);
        printTimes();
        free(pathIn);
        free(pathOut);
        free(newContent);
    }
    return 0;
}
