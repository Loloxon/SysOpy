#include "../zad1/library.h"
#include "stdio.h"
#include "string.h"
#include "stdlib.h"

#include "sys/times.h"
#include "unistd.h"
#include "time.h"

clock_t timer_start, timer_end;
struct tms timer_start_tms, timer_end_tms;

void timerStart(){
    timer_start = times(&timer_start_tms);
}
double timeDiff(clock_t s, clock_t e){
    return (double)(e-s)/ (double)sysconf(_SC_CLK_TCK);
}

void printTimes(char* command){
    timer_end = times(&timer_end_tms);
    printf("%6s: %.3fs %.3fs %.3fs\n",
           command,
           timeDiff(timer_start,timer_end),
           timeDiff(timer_start_tms.tms_cutime,timer_end_tms.tms_cutime),
           timeDiff(timer_start_tms.tms_cstime,timer_end_tms.tms_cstime));
}

int main(int argc, char** argv){
    blockTable* table = NULL;
    for(int i=1;i<argc;i++){
        if(strcmp(argv[i],"create_table") == 0) {
            i++;
//            printf("create: %s\n", argv[i]);
            if(table == NULL) {
                timerStart();
                table = createPointersTable(atoi(argv[i]));
                printTimes("create");
            }
            else{
                printf("Table already created!\n");
                exit(1);
            }
        }
        if(strcmp(argv[i],"wc_files")==0){
            i++;
//            printf("wc tests: %s\n", argv[i]);
            if(table == NULL){
                printf("Table not created yet!\n");
                exit(2);
            }
            else {
                int ctr=0;
                while(i+ctr<argc && argv[i+ctr][0]=='.'){
                    ctr++;
                }
                char** paths = calloc(ctr, sizeof(char*));
                for(int k=0;k<ctr;k++){
                    paths[k] = calloc(strlen(argv[i+k]),sizeof(char));
                    strcpy(paths[k],argv[i+k]);
                }
                timerStart();
                wc(paths, ctr, table);
                printTimes("wc");
                for(int k=0;k<ctr;k++)
                    free(paths[k]);
                free(paths);
//                printf("%d:\n%s\n",index, table->info[index]);
            }
        }
        if(strcmp(argv[i],"remove_block")==0){
            i++;
//            printf("remove: %s\n", argv[i]);
            if(table == NULL){
                printf("Table not created yet!\n");
                exit(2);
            }
            else{
                timerStart();
                removeBlock(atoi(argv[i]),table);
                printTimes("remove");
            }
        }
    }
    removeTable(table);
    return 0;
}
