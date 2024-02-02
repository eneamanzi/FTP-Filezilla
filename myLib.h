#ifndef COMMANDS_H
#define COMMANDS_H
#define NUMBER_OF_COMMANDS 14
#define BUFFER_SIZE 2048

extern const char* mCmd[NUMBER_OF_COMMANDS];


typedef struct 
{
    char  username[256];
    char  password[256];
    int data_socket;            //socket per scambio dati (retr/store/list)
    int server_data;            //socket server creata dalla PASV per accettare connessioni
    char current_working_dir[2048];
}Session;

typedef struct 
{
    char *name;
    int (*commandFunc)(int ,char*,Session*);
}COMMANDS;

#endif