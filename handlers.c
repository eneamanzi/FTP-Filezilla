#include "handlers.h"

int isValidCommand(char *commandIn) {
  for (int i = 0; i < NUMBER_OF_COMMANDS; i++)
    if (strcasecmp(commandIn, mCmd[i]) == 0){
      return 1;
    }
  return -1;
}


void file_mode_string(mode_t mode, char *str) {
  static const char *rwx[] = {"---", "--x", "-w-", "-wx", "r--", "r-x", "rw-", "rwx"};

  str[0] = S_ISDIR(mode) ? 'd' : '-';
  strcpy(&str[1], rwx[(mode >> 6) & 7]);
  strcpy(&str[4], rwx[(mode >> 3) & 7]);
  strcpy(&str[7], rwx[mode & 7]);

  if (mode & S_ISUID)
    str[3] = (mode & S_IXUSR) ? 's' : 'S';
  if (mode & S_ISGID)
    str[6] = (mode & S_IXGRP) ? 's' : 'S';

  str[10] = '\0';
}


void removeSpaces(char *str) {
  char *i = str;
  char *j = str;
  while (*j != '\0') {
    if (!isspace(*j)) {
      *i = *j;
      i++;
    }
    j++;
  }
  *i = '\0';
}


//DA QUI INIZIANO LE IMPLEMENTAZIONI DELLE SINGOLE FUNZIONI //

//TUTTO OK
int handle_USER_Command(int client_socket, char *bufferIn, Session *state) {
  char response[256];
  memset(response, 0, sizeof(response));
  strncpy(state->username, bufferIn + 5, strlen(bufferIn + 4));
  state->username[strlen(state->username)] = '\0';
  removeSpaces(state->username);
  //TODO controllo username
  if (0) {
    snprintf(response, strlen("530 Not logged in. \r\n"), "530 Not logged in. \r\n");
    send(client_socket, response, strlen(response), 0);
    return -1;
  }
  snprintf(response, strlen("331 User name okay, need password.\r\n")+1, "331 User name okay, need password.\r\n");
  send(client_socket, response, strlen(response), 0);
  return 0;
}

//TUTTO OK
int handle_PASS_Command(int client_socket, char *bufferIn, Session *state) {
  if (state == NULL)
    printf("null re\n");
  strncpy(state->password, bufferIn + 4, strlen(bufferIn + 4));
  removeSpaces(state->password);

  //TODO client authentication
  if (1)
    snprintf(bufferIn, BUFFER_SIZE, "230 User logged in, proceed.\r\n");
  else
    snprintf(bufferIn, BUFFER_SIZE, "530 Not logged in.\r\n");
  send(client_socket, bufferIn, strlen(bufferIn), 0);
  return 0;
}

//TUTTO OK
int handle_PORT_Command(int client_socket, char *bufferIn, Session *state) {
  char ip[4];
  unsigned char p1, p2;
  sscanf(bufferIn, "PORT %hhu,%hhu,%hhu,%hhu,%hhu,%hhu", &ip[0], &ip[1], &ip[2],&ip[3], &p1, &p2);
  int data_port = p1 * 256 + p2;
  
  state->data_socket = create_data_connection((unsigned char*) ip, data_port);
  if (state->data_socket < 0) {
    snprintf(bufferIn, BUFFER_SIZE, "425 Can't open data connection.\r\n");
    send(client_socket, bufferIn, strlen(bufferIn), 0);
  } else {
    snprintf(bufferIn, BUFFER_SIZE, "200 PORT command successful.\r\n");
    send(client_socket, bufferIn, strlen(bufferIn), 0);
  }
  return state->data_socket;
}

int create_data_connection(unsigned char ip[4], int port) {
  int data_socket = socket(AF_INET, SOCK_STREAM, 0);
  if (data_socket < 0) {
    perror("socket");
    return -1;
  }

  struct sockaddr_in client_addr;
  client_addr.sin_family = AF_INET;
  client_addr.sin_port = htons(port);
  client_addr.sin_addr.s_addr = htonl((ip[0] << 24) | (ip[1] << 16) | (ip[2] << 8) | ip[3]);

  if (connect(data_socket, (struct sockaddr *)&client_addr, sizeof(client_addr)) < 0) {
    perror("connect");
    close(data_socket);
    return -1;
  }
  return data_socket;
}
//

