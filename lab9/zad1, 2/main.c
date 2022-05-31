#include "utils.h"

pthread_t santa_thread;
pthread_t reindeer_threads[REINDEER_NUM];
pthread_t elf_threads[ELF_NUM];

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond_elves = PTHREAD_COND_INITIALIZER;
pthread_cond_t cond_reindeers = PTHREAD_COND_INITIALIZER;
pthread_cond_t cond_santa = PTHREAD_COND_INITIALIZER;

int elves;
int reindeers;
pthread_t problematic_elves[3];

void* santa(void* arg){
    printf("            Mikołaj here!\n");
    int deliveries = 3;
    while(deliveries>0){
        pthread_mutex_lock(&mutex);
        while(reindeers<REINDEER_NUM && elves<ELVES_REQUIRED) {
            printf("            Mikołaj: zasypiam\n");
            pthread_cond_wait(&cond_santa, &mutex);
            printf("            Mikołaj: budzę się\n");
        }
        if(reindeers==REINDEER_NUM){
            pthread_mutex_unlock(&mutex);
            printf("            Mikołaj: dostarczam zabawki %d raz\n", (4-deliveries));
            rand_working_sleep();
            pthread_mutex_lock(&mutex);
            reindeers=0;
            deliveries--;
            pthread_cond_broadcast(&cond_reindeers);
            if(deliveries==0)
                break;
        }
        if(elves==ELVES_REQUIRED){
            pthread_mutex_unlock(&mutex);
            printf("            Mikołaj: rozwiązuje problem elfów %lu, %lu, %lu\n", problematic_elves[0], problematic_elves[1], problematic_elves[2]);
            rand_solving_sleep();
            pthread_mutex_lock(&mutex);
            elves=0;
            pthread_cond_broadcast(&cond_elves);
        }
        pthread_mutex_unlock(&mutex);
    }
    printf("            Mikołaj: starczy tego!\n");
    pthread_exit((void *) NULL);
}

void* elf(void* arg){
    pthread_t id = pthread_self()%1000;
    printf("Elf %lu here!\n", id);

    int waits = 0;

    while(1){
        if(!waits) rand_working_sleep();

        pthread_mutex_lock(&mutex);
        if(elves<ELVES_REQUIRED) {
            problematic_elves[elves] = id;
            elves++;
            printf("Elf: czeka %d elfów na Mikołaja, %lu\n", elves, id);
            if(elves==ELVES_REQUIRED){
                printf("Elf: wybudzam Mikołaja, %lu\n", id);
                pthread_cond_signal(&cond_santa);
            }
            pthread_cond_wait(&cond_elves, &mutex);
            waits=0;
        }
        else{
            printf("Elf: czeka na powrót elfów, %lu\n", id);
            waits=1;
            while(elves>0)
                pthread_cond_wait(&cond_elves, &mutex);
        }
        
        while(reindeers>0){
            pthread_cond_wait(&cond_reindeers, &mutex);
        }
        pthread_mutex_unlock(&mutex);
    }
}

void* reindeer(void* arg){
    pthread_t id = pthread_self()%1000;
    printf("      Renifer %lu here!\n", id);

    while(1){
        rand_chilling_sleep();
        pthread_mutex_lock(&mutex);
        reindeers++;
        printf("      Renifer: czeka %d reniferów na Mikołaja, %lu\n", reindeers, id);
        if(reindeers==REINDEER_NUM){
            printf("      Renifer: wybudzam Mikołaja, %lu\n", id);
            pthread_cond_signal(&cond_santa);
        }
        while(reindeers>0){
            pthread_cond_wait(&cond_reindeers, &mutex);
        }
        pthread_mutex_unlock(&mutex);
        printf("      Renifer: %lu ide odpocząć\n", id);
    }
}

int main(int argc, char** argv){
    if(argc>1){
        printf("Too many args!\n");
        return -1;
    }
    printf("Hello there!\n");


    if ((pthread_create(&santa_thread, NULL, santa, (void *) NULL)) != 0)
        XDD
    for(int i=0;i<ELF_NUM;i++){
        if ((pthread_create(&elf_threads[i], NULL, elf, (void *) NULL)) != 0)
            XDD
    }
    for(int i=0;i<REINDEER_NUM;i++){
        if ((pthread_create(&reindeer_threads[i], NULL, reindeer, (void *) NULL)) != 0)
            XDD
    }
    pthread_join(santa_thread, NULL);

    for(int i=0;i<ELF_NUM;i++){
        pthread_cancel(elf_threads[i]);
    }
    for(int i=0;i<REINDEER_NUM;i++){
        pthread_cancel(reindeer_threads[i]);
    }
    pthread_mutex_destroy(&mutex);
    pthread_cond_destroy(&cond_reindeers);
    pthread_cond_destroy(&cond_santa);
    pthread_cond_destroy(&cond_elves);
    return 0;
}