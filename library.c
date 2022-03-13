#include "library.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void hello(void) {
    printf("Hello, World!\n");
}

blockTable* table = NULL;

blockTable* createPointersTable(int size){ //zarezerwowanie miejsca
    table = calloc(1,sizeof(blockTable));
    table->size = size;
    return table;
}

int countLinesWordsChars(char* filePath[]) {//wc dla zadanych plików
    int s = sizeof(filePath) / sizeof(filePath[0]);
//    FILE *tmp = tmpfile();
//    if (tmp == NULL) {
//        puts("Unable to create temp file");
//        return -1;
//    }
//    else {
        puts("Temporary file is created\n");
        int len = 5;
        for (int i=0; i<=s; i++) {
            len += strlen(filePath[i])+1;
        }
        char command[len];
        strcat(command, "wc ");
        for (int i=0; i<=s; i++) {
            strcat(command, filePath[i]);
            strcat(command, " ");
        }
        strcat(command, "> /tmp/tmp.txt");
//        strcat(command, "> tmp.txt") //znalezc sciezke pliku tmp
        system(command);
//        fputs(system(command), tmp);
//    }
//    return tmp;

    int i=0;
    for(;i<table->size;i++){
        if(table->info[i]==NULL)
            break;
        if(i==table->size-1){
            printf("no more free space");
            return -1;
        }
    }
    FILE* tmp = fopen("/tmp/tmp.txt","r");
    long size = ftell(tmp);
    table->info[i] = calloc(1,size);
    fread(table->info[i],1, size,tmp);
    return i;

}

int saveToBlock(FILE* tmp){

}

void freeMem(int index){
    free(table->info[index]);
    table->info[index]=NULL;
}