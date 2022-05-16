#include "common.h"

int pid, sem_id, oven_id, table_id;

struct place* oven;
struct place* table;

void sigint_handler(){
    shmdt(oven);
    shmdt(table);
    exit(EXIT_SUCCESS);
}

void print_timestamp(){
    char curr_time[15];
    get_current_time(curr_time);
    printf("C: (PID: %d, time: %s)         ", pid, curr_time);
}

void print_making_pizza(int n){
    print_timestamp();
    printf("Przygotowuje pizze: %d\n", n);
}

void print_adding_pizza(int n, int m){
    print_timestamp();
    printf("Dodałem pizze: %d. Liczba pizz w piecu: %d\n", n, m);
}

void print_pulling_pizza(int n, int m){
    print_timestamp();
    printf("Wyjmuje pizze: %d. Liczba pizz w piecu: %d\n", n, m);

}

void print_getting_pizza(int n, int m, int k){
    print_timestamp();
    printf("Wyjmuje pizze: %d. Liczba pizz w piecy: %d. Liczba pizz na stole: %d\n", n, m, k);

}

int random_pizza(){
    int pizza = (int)rand_number(0, 9);
    print_making_pizza(pizza);
    return pizza;
}

void pizza_to_oven(int pizza){
    sem_decrement(sem_id, USING_OVEN_SEM);

    sem_decrement(sem_id, INSIDE_OVEN_SEM);

    oven->last = (oven->last+1)%5;
    oven->array[oven->last] = pizza;
    oven->count+=1;

    print_adding_pizza(pizza, oven->count);

    sem_increment(sem_id, USING_OVEN_SEM);
}

void pizza_to_table(){
    sem_decrement(sem_id, USING_OVEN_SEM);


    int pizza = oven->array[oven->first];
    oven->first = (oven->first+1)%5;
    oven->count-=1;

    print_pulling_pizza(pizza, oven->count);

    sem_increment(sem_id, USING_OVEN_SEM);
    sem_increment(sem_id, INSIDE_OVEN_SEM);

    sem_decrement(sem_id, USING_TABLE_SEM);
    sem_decrement(sem_id, INSIDE_TABLE_SEM);

    table->last = (table->last+1)%5;
    table->array[table->last] = pizza;
    table->count+=1;

    print_getting_pizza(pizza, oven->count, table->count);

    sem_increment(sem_id, USING_TABLE_SEM);
    sem_increment(sem_id, EMPTY_TABLE_SEM);
}

int main(){
    pid = getpid();
    printf("Im cook no. %d\n", pid);

    get_sem_and_shm_ids(&sem_id, &oven_id, &table_id);

    oven = shmat(oven_id, NULL, 0);
    table = shmat(table_id, NULL, 0);

    srand(pid);
    while(1) {
        int pizza = random_pizza();

        usleep(rand_number(1000000, 2000000));

        pizza_to_oven(pizza);

        usleep(rand_number(4000000, 5000000));

        pizza_to_table();
    }
    return 0;
}
