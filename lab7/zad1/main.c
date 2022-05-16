#include "common.h"

int pids_counter, pids[1000], sem_id, shm_oven_id, shm_table_id;

struct place* oven;
struct place* table;

void sigint_handler(){
    for(int i=0;i<pids_counter;i++){
        kill(pids[i], SIGINT);
    }
    shmdt(oven);
    shmdt(table);
    semctl(sem_id, 0, IPC_RMID);
    shmctl(sem_id, IPC_RMID, NULL);
    shmctl(sem_id, IPC_RMID, NULL);
    exit(EXIT_SUCCESS);
}

void init_sem(){
    sem_id = semget(get_key(SEM_KEY_ID), 5, IPC_CREAT | 0660);
    union semun arg;
    arg.val = 1;
    semctl(sem_id, USING_OVEN_SEM, SETVAL, arg);
    semctl(sem_id, USING_TABLE_SEM, SETVAL, arg);
    arg.val = 5;
    semctl(sem_id, INSIDE_OVEN_SEM, SETVAL, arg);
    semctl(sem_id, INSIDE_TABLE_SEM, SETVAL, arg);
    arg.val = 0;
    semctl(sem_id, EMPTY_TABLE_SEM, SETVAL, arg);
}

void init_shared_mem(){
//    printf("size: %lu\n", sizeof(struct place));
//    printf("key: %d\n", get_key(SHM_OVEN_KEY_ID));
//    printf("key: %d\n", get_key(SHM_TABLE_KEY_ID));
    if((shm_oven_id = shmget(get_key(SHM_OVEN_KEY_ID), sizeof(struct place),
            IPC_CREAT | 0660))==-1){
        printf("error shmget\n");
    }
    if((shm_table_id = shmget(get_key(SHM_TABLE_KEY_ID), sizeof(struct place),
            IPC_CREAT | 0660))==-1){
    printf("error shmget 2\n");
    }

    oven = shmat(shm_oven_id, NULL, 0);
    table = shmat(shm_table_id, NULL, 0);

    oven->first = 0;
    oven->last = 0;
    oven->count = 0;

    table->first = 0;
    table->last = 0;
    table->count = 0;
}

int main(int argc, char** argv){
    if(argc!=3){
        printf("Invalid no. of args!\n");
        return -1;
    }
    int n  = atoi(argv[1]);//cook number
    int m = atoi(argv[2]);//deliver number
    pids_counter = n+m;

    init_sem();
    init_shared_mem();
    signal(SIGINT, sigint_handler);

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
    while(1);
    return 0;
}
