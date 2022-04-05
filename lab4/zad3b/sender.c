#include "signal.h"
#include "stdio.h"
#include "stdlib.h"
#include "unistd.h"
#include "string.h"
#include "errno.h"


int recieved = 0;

int signal1 = SIGUSR1;
int signal2 = SIGUSR2;

int send = 0;
int finished = 0;

void sig1_handler(int signo, siginfo_t *info, void *context){
    if(signo==signal1){
        printf("Got signal1! (sender here)\n");
        recieved++;
        send = 0;
        return;
    }
    return;
}

void sig2_handler(int signo, siginfo_t *info, void *context){
    if(signo==signal2){
        printf("Got signal2! (sender here)\n");
        finished = 1;
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
            printf("Sending signal1 from sender\n");
            kill(catcher_id, signal1);
            send=1;
            while(send==1){

            }
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
                printf("Sending signal1 from sender\n");
            } else {
                printf("ERROR: %d\n", errno);
            }
            send = 1;
            while(send==1){

            }
        }
        printf("Sending signal2 from sender\n");
        sigqueue(catcher_id, signal2, val);
    }

    while(!finished) {

    }

    printf("Caught signal2 in sender\n");


    printf("Sent: %d\n Got : %d\n",released, recieved);

    return 0;
}