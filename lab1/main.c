#include "library.h"
#include "stdio.h"

int main(){
    int argc = 3;
    char* argv[argc];
    argv[0] = "/home/loloxon/Pulpit/SysOpy/lab1/library.c";
    argv[1] = "/home/loloxon/Pulpit/SysOpy/lab1/library.h";
    argv[2] = "/home/loloxon/Pulpit/SysOpy/lab1/main.c";
//    printf("%d \n",argc);
//    for(int i=0;i<argc;i++)
//        printf("%s \n",argv[i]);
    blockTable* table = createPointersTable(5);
//    printf("size of table: %d \n",table->size);
//    char** paths = argv;
//    printf((const char *) paths);
//    printf("Xefwfdss\n");
    int index = countLinesWordsChars(argv, argc);
    int x = countLinesWordsChars(argv, 1);
//    int index = saveToBlock();
    printf("%d: \n %s\n",index, table->info[index]);
    printf("%d: \n %s\n",x, table->info[x]);
//    int x = countLinesWordsChars(argv, argc);
//    printf("index: %d, %d",index, x);
    freeMem(index);
    int index2 = countLinesWordsChars(argv, 2);
    printf("%d: \n %s\n",index2, table->info[index2]);
    freeMem(x);
    freeMem(index2);
    return 0;
}
