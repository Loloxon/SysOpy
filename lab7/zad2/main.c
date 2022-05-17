#include "common.h"

int pids_counter, pids[1000], sem_id, shm_oven_id, shm_table_id;
void* shm_oven_adr, shm_table_adr;
sem_t* using_oven_adr, using_table_adr, full_oven_adr, full_table_adr, empty_table_adr;
struct place* oven;
struct place* table;

void sigint_handler(){
    for(int i=0;i<pids_counter;i++){
        kill(pids[i], SIGINT);
    }
//    shmdt(oven);
//    shmdt(table);
    munmap(shm_oven_adr, sizeof(struct place));
    shm_unlink(OVEN_NAME);
    munmap(shm_table_adr, sizeof(struct place));
    shm_unlink(TABLE_NAME);

    sem_close(using_oven_adr);
    sem_unlink(USING_OVEN_NAME);
    sem_close(using_table_adr);
    sem_unlink(USING_TABLE_NAME);
    sem_close(full_oven_adr);
    sem_unlink(FULL_OVEN_NAME);
    sem_close(full_table_adr);
    sem_unlink("inside talbe");
    sem_close(empty_table_adr);
    sem_unlink(EMPTY_TABLE_NAME);
//    semctl(sem_id, 0, IPC_RMID);
//    shmctl(sem_id, IPC_RMID, NULL);
//    shmctl(sem_id, IPC_RMID, NULL);
    exit(EXIT_SUCCESS);
}

void init_sem(){
//    sem_id = semget(get_key(SEM_KEY_ID), 5, IPC_CREAT | 0660);
//    sem_adr = sem_open("hi", 0, 0, )
    union semun arg;
    arg.val = 1;
    using_oven_adr = sem_open(USING_OVEN_NAME, O_CREAT, , 1);
    using_table_adr = sem_open(USING_TABLE_NAME, O_CREAT, , 1);
    full_oven_adr = sem_open(FULL_OVEN_NAME, O_CREAT, , 5);
    full_table_adr = sem_open(FULL_TABLE_NAME, O_CREAT, , 5);
    empty_table_adr = sem_open(EMPTY_TABLE_NAME, O_CREAT, , 0);
//    semctl(sem_id, USING_OVEN_SEM, SETVAL, arg);
//    semctl(sem_id, USING_TABLE_SEM, SETVAL, arg);
//    arg.val = 5;
//    semctl(sem_id, INSIDE_OVEN_SEM, SETVAL, arg);
//    semctl(sem_id, INSIDE_TABLE_SEM, SETVAL, arg);
//    arg.val = 0;
//    semctl(sem_id, EMPTY_TABLE_SEM, SETVAL, arg);
}

void init_shared_mem(){
//    printf("size: %lu\n", sizeof(struct place));
//    printf("key: %d\n", get_key(SHM_OVEN_KEY_ID));
//    printf("key: %d\n", get_key(SHM_TABLE_KEY_ID));
//    if((shm_oven_id = shmget(get_key(SHM_OVEN_KEY_ID), sizeof(struct place),
//            IPC_CREAT | 0660))==-1){
//        printf("error shmget\n");
//    }
//    if((shm_table_id = shmget(get_key(SHM_TABLE_KEY_ID), sizeof(struct place),
//            IPC_CREAT | 0660))==-1){
//    printf("error shmget 2\n");
//    }
//
//    oven = shmat(shm_oven_id, NULL, 0);
//    table = shmat(shm_table_id, NULL, 0);
    shm_oven_id = shm_open(OVEN_NAME, O_CREAT);
    ftruncate(shm_oven_id, sizeof(struct place));
    shm_oven_adr = mmap(NULL, sizeof(struct place), PROT_WRITE | PROT_READ, MAP_SHARED, shm_oven_id, 0);

    shm_table_id = shm_open(TABLE_NAME, O_CREAT);
    ftruncate(shm_table_id, sizeof(struct place));
    shm_table_adr = mmap(NULL, sizeof(struct place), PROT_WRITE | PROT_READ, MAP_SHARED, shm_table_id, 0);

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
