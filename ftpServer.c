//librerie "base"
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <pthread.h>


#include <semaphore.h>

//da usare
#include <signal.h> 

//nostre librerie
#include "colors.h"
#include "myLib.h"
#include "handlers.h"

COMMANDS mComands[] = {
    {"USER", handle_USER_Command}, {"PASS", handle_PASS_Command},
    {"PORT", handle_PORT_Command}, {"LIST", handle_LIST_Command},
    {"PWD", handle_PWD_Command,},   {"CWD", handle_CWD_Command},
    {"DELE", handle_DELE_Command}, {"RETR", handle_RETR_Command},
    {"STOR", handle_STOR_Command}, {"MKD", handle_MKD_Command},
    {"RMD", handle_RMD_Command},   {"TYPE", handle_TYPE_Command},
    {"QUIT", handle_QUIT_Command}, {"PASV", handle_PASV_Command}
    };

pthread_mutex_t mutex;

int port = 0;
int countCommands = 0;
pthread_cond_t cond_full;
pthread_cond_t cond_empty;

typedef struct logInfo{
  char command [BUFFER_SIZE][BUFFER_SIZE];
  Session *state;
}logInfo;
logInfo logs;

char logBuffer[BUFFER_SIZE][BUFFER_SIZE];
void *handle_client(void *client_fdIn);
void *log_Message();


//Creazione server e creazione thread per ogni client
int main(int argc, char *argv[]) {
  pthread_mutex_init(&mutex, NULL);
  int server_fd;
  int enableOpt =1;
  long client_fd;   //creato come long perchè sizeof(long)=sizeof(void *)
  struct sockaddr_in server_addr, client_addr;
  if (argc != 2) {
    printf("Please enter Port number\n");
    return 1;
  }
  if (argv[1] != NULL)
    port = atoi(argv[1]);
  // Create a socket
  if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
    perror("socket");
    exit(EXIT_FAILURE);
  }

  server_addr.sin_family = AF_INET;
  server_addr.sin_addr.s_addr = INADDR_ANY;
  server_addr.sin_port = htons(port);

  /* Address can be reused instantly after program exits */
  setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &enableOpt, sizeof(enableOpt));

  // Bind the socket
  if (bind(server_fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
    perror("bind");
    exit(EXIT_FAILURE);
  }

  // Listen for connections
  if (listen(server_fd, 5) < 0) {
    perror("listen");
    exit(EXIT_FAILURE);
  }

  socklen_t addr_len = sizeof(client_addr);
  pthread_t consumer;
  pthread_cond_init(&cond_empty,NULL);
  pthread_cond_init(&cond_full,NULL);
  pthread_create(&consumer,NULL,log_Message,NULL);

  printf(YELLOW);
  printf("Waiting for incoming connection\n");
  printf(RESET);

  while (1) {
    // Accept a connection
        if ((client_fd = accept(server_fd, (struct sockaddr *)&client_addr, &addr_len)) < 0) {
      perror("accept");
      exit(EXIT_FAILURE);
    }
   
    printf(RESET);
    printf(GREEN);
    printf("Connection accepted from %s:%d\n", inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port));
    printf(RESET);
    pthread_t clientThread;
    pthread_create(&clientThread, NULL, handle_client, (void *)client_fd);
  }

  close(server_fd);
  return 0;
}

//Funzione di gestione per ogni thread generato -_> si occupa di invocare le funzioni per i rispettivi comandi
void *handle_client(void *client_fdIn) {
  int client_fd = (long)client_fdIn;
  printf("Client File Descriptor %d \n", client_fd);
  char buffer[BUFFER_SIZE];
  memset(buffer, 0, sizeof(buffer));
  int read_bytes;
  Session state;

  char *cwd = getenv("HOME");   //prendo la HOME dell'utente che esegue il server
  //getcwd(mCwd, sizeof(mCwd)); //funzione C per directory corrente

  memset(&logs,0,sizeof(logs));
  memset(&state, 0, sizeof(Session));

  strncpy(state.current_working_dir, cwd, strlen(cwd)+1);
  chdir(cwd);       //cambio la CW alla home

  // Send welcome message
  snprintf(buffer, BUFFER_SIZE, "220 Welcome to the simple FTP server.\r\n");
  send(client_fd, buffer, strlen(buffer), 0);
  
  while ((read_bytes = recv(client_fd, buffer, BUFFER_SIZE, 0)) > 0) {
    // Ensure the buffer is null-terminated
    //printf("%d", read_bytes);
    buffer[read_bytes] = '\0';
    char cmd[5];
    cmd[4]='\0';
    sscanf(buffer, "%4s", cmd);
    
    pthread_mutex_lock(&mutex);
    while(countCommands >= 10)
    {
        printf("Wait for new command\n");
        pthread_cond_wait(&cond_empty,&mutex);
    }
    strcpy(logs.command[countCommands],cmd);
    logs.state = &state;
    countCommands++;
    pthread_mutex_unlock(&mutex);
    if(countCommands == 10)pthread_cond_signal(&cond_full);

   
    if (isValidCommand(cmd) != -1) {                //fa controllo nell'array di comandi
      //printf("(passato valid command %s %ld)\n", cmd, strlen(cmd));
      for (int i = 0; i < NUMBER_OF_COMMANDS; ++i) {
        if (strcasecmp(cmd, mComands[i].name) == 0) {     //cerca se il comando inviato dall'utente corrisponde a uno presente

          printf(BOLDRED);
          printf("%s", buffer);
          printf(RESET);
          mComands[i].commandFunc(client_fd, buffer, &state);   //commandFunc è elemento della struct definito come puntatore a funzione di tipo Int che accetta 3 parametri--> con questa sintassi invoco quella funzione di gestione
          break;
        }
      }
    } else {
      printf(WHITE);
      printf("\t(fallito valid command: %s %ld)\n", cmd, strlen(cmd));
      printf(RESET);
      snprintf(buffer, BUFFER_SIZE, "500 Unknown command.\r\n");
      send(client_fd, buffer, strlen(buffer), 0);
    }
  }
  printf("uscito client [%d]\n",client_fd);
  return 0;
}

//Funzione Thread di logging --> ogni 10 comandi (gestito da mutex)
void *log_Message(){
  FILE * fp = fopen("logs.txt","a");
  if(fp == NULL)printf("Problem Opening File\n");
  while(1)
  {
      pthread_mutex_lock(&mutex);
      while(countCommands < 10)
      {
          //printf("Still not \n");
          pthread_cond_wait(&cond_full,&mutex);
      }
      for(int i =0; i < countCommands;++i)
      {
        fprintf(fp, "[%s] : %s\n",logs.state->username ,logs.command[i]);
        fflush(fp);
      }
      countCommands = 0;
      pthread_mutex_unlock(&mutex);
      pthread_cond_signal(&cond_empty);
  }
}