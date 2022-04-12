#include "stdio.h"
#include "string.h"
#include "stdlib.h"

#include "unistd.h"

long size;
int max_len;
int cmd_len_max;

void run_commands(char* commands[], int len){
    int pipes[len][2];
    for(int i=0;i<len;i++){
        if(pipe(pipes[i])<0){
            printf("error with pipe!\n");
            exit(11);
        }
    }
    printf("======\n");
    for(int i=0;i<len;i++) {
        pid_t pid = fork();
        if (pid < 0) {
            printf("cant fork\n");
            exit(12);
        }
        if (pid == 0) {
            if (i != 0) {
                dup2(pipes[i - 1][0], STDIN_FILENO);
            }
            if (i != len - 1) {
                dup2(pipes[i][1], STDOUT_FILENO);
            }
            for (int j = 0; j < len - 1; j++) {
                close(pipes[j][0]);
                close(pipes[j][1]);
            }
            char *args[cmd_len_max];
            args[0] = strtok(commands[i], " ");
            for (int r = 1; r < cmd_len_max; r++) {
                args[r] = strtok(NULL, " ");
            }

            execvp(args[0], args);
            exit(EXIT_SUCCESS);
        }
    }
}

void parse_commands(int all_cmds,int commands_n, int orders[commands_n-all_cmds][10], char*** commands){
    for(int i=commands_n-all_cmds-1;i>=0;i--) {
        int j = 0;
        int ctr=0;
        char** parsed = calloc(all_cmds*cmd_len_max,1);
        while(orders[i][j]>-1){
            int k = 0;
            while(k<cmd_len_max && strcmp(commands[orders[i][j]][k]," ")){
                parsed[ctr] = calloc(max_len,1);
                strcpy(parsed[ctr],commands[orders[i][j]][k]);
                ctr++;
                k++;
            }
            j++;
        }
        run_commands(parsed, ctr);
        free(parsed);
    }
}

int main(int argc, char** argv){
    if(argc>2){
        printf("Too many args!\n");
        return -1;
    }
    char* path = argv[1];
    FILE* f= fopen(path, "r");
    if(f==NULL){
        printf("error!\n");
        return -1;
    }
    char* buffor = calloc(1,2048);

    fseek(f, 0, SEEK_END);
    size = ftell(f);
    fseek(f, 0, SEEK_SET);
    fread(buffor,size, 1,f);
    fclose(f);

    printf("\n%s\n---\n",buffor);

    int commands_n = 0;
    char *c = buffor;

    int curr_len = 0, all_cmds=0, cmd_len=1;
    max_len = 0;
    cmd_len_max=0;
    while(*c!='\0'){
        if(*c=='|'){
            cmd_len+=1;
        }
        if(*c=='\n'){
            if(cmd_len_max<cmd_len)
                cmd_len_max=cmd_len;
            cmd_len=1;
            commands_n+=1;
            if(curr_len>max_len){
                max_len=curr_len;
            }
            if(curr_len==0)
                all_cmds=commands_n-1;
            curr_len=0;
        }
        else
            curr_len+=1;
        c++;
    }

    char eq = '=';
    char stp = '|';
    char end = '\n';

    char *** commands = calloc(commands_n+1,1);
    int orders[commands_n-all_cmds][10];
    for(int i=0;i<commands_n-all_cmds;i++)
        for(int j=0;j<10;j++)
            orders[i][j]=-1;
    for(int i=0;i<commands_n;i++) {
        commands[i] = calloc(cmd_len_max + 1, 1);
        for (int j = 0; j < cmd_len_max; j++) {
            commands[i][j] = calloc(max_len + 1, 1);
            commands[i][j] = " ";
        }
    }
    char* cmd=strtok(buffor,&stp);
    int row = -1;
    int col;
    while(cmd!=NULL){
        if(cmd[0]!=' '){
            row+=1;
            col=0;
            if(row>=all_cmds){
                if(cmd[0]==' '){
                    cmd+=9;
                }
                else{
                    cmd+=8;
                }
                orders[row-all_cmds][col] = atoi(cmd)-1;
                col+=1;
            }
        }
        else{
            if(row>=all_cmds){
                if(cmd[0]==' '){
                    cmd+=9;
                }
                else{
                    cmd+=8;
                }
                orders[row-all_cmds][col] = atoi(cmd)-1;
            }
            else {
                commands[row][col] = cmd;
                printf("|%d|%d|:%s\n", row, col, commands[row][col]);
            }
            col+=1;
        }
        cmd=strtok(NULL,&stp);
    }
    parse_commands(all_cmds,commands_n, orders, commands);
    free(commands);
    free(buffor);
    return 0;
}