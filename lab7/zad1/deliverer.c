#include "common.h"

int pid, sem_id, oven_id, table_id;

struct place* oven;
struct place* table;

int current_pizza;

void sigint_handler(){
    shmdt(oven);
    shmdt(table);
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
    sem_decrement(sem_id, EMPTY_TABLE_SEM);
    sem_decrement(sem_id, USING_TABLE_SEM);

    current_pizza = table->array[table->first];
    table->first = (table->first+1)%5;
    table->count-=1;
    print_pickup_pizza(current_pizza, table->count);

    sem_increment(sem_id, INSIDE_TABLE_SEM);
    sem_increment(sem_id, USING_TABLE_SEM);

}

void deliver_pizza(){
    print_deliver_pizza(current_pizza);
}

int main(){
    pid = getpid();
    printf("Im deliverer no. %d\n", pid);


    get_sem_and_shm_ids(&sem_id, &oven_id, &table_id);

    table = shmat(table_id, NULL, 0);

    while(1) {
        pickup_pizza();
        usleep(rand_number(4000000, 5000000));

        deliver_pizza();
        usleep(rand_number(4000000, 5000000));
    }

    return 0;
}
