#include "signal.h"
#include "stdio.h"
#include "stdlib.h"
#include "unistd.h"
#include "string.h"
#include "errno.h"


int recieved = 0;
pid_t sender;

int signal1 = SIGUSR1;
int signal2 = SIGUSR2;

void sig1_handler(int signo, siginfo_t *info, void *context){
    if(signo==signal1 && info->si_code==SI_USER){
        printf("Got signal1! (sender here)\n");
        recieved++;
        return;
    }
    return;
}

void sig2_handler(int signo, siginfo_t *info, void *context){
    if(signo==signal2 && info->si_code==SI_USER){
        printf("Got signal2! (sender here)\n");
        sender = info->si_pid;
        return;
    }
    return;
}

int main(int argc, char** argv){
    int catcher_id = atoi(argv[1]);
    int released = atoi(argv[2]);
    char* mode = argv[3];
    if(strcmp(mode,"kill") && strcmp(mode,"sigqueue") && strcmp(mode,"sigrt")){
        printf("Invalid mode!\n");
        return -1;
    }
    if(!strcmp(mode,"sigrt")){
        signal1 = SIGRTMIN+12;
        signal2 = SIGRTMIN+24;
    }
    printf("My pid is: %d\n", getpid());
    printf("I'll send to: %d\n",catcher_id);
    struct sigaction sig1;
    sig1.sa_sigaction = sig1_handler;
    sig1.sa_flags = SA_SIGINFO;
    sigemptyset(&sig1.sa_mask);
    if(sigaction(signal1, &sig1, NULL)==-1){
        return -1;
    }
    struct sigaction sig2;
    sig2.sa_sigaction = sig2_handler;
    sig2.sa_flags = SA_SIGINFO;
    sigemptyset(&sig2.sa_mask);
    if(sigaction(signal2, &sig2, NULL)==-1){
        return -1;
    }
    
    if (!strcmp(mode, "kill") || !strcmp(mode, "sigrt")) {
        for (int i = 0; i < released; i++) {
            kill(catcher_id, signal1);
        }
        printf("Sending signal2 from sender\n");
        kill(catcher_id, signal2);
    }
    else if (!strcmp(mode, "sigqueue")) {
        union sigval val;
        for (int i = 0; i < released; i++) {
            union sigval val;
            val.sival_int = i;
            if (sigqueue(catcher_id, signal1, val) == 0) {
                printf("Signal sent!\n");
            } else {
                printf("ERROR: %d\n", errno);
            }
        }
        printf("Sending signal2 from sender\n");
        sigqueue(catcher_id, signal2, val);
    }
    sigset_t mask;
    sigemptyset(&mask);
    sigaddset(&mask, signal2);
    sigsuspend(&mask);


    printf("Caught signal2 in sender\n");


    printf("Sent: %d\n Got : %d\n",released, recieved);

    return 0;
}