#include "../zad1/library.h"
#include "stdio.h"

int main(int argc, char** argv){
//    argc = 3;
//    char* argv[argc];
    printf("hehehe: %d\n", argc);
    for(int i=0;i<argc;i++){
        printf("%s \n", argv[i]);
    }
//    argv[0] = "../zad2/tests/big_1";
//    argv[1] = "../zad2/tests/big_2";
//    argv[2] = "../zad2/tests/big_3";
//    printf("%d \n",argc);
//    for(int i=0;i<argc;i++)
//        printf("%s \n",argv[i]);
    blockTable* table = createPointersTable(argc);
//    printf("size of table: %d \n",table->size);
//    char** paths = argv;
//    printf((const char *) paths);
//    printf("Xefwfdss\n");
    int index = wc(argv, argc, table);
//    int x = wc(argv, 1, table);
//    int index = saveToBlock();
    printf("%d: \n %s\n",index, table->info[index]);
//    printf("%d:\n",index);
//    printf("%d: \n %s\n",x, table->info[x]);
//    printf("%d: \n",x);
//    int x = countLinesWordsChars(argv, argc);
//    printf("index: %d, %d",index, x);
    freeMem(index, table);
//    int index2 = wc(argv, 2, table);
//    printf("%d: \n %s \n",index2, table->info[index2]);
//    freeMem(x, table);
//    freeMem(index2, table);
    freeTable(table);
    return 0;
}
