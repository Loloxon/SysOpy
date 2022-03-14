#include "library.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>

char* path = "/tmp/tmp.txt";

blockTable *createPointersTable(int size){ //zarezerwowanie miejsca
    blockTable* table = calloc(1,sizeof(blockTable));
    table->size = size;
    table->info = calloc(size, sizeof(char*));
    return table;
}

int wc(char* filePath[], int s, blockTable* table) {//wc dla zadanych plików
    unsigned long len = 0;
    for (int i=0; i<s; i++) {
        len += strlen(filePath[i])+1;
    }
    char* command = calloc(len+s+50, sizeof (char));
    strcat(command, "wc ");
    for (int i=0; i<s; i++) {
        strcat(command, filePath[i]);
        strcat(command, " ");
    }
    strcat(command, "> ");
    strcat(command, path);
    system(command);
    free(command);
    int i=0;
    for(;i<table->size;i++){
        if(table->info[i]==NULL){
            break;
        }
        if(i==table->size-1){
            printf("no more free space\n");
            exit(112);
        }
    }
    FILE* tmp = fopen(path,"rb");
    if(!tmp) {
        fprintf(stderr,"BIG ERROR while opening tmp file\n");
    }
    fseek(tmp, 0, SEEK_END);
    long size = ftell(tmp);
    fseek(tmp, 0, SEEK_SET);
    rewind(tmp);
    table->info[i] = calloc(size+10,sizeof(char));

    fread(table->info[i],sizeof(char), size+5,tmp);
    fclose(tmp);
    return i;
}

void removeBlock(int index, blockTable* table){
    free(table->info[index]);
    table->info[index]=NULL;
}

void removeTable(blockTable* table){
    for(int i=0;i<table->size;i++)
        if(table->info[i]!=NULL)
            free(table->info[i]);
    free(table->info);
    free(table);
}