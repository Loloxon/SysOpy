#include "signal.h"
#include "stdio.h"
#include "stdlib.h"
#include "unistd.h"




int recieved = 0;
pid_t sender_id;

void sig1_handler(int signo, siginfo_t *info, void *context){
    if(signo==SIGUSR1 && info->si_code==SI_USER){
        printf("Got SIGUSR1! (%d) (catcher here)\n", recieved);
        recieved++;
        return;
    }
    return;
}

void sig2_handler(int signo, siginfo_t *info, void *context){
    if(signo==SIGUSR2 && info->si_code==SI_USER){
        printf("Got SIGUSR2! (catcher here)\n");
        sender_id = info->si_pid;
        return;
    }
    return;
}

int main(int argc, char** argv){
    printf("Catcher pid: %d\n",getpid());
    struct sigaction sig1;
    sig1.sa_sigaction = sig1_handler;
    sig1.sa_flags = SA_SIGINFO;
    if(sigaction(SIGUSR1, &sig1, NULL)==-1){
        return -1;
    }
    struct sigaction sig2;
    sig2.sa_sigaction = sig2_handler;
    sig2.sa_flags = SA_SIGINFO;
    if(sigaction(SIGUSR2, &sig2, NULL)==-1){
        return -1;
    }

    sigset_t mask;
    sigemptyset(&mask);
    sigaddset(&mask, SIGUSR2);
    sigsuspend(&mask);

    printf("I'll send to: %d\n",sender_id);
    for(int i=0;i<recieved;i++){
        kill(sender_id, SIGUSR1);
    }
    kill(sender_id, SIGUSR2);

    printf("Recieved: %d\n", recieved);

    return 0;
}