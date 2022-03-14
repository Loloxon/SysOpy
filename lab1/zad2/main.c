#include "../zad1/library.h"
#include "stdio.h"
#include "string.h"
#include "stdlib.h"

#include "sys/times.h"
#include "unistd.h"

int main(int argc, char** argv){
    blockTable* table = NULL;
    for(int i=1;i<argc;i++){
        if(strcmp(argv[i],"create_table") == 0) {
            i++;
//            printf("create: %s\n", argv[i]);
            if(table == NULL) {
                table = createPointersTable(atoi(argv[i]));
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
                int index = wc(paths, ctr, table);
                for(int k=0;k<ctr;k++)
                    free(paths[k]);
                free(paths);
                printf("%d:\n%s\n",index, table->info[index]);
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
                removeBlock(atoi(argv[i]),table);
            }
        }
    }
    removeTable(table);
    return 0;
}
