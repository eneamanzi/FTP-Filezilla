#ifndef COMMANDS_H
#define COMMANDS_H
#define NUMBER_OF_COMMANDS 13
#define BUFFER_SIZE 2048

typedef enum 
{
    PWD=0,
    CWD,
    LIST,
    RETR,
    DELE,
    MKD,
    RMD,
    PORT,
    QUIT
}Commands;
extern const char* mCmd[NUMBER_OF_COMMANDS];


typedef struct 
{
    char  username[256];
    char  password[256];
    int data_socket;
    char current_working_dir[2048];
}Session;

typedef struct 
{
    char *name;
    int (*commandFunc)(int ,char*,Session*);
}COMMANDS;

#endif