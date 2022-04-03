#include "stdio.h"
#include "string.h"
#include "stdlib.h"

#include "sys/times.h"
#include "unistd.h"
#include "time.h"

#include "signal.h"

int checkMode(char* command){
    if(!strcmp(command,"ignore")){
        return 0;
    }
    if(!strcmp(command,"handler")){
        return 1;
    }
    if(!strcmp(command,"mask")){
        return 2;
    }
    if(!strcmp(command,"pending")){
        return 3;
    }
    return -1;
}

void handler(int sign){
    printf("Handled!\n");
}

int main(int argc, char** argv){
    if(argc!=3){
        printf("Invalid no. of arguments!\n");
        return 1;
    }
    printf("_____________________________________\n");
    printf("----> For: %s. Command: %s.\n", argv[1], argv[2]);
    int mode = checkMode(argv[2]);
    switch(mode){ // 0-ignore, 1-handler, 2-mask, 3-pending
        case 0:
            signal(SIGUSR1, SIG_IGN);
            raise(SIGUSR1);
            printf("Ignored :|\n");
            break;
        case 1:
            signal(SIGUSR1, handler);
            raise(SIGUSR1);
            break;
        case 2:
        case 3:
            ;sigset_t newmask;
            sigemptyset(&newmask);
            sigaddset(&newmask, SIGUSR1);
            if(sigprocmask(SIG_BLOCK,&newmask, NULL)<0)
                perror("Cant block signal!");
            raise(SIGUSR1);
            if(mode==3){
                sigset_t currsigs;
                sigpending(&currsigs);
                printf("Pending... (in parent? %s)\n", sigismember(&currsigs, SIGUSR1)?"yes":"no");
            }
            else{
                printf("Masked xO\n");
            }
            break;
        default:
            printf("Invalid command!\n");
            return 1;
    }
    if(!strcmp(argv[1],"exec")){
        execl("./child.o", "./child.o", argv[2], NULL);
    }
    else if(!strcmp(argv[1],"fork")){
        if(fork()==0){
            switch(mode){
                case 0:
                    raise(SIGUSR1);
                    printf("Ignored :| (in child)\n");
                    break;
                case 1:
                    raise(SIGUSR1);
                    break;
                case 2:
                    raise(SIGUSR1);
                    printf("Masked xO (in child)\n");
                    break;
                case 3:
                    ;sigset_t currsigs;
                    sigpending(&currsigs);
                    printf("Pending... (in child? %s)\n", sigismember(&currsigs, SIGUSR1)?"yes":"no");
                    break;
            }
        }
    }
    else{
        printf("Invalid mode!\n");
        return 1;
    }
    return 0;
}