// SEMBRA TUTTO OK
int handle_PASV_Command(int client_socket, char *bufferIn, Session *state) {
  //if(state->logged_in){
  int ip[4];
  int portPart[2];
  int port;
  char buff[255];
  char *response = "227 Entering Passive Mode (%d,%d,%d,%d,%d,%d)\n";
  
  srand(time(NULL));
  portPart[0] = 128 + (rand()%64);
  portPart[1] = (rand()%255);
  port = portPart[0]*256 + portPart[1];

  /* Close previous passive socket */
  close(state->data_socket);

  /* Start listening here, but don't accept the connection */
  state->data_socket = create_socket_pasv(port);

  //prendo IP dalla socket client
  socklen_t addr_size = sizeof(struct sockaddr_in);
  struct sockaddr_in addr;
  getsockname(client_socket, (struct sockaddr *)&addr, &addr_size);   //salvo indirizzo formattato dentro addr
  char* host = inet_ntoa(addr.sin_addr);
  sscanf(host,"%d.%d.%d.%d",&ip[0],&ip[1],&ip[2],&ip[3]);
 
  //filla il messaggio definito in response e lo mette dentro buff
  sprintf(buff,response,ip[0],ip[1],ip[2],ip[3],portPart[0],portPart[1]);
  
  printf("\tPort:%d = %d * 256 + %d\n", port, portPart[0], portPart[1]);
  printf("\t%s", buff);

  send(client_socket, buff, strlen(buff), 0);

  /*}else{
    state->message = "530 Please login with USER and PASS.\n";
    printf("%s",state->message);
  }*/
  return 0;
}

int create_socket_pasv(int port){
  int sock;
  int reuse = 1;

  if((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0){
    fprintf(stderr, "Cannot open socket");
    exit(EXIT_FAILURE);
  }

  /* Server addess */
  struct sockaddr_in address;
  address.sin_family = AF_INET;
  address.sin_addr.s_addr = INADDR_ANY;
  address.sin_port = htons(port);

  /* Address can be reused instantly after program exits */
  setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof reuse);

  /* Bind socket to server address */
  if(bind(sock,(struct sockaddr*) &address, sizeof(address)) < 0){
    fprintf(stderr, "Cannot bind socket to address");
    exit(EXIT_FAILURE);
  }

  listen(sock,5);
  return sock;
}

//TUTTO OK
int handle_LIST_Command(int client_socket, char *bufferIn, Session *state) {
  DIR *dir;
  struct dirent *entry;
  char entry_buffer[BUFFER_SIZE];
  dir = opendir(state->current_working_dir);
  //errore
  if (dir == NULL) {
    perror("opendir");
    snprintf(bufferIn, BUFFER_SIZE, "550 Failed to open directory.\r\n");
    send(client_socket, bufferIn, strlen(bufferIn), 0);
    close(state->data_socket);
    return -1;
  //funzionamento
  } else {
    snprintf(bufferIn, BUFFER_SIZE, "150 Opening ASCII mode data connection for file list.\r\n");
    send(client_socket, bufferIn, strlen(bufferIn), 0);

    while ((entry = readdir(dir)) != NULL) {
      struct stat file_stat;
      char filepath[BUFFER_SIZE];

      snprintf(filepath, BUFFER_SIZE*2, "%s/%s", state->current_working_dir,entry->d_name); //????????????????????????????????
      if (stat(filepath, &file_stat) == 0) {
        char file_mode[11];
        file_mode_string(file_stat.st_mode, file_mode);
        struct passwd *user_info = getpwuid(file_stat.st_uid);
        struct group *group_info = getgrgid(file_stat.st_gid);
        
        char time_buffer[80];
        strftime(time_buffer, sizeof(time_buffer), "%b %d %H:%M",localtime(&(file_stat.st_mtime)));

        snprintf(entry_buffer, BUFFER_SIZE, "%s %ld %s %s %lld %s %s\r\n",
                 file_mode, (long)file_stat.st_nlink, user_info->pw_name,
                 group_info->gr_name, (long long)file_stat.st_size, time_buffer,entry->d_name);

        send(state->data_socket, entry_buffer, strlen(entry_buffer), 0);
      }
    }

    snprintf(bufferIn, BUFFER_SIZE, "226 Transfer complete.\r\n");
    send(client_socket, bufferIn, strlen(bufferIn), 0);
    closedir(dir);
    close(state->data_socket);
    return 0;
  }
}

