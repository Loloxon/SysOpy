#include "common.h"

int pids_counter;
int pids[1000];

void sigint_handler(){

}

void init_sem(){

}

void init_shared_mem(){

}

int main(int argc, char** argv){
    if(argc!=3){
        printf("Invalid no. of args!\n");
        return -1;
    }
    signal(SIGINT, sigint_handler);
    int n  = atoi(argv[1]);//cook number
    int m = atoi(argv[2]);//deliver number
    pids_counter = n+m;

    init_sem();
    init_shared_mem();

    pid_t child;
    for(int i=0;i<pids_counter;i++){
        child = fork();
        if(child == 0){
            if(i<n)
                execl("./cook", "./cook", NULL);
            else
                execl("./deliver", "./deliver", NULL);
        }
        else{
            pids[i] = child;
        }
    }
    return 0;
}
