#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>


int main(int argc, char** argv){

    int n = atoi(argv[1]);

    for(int i=0;i<n;i++) {
        pid_t child_pid;
        child_pid = fork();
        wait();
        if (child_pid == 0) {
            execvp("./child.o",NULL);
        }
    }
}