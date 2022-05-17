//
// Created by loloxon on 13.05.22.
//

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <sys/time.h>
#include <string.h>
#include <errno.h>

//#include <sys/sem.h>
//#include <sys/shm.h>
//#include <sys/ipc.h>
//#include <sys/types.h>

#include "semaphore.h"
#include "sys/stat.h"
#include "fcntl.h"
//#include "pthread.h"
#include "sys/mman.h"
#include "sys/stat.h"
//#include "rt"


#include <signal.h>

#ifndef LAB7_COMMON_H
#define LAB7_COMMON_H

#define SEM_KEY_ID 1
#define SHM_OVEN_KEY_ID 2
#define SHM_TABLE_KEY_ID 3

#define USING_OVEN_SEM 0
#define USING_TABLE_SEM 1
#define INSIDE_OVEN_SEM 2
#define INSIDE_TABLE_SEM 3
#define EMPTY_TABLE_SEM 4

#define USING_OVEN_NAME "using oven"
#define USING_TABLE_NAME "using table"
#define FULL_OVEN_NAME "inside oven"
#define FULL_TABLE_NAME "inside table"
#define EMPTY_TABLE_NAME "empty table"
#define OVEN_NAME "oven"
#define TABLE_NAME "table"

struct place {
    int array[5];
    int count;
    int first;
    int last;
};

union semun{
    int val;
    struct semid_ds *buf;
    unsigned short *array;
    struct seminfo *__buf;
};

void get_current_time(char *curr_time) {
    struct timeval t;
    char buffer[10];

    gettimeofday(&t, NULL);
    strftime(buffer, 20, "%H:%M:%S", localtime(&t.tv_sec));
    sprintf(curr_time, "%s:%03ld", buffer, t.tv_usec / 1000);
}

unsigned int rand_number(unsigned int min, unsigned int max) {
//    return (unsigned int)((double)random()/RAND_MAX * (max - min + 1) + min);
    return rand()%(max-min)+min;
}

key_t get_key(int proj_id) {
    const char *home = getenv("HOME");
//    if (!home) error("getenv error");

    key_t key;
//    if ((key = ftok(home, proj_id)) == -1) error("Error while generating key");
    key = ftok(home, proj_id);

    return key;
}

void get_sem_and_shm_ids(int *sem_id, int *oven_shm_id, int *table_shm_id) {
    if ((*sem_id = semget(get_key(1), 0, 0)) == -1) {
//        error("semget error");
    }
    if((*oven_shm_id = shmget(get_key(2), 0, 0)) == -1) {
//        error("Oven shmget error");
    }
    if((*table_shm_id = shmget(get_key(3), 0, 0)) == -1) {
//        error("Table shmget error");
    }
}

//void sem_decrement(int sem_id, int sem_num) {
//    struct sembuf buffer = {sem_num, -1, 0};
//    if (semop(sem_id, &buffer, 1) == -1) error("Semaphore decrement error");
//}
//
//
//void sem_increment(int sem_id, int sem_num) {
//    struct sembuf buffer = {sem_num, 1, 0};
//    if (semop(sem_id, &buffer, 1) == -1) error("Semaphore increment error");
//}

//void sem_increment()

#endif //LAB7_COMMON_H
