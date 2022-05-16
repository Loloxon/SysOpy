#include "common.h"

int pid;

struct place* oven;
struct place* table;

int current_pizza;

void print_timestamp(){
    char curr_time[15];
    get_current_time(curr_time);
    printf("(PID: %d, time: %s) ", pid, curr_time);
}

void print_pickup_pizza(int n, int k){
    print_timestamp();
    printf("Pobieram pizze: %d. Liczba pizz na stole: %d\n", n, k);
}

void print_deliver_pizza(int n){
    print_timestamp();
    printf("Dostarczam pizze: %d\n", n);

}


// 1) Dostawca pobiera pizze ze stołu.
//Wypisanie komunikatu: (pid timestamp) Pobieram pizze: n Liczba pizz na stole: k.
//
//2) Dojeżdża do klienta (4-5s).
//
//3) Dostarcza pizze.
//Wypisanie komunikatu: (pid timestamp) Dostarczam pizze: n.
//
//4) Wraca (4-5s).

void pickup_pizza(){
    current_pizza = table->first;
    table->first = (table->first+1)%5;
    table->count-=1;
    print_pickup_pizza(current_pizza, table->count);
}

void deliver_pizza(){
    print_deliver_pizza(current_pizza);
}

int main(){
    pid = getpid();

//    int sem_id, oven_id, table_id;
//    get_sem_and_shm_ids(&sem_id, &oven_id, &table_id);
//
//    oven = shmat(oven_id, NULL, 0);
//    table = shmat(table_id, NULL, 0);

    pickup_pizza();

    usleep(rand_number(4000000,5000000));

    deliver_pizza();

    usleep(rand_number(4000000,5000000));

    return 0;
}
