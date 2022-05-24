#include "common.h"

static int client_queue_id;
static int server_queue_id;
static int client_index;
static int client_key;

int initialize_connection(key_t key){
    server_queue_id = create_msg_queue(SERVER_KEY_FILEPATH, PROJECT_ID, 0);
    Message message = { .type = INIT,
            .index = key};

    sprintf(&message.text[0], "%d", getpid());
    if(msgsnd(server_queue_id, &message, MESSAGE_SIZE, 0) == -1){
        perror("msgsnd initialize_connection");
        exit(EXIT_FAILURE);
    }

    if(msgrcv(client_queue_id, &message, MESSAGE_SIZE, INIT, 0) == -1){
        perror("msgrcv");
        exit(EXIT_FAILURE);
    }

    int ret = atoi(message.text);
    if(ret == -1){
        printf("No free slots\n");
        _Exit(0);
    }
    printf("My ID: %d\n", message.index);
    return message.index;
}

void stop_handler(){
    Message m;
    m.type = STOP;
    m.index = client_index;
    strcpy(m.text, "STOP");
    printf("Stops working\n");
    fflush(stdout);
    if(msgsnd(server_queue_id, &m, MESSAGE_SIZE, 0) == -1){
        perror("msgsnd stop_handler");
        exit(EXIT_FAILURE);
    }

    msgctl(client_queue_id, IPC_RMID, NULL);
    _Exit(0);
}

void list_handler(Message *m){
    m->type = LIST;
    if(msgsnd(server_queue_id, m, MESSAGE_SIZE, 0) == -1){
        perror("msgsnd manage LIST");
        exit(EXIT_FAILURE);
    }
}

void send_handler(Message *m){
    if(m->receiver==-2)
        m->type = ALL;
    else
        m->type = ONE;
    if(msgsnd(server_queue_id, m, MESSAGE_SIZE, 0) == -1){
        perror("msgsnd send_handler SEND");
        exit(EXIT_FAILURE);
    }
}

void manage(Message *m, char *buf) {
    if (starts_with(buf, "STOP")) {
        stop_handler();
    } else if (starts_with(buf, "LIST")) {
        list_handler(m);
    } else if (starts_with(buf, "2ALL")) {
        m->receiver = -2;
        send_handler(m);
    } else if (starts_with(buf, "2ONE")) {
        m->receiver = atoi(&buf[5]);
        send_handler(m);
    }
}

void sender(){
    Message message;
    char* line = NULL;
    size_t len = 0;
    getline(&line, &len, stdin);
    char* cmd = strtok(line, "\n");
    if((strcpy(&message.text[0],cmd))!=NULL){
        message.index = client_index;
        manage(&message, &message.text[0]);
    }
}

void receiver(){
    Message message;
    while(!is_empty(client_queue_id)){
        if(msgrcv(client_queue_id, &message, MESSAGE_SIZE, 0, IPC_NOWAIT)!=-1){
            switch (message.type) {
                case STOP: {
                    stop_handler();
                    break;
                }
                case LIST: {
                    printf("%s", message.text);
                    fflush(stdout);
                    break;
                }
                case ALL: {
                    char *mess = malloc(MAX_MESSAGE_LEN);
                    strcpy(mess, message.text);
                    mess += 5;
                    printf("==========General message from: client %d==========\n",message.index);
                    printf("%s\n", mess);
                    printf("==============================\n");
                    fflush(stdout);
                    break;
                }
                case ONE: {
                    char *mess = malloc(MAX_MESSAGE_LEN);
                    strcpy(mess, message.text);
                    mess += 7;
                    printf("==========Private message from: client %d==========\n",message.index);
                    printf("%s\n", mess);
                    printf("==============================\n");
                    fflush(stdout);
                    break;
                }
            }
        }
    }
}

int main(int argc, char *argv[]){
    atexit(stop_handler);
    signal(SIGINT, stop_handler);
    signal(WAKEUP_SIGNAL, receiver);

    const char *home_dir = getenv("HOME");
    if(home_dir == NULL){
        home_dir = argv[0];
    }

    client_queue_id = create_msg_queue(home_dir, getpid(), IPC_CREAT);

    if((client_key = ftok(home_dir, getpid())) == -1){
        perror("ftok");
        exit(EXIT_FAILURE);
    }
    client_index = initialize_connection(client_key);
    while(true){
        sender();
        usleep(100000);
        receiver();
        usleep(100000);
    }
}

