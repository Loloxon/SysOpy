#include "library.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>


//struct blockTable* table = NULL;
char* path = "/tmp/tmp.txt";

blockTable *createPointersTable(int size){ //zarezerwowanie miejsca
    blockTable* table = calloc(1,sizeof(blockTable));
    table->size = size;
    table->info = calloc(size, sizeof(char*));
//    for(int i=0;i<size;i++) {
//        table->info[i] = NULL;
//    }
    return table;
}

int countLinesWordsChars(char* filePath[], int s, blockTable* table) {//wc dla zadanych plików
    unsigned long len = 0;
    for (int i=0; i<s; i++) {
        len += strlen(filePath[i])+1;
    }
    char* command = calloc(len+100, sizeof (char));
    strcat(command, "wc ");
    for (int i=0; i<s; i++) {
        strcat(command, filePath[i]);
        strcat(command, " ");
    }
    strcat(command, "> ");
    strcat(command, path);
//    printf("%s \n", command);
    system(command);
    free(command);
//    printf("table size: %d\n", table->size);
    int i=0;
    for(;i<table->size;i++){
        if(table->info[i]==NULL){
//            printf("i: %d\n", i);
            break;
        }
        if(i==table->size-1){
            printf("no more free space");
            return -1;
        }
    }
    FILE* tmp = fopen(path,"rb");

    if(!tmp) {
        fprintf(stderr,"BIG ERROR\n");
    }
    fseek(tmp, 0, SEEK_END);
    long size = ftell(tmp);
    rewind(tmp);

//    printf("size: %ld; i: %d\n", size, i);
    table->info[i] = calloc(size+10,sizeof(char));

    fread(table->info[i],1, size,tmp);
    fclose(tmp);
//    printf("%s \n",table->info[i]);
    return i;
}

void saveToBlock(){
}

void freeMem(int index, blockTable* table){
    free(table->info[index]);
    table->info[index]=NULL;
}