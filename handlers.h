#include "myLib.h"
#include <dirent.h>
#include <grp.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <pwd.h>
#include <string.h>
//librerie per socket
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
//
#include <time.h>
#include <unistd.h> //funzioni linux sistema
#include <ctype.h> // verifica dei tipi --> isspace

int handle_USER_Command(int client_socket, char *bufferIn, Session *state);
int handle_PASS_Command(int client_socket, char *bufferIn, Session *state);
int handle_PASV_Command(int client_socket, char *bufferIn, Session *state);
int handle_LIST_Command(int client_socket, char *bufferIn, Session *state);
int handle_PWD_Command(int client_socket, char *bufferIn, Session *state);
int handle_CWD_Command(int client_socket, char *bufferIn, Session *state);
int handle_PORT_Command(int client_socket, char *bufferIn, Session *state);
int handle_DELE_Command(int client_socket, char *bufferIn, Session *state);
int handle_RETR_Command(int client_socket, char *bufferIn, Session *state);
int handle_STOR_Command(int client_socket, char *bufferIn, Session *state);
int handle_MKD_Command(int client_socket, char *bufferIn, Session *state);
int handle_TYPE_Command(int client_socket, char *bufferIn, Session *state);
int handle_RMD_Command(int client_socket, char *bufferIn, Session *state);
int handle_STOR_Command(int client_socket, char *bufferIn, Session *state);


int create_data_connection(unsigned char ip[4], int port);
void file_mode_string(mode_t mode, char *str);
void removeSpaces(char *str);