//TUTTO OK
int handle_PWD_Command(int client_socket, char *bufferIn, Session *state) {
  char cwd[BUFFER_SIZE];
  getcwd(cwd, sizeof(cwd));
  strcpy(state->current_working_dir, cwd);
  printf("\t%s\n", state->current_working_dir);
  snprintf(bufferIn, BUFFER_SIZE*2, "257 \"%s\" is the current directory.\r\n",cwd);
  send(client_socket, bufferIn, strlen(bufferIn), 0);
  return 0;
}

//TUTTO OK
int handle_CWD_Command(int client_socket, char *bufferIn, Session *state) {
  char tmpBuffer[BUFFER_SIZE*2];  //*2 preventivo --> risolve un warning sulla dimensione
  memset(tmpBuffer, 0, BUFFER_SIZE);
  memset(state->current_working_dir, 0, sizeof(state->current_working_dir));

  strncpy(state->current_working_dir, bufferIn + 4, strlen(bufferIn + 4));
  state->current_working_dir[strlen(state->current_working_dir)] = '\0';

//rimuove il \n poichè di solito la terminazione è \r\n  --> forse useless
  char *cr = strchr(state->current_working_dir, '\r');
  if (cr != NULL)
    *cr = '\0';

  if (chdir(state->current_working_dir) == -1)
    perror("chdir");
  printf("\t%s\n", state->current_working_dir);
  snprintf(tmpBuffer, BUFFER_SIZE*2, "250 Directory successfully changed to %s.\r\n", state->current_working_dir);
  send(client_socket, tmpBuffer, strlen(tmpBuffer), 0);
  return 0;
}

//TUTTO OK --> parametro: nome cartella
int handle_RMD_Command(int client_socket, char *bufferIn, Session *state) {
  char directoryName[BUFFER_SIZE];  
  char directoryPath[BUFFER_SIZE*2];  //*2 preventivo --> risolve un warning sulla dimensione
  char *response;
  memset(directoryName, 0, sizeof(directoryName));
  memset(directoryPath, 0, sizeof(directoryPath));

  memcpy(directoryName, bufferIn + 4, strlen(bufferIn + 4));
  sprintf(directoryPath, "%s%s%s", state->current_working_dir, "/",directoryName);
  removeSpaces(directoryPath);
  if (rmdir(directoryPath) == 0){
    response = "250 Directory removed successfully.\r\n";
    send(client_socket, response, strlen(response), 0);
    return 0;
  }else{
    response = "550 Failed to remove directory.\r\n";
    send(client_socket, response, strlen(response), 0);
    return -1;
  }
}

//TUTTO OK --> parametro: nome cartella
int handle_MKD_Command(int client_socket, char *bufferIn, Session *state) {
  char finalDir[BUFFER_SIZE*2]; //*2 preventivo --> risolve un warning sulla dimensione
  char *response = "550 Failed to create directory.\r\n";
  sprintf(finalDir, "%s%s%s", state->current_working_dir, "/", bufferIn + 4);
  removeSpaces(finalDir);
  if (mkdir(finalDir, 0777) == 0){
    response = "250 Directory created successfully.\r\n";
    send(client_socket, response, strlen(response), 0);
    return 0;
  }else{
    response = "550 Failed to create directory.\r\n";
    send(client_socket, response, strlen(response), 0);
    return -1;
  }
    
}

