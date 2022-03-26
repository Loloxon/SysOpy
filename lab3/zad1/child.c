#include "stdio.h"
#include "unistd.h"

int main(){
    printf("Witaj! Jestem procesem nr: %d\n",(int)getpid());
    return 0;
}