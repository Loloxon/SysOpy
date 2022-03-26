#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include "stdlib.h"
#include "sys/wait.h"
#include "errno.h"


int main(int argc, char** argv){

    int n = atoi(argv[1]);

    for(int i=0;i<n;i++) {
        pid_t child_pid;
        child_pid = fork();
        if (child_pid == 0) {
            execvp("./child.o",NULL);
        }
    }
    for(int i=0;i<n;i++){
        wait(NULL);
    }
}