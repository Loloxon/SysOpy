#ifndef SYSOPY_LIBRARY_H
#define SYSOPY_LIBRARY_H

#include <bits/types/FILE.h>

typedef struct blockTable{
    char **info;
    int size;
}blockTable;

blockTable* createPointersTable(int size);
int wc(char* filePath[], int s, blockTable* table);
void freeMem(int index, blockTable* table);
void freeTable(blockTable* table);

#endif //SYSOPY_LIBRARY_H