//TUTTO OK --> parametro: nome file
int handle_STOR_Command(int client_socket, char *bufferIn, Session *state) {
  char fileName[BUFFER_SIZE];
  char filePath[BUFFER_SIZE*2]; //*2 preventivo --> risolve un warning sulla dimensione
  const char *response;
  int read_bytes;
  memset(fileName, 0, BUFFER_SIZE);
  memset(filePath, 0, BUFFER_SIZE);

  strncpy(fileName, bufferIn + 4, strlen(bufferIn + 4));
  sprintf(filePath, "%s%s%s", state->current_working_dir, "/", fileName);
  removeSpaces(filePath);
  printf("\t%s\n", filePath);

  FILE *fp = fopen(filePath, "wb");
  if (fp == NULL) {
    printf("Unable to open the file\n");
    return 1;
  }
  response = "150 Ok to send data.\r\n";
  send(client_socket, response, strlen(response), 0);
  
  while ((read_bytes = recv(state->data_socket, bufferIn, BUFFER_SIZE, 0)) > 0)
    fwrite(bufferIn, 1, read_bytes, fp);
  response = "226 Transfer complete. \r\n";
  fclose(fp);
  send(client_socket, response, strlen(response), 0);
  close(state->data_socket);
  return 0;
}

//TUTTO SEMIOK --> parametro: nome file --> toglie spazi
int handle_DELE_Command(int client_socket, char *bufferIn, Session *state) {
  char fileToDelete[BUFFER_SIZE];
  char finalFile[BUFFER_SIZE*2];  ///*2 preventivo --> risolve un warning sulla dimensione
  strncpy(fileToDelete, bufferIn + 4, strlen(bufferIn + 4));

  char *cr = strchr(fileToDelete, '\r');
  if (cr != NULL)
    *cr = '\0';
  sprintf(finalFile, "%s%s%s", state->current_working_dir, "/", fileToDelete);
  removeSpaces(finalFile);
  printf("\t%s\n", finalFile);
  int res = remove(finalFile);
  printf("\tResult: %d\n", res);
  sprintf(bufferIn, "250 File deleted successfully.\r\n");
  send(client_socket, bufferIn, strlen(bufferIn), 0);
  return 0;
}


int handle_RETR_Command(int client_socket, char *bufferIn, Session *state) {
  //prendo il nome el file da buffer e setto variabili di utilizzo
  char fileName[BUFFER_SIZE];
  char filePath[BUFFER_SIZE*2];   ///*2 preventivo --> risolve un warning sulla dimensione
  memset(fileName, 0, BUFFER_SIZE);
  memset(filePath, 0, BUFFER_SIZE);
  strncpy(fileName, bufferIn + 4, strlen(bufferIn + 4));
  sprintf(filePath, "%s%s%s", state->current_working_dir, "/", fileName);
  removeSpaces(filePath);

  FILE *fp = fopen(filePath, "rb");
  if (fp == NULL)
    perror("Fopen");
  printf("\t%s\n", filePath);

  // Read and send file data
  char fileBuffer[BUFFER_SIZE];
  size_t bytes_read;
  while ((bytes_read = fread(fileBuffer, 1, BUFFER_SIZE, fp)) > 0) {
    int sended = send(state->data_socket, fileBuffer, bytes_read, 0);
    printf("\tBytes to read %d\n", sended);
  }
  fclose(fp);
  
  const char *response = "226 Transfer complete.\r\n";
  send(client_socket, response, strlen(response), 0);
  printf("\tUpload Finished\n");
  close(state->data_socket);
  return 0;
}

int handle_TYPE_Command(int client_socket, char *bufferIn, Session *state) {
  char type;
  
  sscanf(bufferIn, "TYPE %c", &type);

  //TODO
  //if (type == 'I') {
  if(1){
    snprintf(bufferIn, BUFFER_SIZE, "200 Type set to I.\r\n");
    send(client_socket, bufferIn, strlen(bufferIn), 0);
    return 0;
  } else {
    snprintf(bufferIn, BUFFER_SIZE, "504 Command not implemented for that parameter.\r\n");
    send(client_socket, bufferIn, strlen(bufferIn), 0);
    return -1;
  }
}

int handle_QUIT_Command(int client_socket, char *bufferIn, Session *state){
  snprintf(bufferIn, BUFFER_SIZE, "221 Goobye.\r\n");
  send(client_socket, bufferIn, strlen(bufferIn), 0);
  close(client_socket);
  return 0;
}