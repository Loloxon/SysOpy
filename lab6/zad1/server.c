#include "common.h"

static int server_queue_id;
ClientData clients[MAX_CLIENTS];

char* get_data(){
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);
    char* timestamp = calloc(1024,1);
    sprintf(timestamp, "\n%d-%02d-%02d %02d:%02d:%02d",
            tm.tm_year+1900, tm.tm_mon+1, tm.tm_mday, tm.tm_hour, tm.tm_min, tm.tm_sec);
    return timestamp;
}

void logs(Message* message){
    FILE* f = fopen("raport.txt", "a+");
    fprintf(f, "%s %s from client %d\n====\n", message->text, get_data(), message->index);
    fclose(f);
    return;
}

int get_free_index(){
    for(int i = 0; i < MAX_CLIENTS; i++){
        if(clients[i].status == NOT_CONNECTED){
            return i;
        }
    }
    return -1;
}

void init_handler(Message* message){
    int cid = message->index;
    int msgid;
    if ((msgid = msgget((key_t) cid, PERMISSIONS)) == -1) {
        perror("msgget INIT");
        return;
    }
    Message m = {.type = INIT};
    int free_client_index = get_free_index();
    if (free_client_index == -1) { //no free places
        sprintf(&m.text[0], "%d", -1);
        if (msgsnd(msgid, &m, MESSAGE_SIZE, 0) == -1) {
            perror("msgsnd CONNECT bad");
            return;
        }
        return;
    }

    clients[free_client_index].index = free_client_index;
    clients[free_client_index].status = CONNECTED;
    clients[free_client_index].que_id = msgid;
    clients[free_client_index].pid = atoi(message->text);
    m.index = clients[free_client_index].index;

    if (msgsnd(clients[free_client_index].que_id,
               &(Message) {.type = INIT, .index = clients[free_client_index].index, .text[0] = '\0'},
               MESSAGE_SIZE, 0) == -1) {
        perror("msgsnd INIT");
        return;
    }

    printf("> INIT | cid: %d, que_id: %d\n",
           clients[free_client_index].index,
           clients[free_client_index].que_id);
}

void stop_handler(Message* message){
    int cid = message->index;
    printf("> STOP | cid: %d\n", clients[cid].index);
    clients[cid].index = -1;
    clients[cid].status = NOT_CONNECTED;
    clients[cid].que_id = 0;
    clients[cid].pid = 0;
}

void list_handler(Message *message){
    int cid = message->index;
    printf("> LIST | cid: %d, que_id: %d\n", clients[cid].index, clients[cid].que_id);

    Message m = {.type = LIST, .index = cid};
    int len = 0;
    len += snprintf(&m.text[len], MAX_MESSAGE_LEN - len, "You         : %d -> %s\n", clients[cid].index,
                    kClientStatusStr[clients[cid].status]);
    for (int i = 0; i < MAX_CLIENTS; i++) {
        if (clients[i].status == CONNECTED && cid != i)
            len += snprintf(&m.text[len], MAX_MESSAGE_LEN - len, "Other client: %d -> %s\n",
                            clients[i].index, kClientStatusStr[clients[i].status]);
    }
    if (msgsnd(clients[cid].que_id, &m, MESSAGE_SIZE, 0) == -1) {
        perror("msgsnd LIST");
        return;
    }

}

void send_handler(Message *message, int rec){
    strcat(message->text,get_data());
    if(rec==0) {
        for (int i = 0; i < MAX_CLIENTS; i++) {
            if (clients[i].status == CONNECTED && message->index != i) {
                if (msgsnd(clients[i].que_id, message, MESSAGE_SIZE, 0) == -1) {
                    perror("msgsnd 2ALL");
                    return;
                }
                printf("> SENDING ALL | to: %d, que_id: %d, type: %ld\n", clients[i].index, clients[i].que_id, message->type);
                kill(clients[i].pid, WAKEUP_SIGNAL);
            }
        }
    }
    else{
        int receiver_index = atoi(&message->text[5]);
        if(receiver_index==message->index){
            printf("Cant send message to yourself!\n");
        }
        else {
            if (msgsnd(clients[receiver_index].que_id, message, MESSAGE_SIZE, 0) == -1) {
                perror("msgsnd 2ONE");
            }
            printf("> SENDING PV | to: %d, que_id: %d\n", clients[receiver_index].index,
                   clients[receiver_index].que_id);
            kill(clients[receiver_index].pid, WAKEUP_SIGNAL);
        }
    }
}

void handle_message(Message *message) {
    switch (message->type) {
        case INIT: {
            init_handler(message);
            break;
        }
        case LIST: {
            logs(message);
            list_handler(message);
            break;
        }
        case ALL:{
            logs(message);
            send_handler(message, 0);
            break;
        }
        case ONE:{
            logs(message);
            send_handler(message, 1);
            break;
        }
        case STOP: {
            logs(message);
            stop_handler(message);
            break;
        }
        default:
            break;
    }
}

void shutdown_clients(){
    Message m;
    for(int i = 0; i < MAX_CLIENTS; i++){
        if (clients[i].status == CONNECTED) {
//            m.text[0] = '\0';
            m.type = STOP;
            m.index = i;
            if (msgsnd(clients[i].que_id, &m, MESSAGE_SIZE, 0) == -1) {
                perror("msgsnd shutdown_clients");
                return;
            }
            usleep(50000);
            kill(clients[i].pid, SIGINT);
            usleep(50000);
            memset(&m, 0, sizeof(Message));
            if (msgrcv(server_queue_id, &m, MESSAGE_SIZE, STOP, 0) == -1) {
                perror("msgsnd shutdown_clients");
                return;
            }
            if (m.type == STOP) {
                printf("Shutting down client: %d\n", i);
            }
        }
    }

    msgctl(server_queue_id, IPC_RMID, NULL);
    _Exit(EXIT_SUCCESS);
}

int main(int argc, char *argv[]){
    atexit(shutdown_clients);
    signal(SIGINT, shutdown_clients);

    server_queue_id = create_msg_queue(SERVER_KEY_FILEPATH, PROJECT_ID, IPC_CREAT);

    Message message;
    memset(&clients[0], 0, MAX_CLIENTS * sizeof(ClientData));

    while(true){
        if( msgrcv(server_queue_id, &message, MESSAGE_SIZE, STOP, IPC_NOWAIT) == -1 &&
            msgrcv(server_queue_id, &message, MESSAGE_SIZE, LIST, IPC_NOWAIT) == -1 &&
            msgrcv(server_queue_id, &message, MESSAGE_SIZE, 0, IPC_NOWAIT) == -1){
            if(errno != ENOMSG){
                perror("msgrcv");
                exit(EXIT_FAILURE);
            }else{
                usleep(100000);
                continue;
            }
        }
        handle_message(&message);
    }
}