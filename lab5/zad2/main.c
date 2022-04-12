#include "stdio.h"
#include "string.h"
#include "stdlib.h"

int main(int argc, char* argv[]){
    if(argc==2){

        char* arg = argv[1];
        char cmd[16] = "sort -k ";
        strcmp(arg,"data")==0?strcat(cmd,"3"):(strcmp(arg,"nadawca")==0?strcat(cmd,"2"):exit(11));
        FILE* sort = popen(cmd, "w");
        FILE* mails = popen("mailq", "r");
        char buff[1024];
        while(fgets(buff, sizeof(buff), mails)!=NULL)
            fputs(buff, sort);
        pclose(sort);
        pclose(mails);

    }else if(argc==4){
        char* email = argv[1];
        char* title = argv[2];
        char* text = argv[3];
        char cmd[4096] = "mail -s '";
        strcat(strcat(strcat(cmd, title),"' "),email);
//        printf("%s\n",cmd);
        FILE* mail = popen(cmd, "w");
        fputc('"', mail);
        fputs(text, mail);
        fputc('"', mail);
        pclose(mail);

    }else{
        printf("invalid arguments\n");
        exit(12);
    }
    return 0;
}