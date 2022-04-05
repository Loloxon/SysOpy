#include "signal.h"
#include "stdio.h"
#include "stdlib.h"
#include "unistd.h"
#include "errno.h"
#include "string.h"

int recieved = 0;
pid_t sender_id;

int signal1 = SIGUSR1;
int signal2 = SIGUSR2;

char* mode;
int finish = 0;

void sig1_handler(int signo, siginfo_t *info, void *context){
    if(signo==signal1){
        printf("Got signal1! (%d) (catcher here)\n", recieved);
        recieved++;
        sender_id = info->si_pid;
        if (!strcmp(mode, "kill") || !strcmp(mode, "sigrt")) {
            printf("Sending signal1 from catcher\n");
            kill(sender_id, signal1);
        }
        else if (!strcmp(mode, "sigqueue")) {
            union sigval val;
            val.sival_int = recieved;
            printf("Sending signal1 from catcher\n");
            sigqueue(sender_id, signal1, val);
        }
        return;
    }
    return;
}

void sig2_handler(int signo, siginfo_t *info, void *context){
    if(signo==signal2){
        printf("Got signal2! (catcher here)\n");
        finish = 1;
        return;
    }
    return;
}

int main(int argc, char** argv){
    mode = argv[1];
    if(strcmp(mode,"kill") && strcmp(mode,"sigqueue") && strcmp(mode,"sigrt")){
        printf("Invalid mode!\n");
        return -1;
    }
    if(!strcmp(mode,"sigrt")){
        signal1 = SIGRTMIN+12;
        signal2 = SIGRTMIN+24;
    }
    printf("Catcher pid: %d\n",getpid());
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
//    sigset_t mask1;
//    sigemptyset(&mask1);
//    sigaddset(&mask1, signal1);
//    sigsuspend(&mask1);

    sigset_t mask;
    sigemptyset(&mask);
    sigaddset(&mask, signal2);
    sigsuspend(&mask);

    while(!finish){

    }

    if (!strcmp(mode, "kill") || !strcmp(mode, "sigrt")) {
        printf("Sending signal2 from catcher\n");
        kill(sender_id, signal2);
    }
    else if (!strcmp(mode, "sigqueue")) {
        union sigval val;
        val.sival_int = recieved;
        printf("Sending signal2 from catcher\n");
        sigqueue(sender_id, signal2, val);
    }

    printf("Recieved: %d\n", recieved);

    return 0;
}