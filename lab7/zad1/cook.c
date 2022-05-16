#include "common.h"

int pid;

struct place* oven;
struct place* table;

void print_timestamp(){
    char curr_time[15];
    get_current_time(curr_time);
    printf("(PID: %d, time: %s) ", pid, curr_time);
}

void print_making_pizza(int n){
    print_timestamp();
    printf("Przygotowuje pizze: %d\n", n);
}

void print_adding_pizza(int n, int m){
    print_timestamp();
    printf("Dodałem pizze: %d. Liczba pizz w piecu: %d\n", n, m);
}

void print_getting_pizza(int n, int m, int k){
    print_timestamp();
    printf("Wyjmuje pizze: %d. Liczba pizz w piecy: %d. Liczba pizz na stole: %d\n", n, m, k);

}


// 1) Losuje typ pizzy (n) w przedziale 0-9 i następnie ją przygotowuje (1-2s).
//Wypisanie komunikatu: (pid timestamp) Przygotowuje pizze: n.
//gdzie pid to PID procesu pracownika, timestamp to aktualny czas (z dokładnością do milisekund)
//
//2) Umiesza pizze w piecu (wpisuje n).
// Wypisanie komunikatu: (pid timestamp) Dodałem pizze: n. Liczba pizz w piecu: m.
//
//3) Czeka (4-5s).
//
//4) Wyjmuje pizze i umieszcza ją na stole do wysyłki (wpisuje n).
//Wypisanie komunikatu: (pid timestamp) Wyjmuję pizze: n. Liczba pizz w piecu: m. Liczba pizz na stole: k.

int random_pizza(){
    int pizza = (int)rand_number(0, 9);
    print_making_pizza(pizza);
    return pizza;
}

void pizza_to_oven(int pizza){
    if(oven->count>=5) {
    //todo wait till free
    }
    oven->last+=1;
    oven->array[oven->last] = pizza;
    oven->count+=1;

    print_adding_pizza(pizza, oven->count);
}

void pizza_to_table(){
    int pizza = oven->first;
    oven->first = (oven->first+1)%5;
    oven->count-=1;

    if(table->count>=5) {
        //todo wait till free
    }
    table->last = (table->last+1)%5;
    table->array[table->last] = pizza;
    table->count+=1;

    print_getting_pizza(pizza, oven->count, table->count);
}

int main(){
    pid = getpid();

    int sem_id, oven_id, table_id;
    get_sem_and_shm_ids(&sem_id, &oven_id, &table_id);

    oven = shmat(oven_id, NULL, 0);
    table = shmat(table_id, NULL, 0);

    int pizza = random_pizza();

    usleep(rand_number(1000000,2000000));

    pizza_to_oven(pizza);

    usleep(rand_number(4000000,5000000));

    pizza_to_table();

    return 0;
}
