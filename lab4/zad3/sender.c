#include "signal.h"
#include "stdio.h"
#include "stdlib.h"
#include "unistd.h"




int recieved = 0;
pid_t sender;

void sig1_handler(int signo, siginfo_t *info, void *context){
    if(signo==SIGUSR1 && info->si_code==SI_USER){
        printf("Got SIGUSR1! (sender here)\n");
        recieved++;
        return;
    }
    return;
}

void sig2_handler(int signo, siginfo_t *info, void *context){
    if(signo==SIGUSR2 && info->si_code==SI_USER){
        printf("Got SIGUSR2! (sender here)\n");
        sender = info->si_pid;
        return;
    }
    return;
}

int main(int argc, char** argv){
    int catcher_id = atoi(argv[1]);
    int signals_total = atoi(argv[2]);
    char* mode = argv[3];
    printf("My pid is: %d\n", getpid());
    printf("I'll send to: %d\n",catcher_id);
    struct sigaction sig1;
    sig1.sa_sigaction = sig1_handler;
    if(sigaction(SIGUSR1, &sig1, NULL)==-1){
        return -1;
    }
    struct sigaction sig2;
    sig2.sa_sigaction = sig2_handler;
    if(sigaction(SIGUSR2, &sig2, NULL)==-1){
        return -1;
    }

//    sigset_t mask;
//    sigemptyset(&mask);
//    sigaddset(&mask, SIGUSR2);
//    sigsuspend(&mask);

    for(int i=0;i<signals_total;i++){
        kill(catcher_id, SIGUSR1);
//        printf("sig sent\n");
    }
//    printf("sending last one\n");
    kill(catcher_id, SIGUSR2);

    sigset_t mask;
    sigemptyset(&mask);
    sigaddset(&mask, SIGUSR2);
    sigsuspend(&mask);

    printf("Sent: %d\n Got : %d\n",signals_total, recieved);

    return 0;
}