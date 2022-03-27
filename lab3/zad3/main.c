#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include "stdlib.h"
#include "sys/wait.h"
#include "stdbool.h"

#include "string.h"

#include "dirent.h"
#include "sys/stat.h"

bool check(char* path, char* text){
    bool flag = false;
    FILE* fOut = fopen(path,"r");
    fseek(fOut, 0, SEEK_END);
    long size = ftell(fOut);
    fseek(fOut, 0, SEEK_SET);
    rewind(fOut);
    char content[size];
    fread(content, 1, size, fOut);
    fclose(fOut);
    if(strstr(content, text))
        flag = true;
    return flag;
}

void go_inside(char* path, int depth, int id, char* text){
    DIR* dirInit = opendir(path);
    struct dirent* dir;
    char newPath[512];
    pid_t child_pid;
    int ctr = 0;
    while((dir = readdir(dirInit))!=NULL){
        if(strcmp(dir->d_name,".")!=0 && strcmp(dir->d_name,"..") != 0){
            struct stat info;
            stat(dir->d_name,&info);
            strcpy(newPath, path);
            strcat(newPath, "/");
            strcat(newPath, dir->d_name);
            if(dir->d_type==DT_DIR && depth>1){
                child_pid = fork();
                if (child_pid == 0) {
                    go_inside(newPath, depth-1, 1, text);
                }
                else{
                    ctr+=1;
                }
            }
            else{
                int n = strlen(dir->d_name);
                if(n>4 && dir->d_name[n-4]=='.' && dir->d_name[n-3]=='t' && dir->d_name[n-2]=='x' && dir->d_name[n-1]=='t'){
                    if(check(newPath, text)){
                        printf("Process no: %d on relative path: %s\n",getpid(),newPath);
                    }
                }
            }
        }
    }
    for(int i=0;i<ctr;i++)
        wait(NULL);
    if(id!=-1)
        exit(0);
}

int main(int argc, char** argv){
    char* path = argv[1];
    char* text = argv[2];
    int depth = atoi(argv[3]);
    if(depth<1){
        printf("You really dont want me to go into any directory? D:\n");
        return 0;
    }
    go_inside(path, depth, -1,text);
    return 0;
}