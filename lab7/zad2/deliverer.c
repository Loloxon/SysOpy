#include "common.h"

int pid, sem_id, oven_id, table_id;
sem_t* using_oven_adr, using_table_adr, full_oven_adr, full_table_adr, empty_table_adr;

struct place* oven;
struct place* table;

int current_pizza;

void sigint_handler(){
//    shmdt(oven);
//    shmdt(table);
    exit(EXIT_SUCCESS);
}

void print_timestamp(){
    char curr_time[15];
    get_current_time(curr_time);
    printf("D: (PID: %d, time: %s)         ", pid, curr_time);
}

void print_pickup_pizza(int n, int k){
    print_timestamp();
    printf("Pobieram pizze: %d. Liczba pizz na stole: %d\n", n, k);
}

void print_deliver_pizza(int n){
    print_timestamp();
    printf("Dostarczam pizze: %d\n", n);

}


void pickup_pizza(){
//    sem_decrement(sem_id, EMPTY_TABLE_SEM);
//    sem_decrement(sem_id, USING_TABLE_SEM);
    sem_wait(&empty_table_adr);
    sem_wait(&using_table_adr);

    current_pizza = table->array[table->first];
    table->first = (table->first+1)%5;
    table->count-=1;
    print_pickup_pizza(current_pizza, table->count);

//    sem_increment(sem_id, INSIDE_TABLE_SEM);
//    sem_increment(sem_id, USING_TABLE_SEM);
    sem_post(&full_table_adr);
    sem_post(&using_table_adr);
}

void deliver_pizza(){
    print_deliver_pizza(current_pizza);
}

int main(){
    pid = getpid();
    printf("Im deliverer no. %d\n", pid);


    get_sem_and_shm_ids(&sem_id, &oven_id, &table_id);

//    table = shmat(table_id, NULL, 0);
    table = mmap(NULL, sizeof(struct place), PROT_WRITE | PROT_READ, MAP_SHARED,
                 shm_open(TABLE_NAME, O_RDWR, 0666),
                 0);

    while(1) {
        pickup_pizza();
        usleep(rand_number(4000000, 5000000));

        deliver_pizza();
        usleep(rand_number(4000000, 5000000));
    }

    return 0;
}
