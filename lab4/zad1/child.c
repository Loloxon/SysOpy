#include "stdio.h"
#include "string.h"

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

int main(int argc, char** argv){
    if(argc!=2){
        printf("Invalid no. of arguments in child!\n");
        return 1;
    }
    printf("-> Child command: %s\n", argv[1]);
    int mode = checkMode(argv[1]);
    switch(mode){ // 0-ignore, 1-handler, 2-mask, 3-pending
        case 0:
            raise(SIGUSR1);
            printf("Ignored :| (in child)\n");
            break;
        case 1:
            printf("No handler allowed in child!!! (read the exercise more carefully next time >:( )\n");
            break;
        case 2:
            raise(SIGUSR1);
            printf("Masked xO (in child)\n");
            break;
        case 3:
            ;sigset_t curr_sigs;
            sigpending(&curr_sigs);
            printf("Pending... (in parent? %s)\n", sigismember(&curr_sigs, SIGUSR1)?"yes":"no");
            break;
        default:
            printf("Invalid command!\n");
            return 1;
    }
    return 0;
}