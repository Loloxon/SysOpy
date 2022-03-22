#include "stdio.h"
#include "string.h"
#include "stdlib.h"

#include "dirent.h"
#include "sys/stat.h"
#include "time.h"

char* timeprs(__time_t* t){
    char* tmp = calloc(32,1);
    strftime(tmp, 32, "%Y-%m-%d %H:%M:%S",localtime(t));
    return tmp;
}

int FIFO=0,soket=0,katalog=0,zwykly_plik=0,plik_blokowy=0,plik_znakowy=0,link_symboliczny=0;

char* moreinfo(char type){
    if(DT_FIFO == type){
        FIFO++;
        return "kolejka FIFO";
    }
    if(DT_SOCK == type){
        soket++;
        return "soket";
    }
    if(DT_DIR == type){
        katalog++;
        return "katalog";
    }
    if(DT_REG == type){
        zwykly_plik++;
        return "zwykly plik";
    }
    if(DT_BLK == type){
        plik_blokowy++;
        return "plik specialny blokowy";
    }
    if(DT_CHR == type){
        plik_znakowy++;
        return "plik specialny znakowy";
    }
    if(DT_LNK == type){
        link_symboliczny++;
        return "link symboliczny";
    }
    return "idk";
}

void printinfo(struct dirent* dir, char* path){
    struct stat info;
    stat(dir->d_name,&info);
    printf("Ścieżka bezwzględna: %s\n"
           "Liczba dowiązań: %lu\n"
           "Rodzaj pliku: %s\n"
           "Rozmiar w bajtach: %ld\n"
           "Data ostatniego dostępu: %s\n"
           "Data ostatniej modyfikacji: %s\n"
           "--------------------\n", realpath(path,NULL), info.st_nlink, moreinfo(dir->d_type),
           info.st_size, timeprs((__time_t*)&info.st_atim), timeprs((__time_t*)&info.st_mtim));
}

void dfs(char* path){
    DIR* dirInit = opendir(path);
    if(!dirInit)
        return;
    struct dirent* dir;
    char newPath[512];
    while((dir = readdir(dirInit))!=NULL){
        if(strcmp(dir->d_name,".")!=0 && strcmp(dir->d_name,"..") != 0){
            strcpy(newPath, path);
            strcat(newPath, "/");
            strcat(newPath, dir->d_name);
            printinfo(dir, newPath);
            dfs(newPath);
        }
    }
    closedir(dirInit);
}

int main(int argc, char** argv){
    if(argc>2){
        printf("Too many arguments\n");
    }
    else if(argc<2){
        printf("You didnt give me directory!!!\n");
    }
    else{
        dfs(argv[1]);
        printf(" FIFO: %d\n soket: %d\n katalog: %d\n zwykly_plik: %d\n plik_blokowy: %d\n plik_znakowy: %d\n link_symboliczny: %d\n",
               FIFO, soket, katalog, zwykly_plik, plik_blokowy, plik_znakowy, link_symboliczny);
    }
    return 0;
}
