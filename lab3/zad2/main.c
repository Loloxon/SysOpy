#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include "math.h"

double f(double x){
    return 4/(x*x+1);
}

int main(int argc, char** argv){

    int n = atoi(argv[1]);
    double width = 1.0/n;
    double x = 0.5/n;

    if(n>10000){
        printf("Too many processes! Sorry, i won't risk burning my PC\n");
        return 0;
    }

    for(int i=0;i<n;i++) {
//        printf("iteration: %d; middle of the part: %f\n",i,x);
        pid_t child_pid;
        child_pid = fork();
        wait();
        if (child_pid == 0) {
            char fileName[32];
            snprintf(fileName,32,"../zad2/w's/w%d.txt",i+1);
            FILE* fptr = fopen(fileName,"w");
            fprintf(fptr, "%f",f(x)*width);
            fclose(fptr);
            execvp("./child.o",NULL);
        }
        x+= (1.0/n);
    }
    double ans = 0, buff;
    for(int i=0;i<n;i++){
        char fileName[32];
        snprintf(fileName,32,"../zad2/w's/w%d.txt",i+1);
        FILE* fptr = fopen(fileName,"r");
        fscanf(fptr,"%lf",&buff);
        fclose(fptr);
        ans+=buff;
//        printf("%f\n",ans);
    }
    printf("Result: %f\n",ans);
    return 0;
}