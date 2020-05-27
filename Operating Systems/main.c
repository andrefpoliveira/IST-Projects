/*
Projeto realizado por:
 - Andre Oliveira - ist193686
 - Pedro Xavier - ist193747
 Turno: Segunda-feira 16h
*/

//Sigmask
//Read

//#include <lcsignal.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <string.h>
#include <ctype.h>
#include <sys/time.h>
#include <pthread.h>
#include <semaphore.h>
#include <sys/mount.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>
#include <sys/wait.h>
#include "fs.h"
#include "lib/hash.h"
#include "sema.h"
#include "lib/inodes.h"
#include "tecnicofs-client-api.h"
#include "unix.h"
#include "tecnicofs-api-constants.h"

#define MAX_COMMANDS 10
#define MAX_INPUT_SIZE 100
#define MAX_OPENED_FILES 5
#define MAXLINE 512

int sockfd, /*newsockfd,*/ childpid, servlen;
socklen_t clilen;
struct sockaddr_un cli_addr, serv_addr;

pthread_t* clients;

int *newsockfd;
char *nomeSocket;
int numberBuckets = 0;
int numberThreads = 0;
tecnicofs* fs;

FILE* input = NULL;
FILE* output = NULL;

int proxIndLeitura = 0;
int proxIndEscrita = 0;

char inputCommands[MAX_COMMANDS][MAX_INPUT_SIZE];
int numberClients = 0;
int numberCommands = 0;
int headQueue = 0;

static void displayUsage (const char* appName){
    printf("Usage: %s nomeSocket outputFile numberOfBuckets\n", appName);
    exit(EXIT_FAILURE);
}

static void parseArgs (long argc, char* const argv[]){
    if (argc != 4) {
        perror("Invalid format:\n");
        displayUsage(argv[0]);
    }
}

int insertCommand(char* data) {
    strcpy(inputCommands[proxIndEscrita++], data);
    proxIndEscrita = proxIndEscrita % MAX_COMMANDS;  
    return 1;
}

char* removeCommand() {
    proxIndLeitura = proxIndLeitura % MAX_COMMANDS;

    //Se o comando for q, o endereco de leitura nao aumenta pois nao ha mais comandos
    if (!strcmp(inputCommands[proxIndLeitura], "q")) {
        return inputCommands[proxIndLeitura];
    }

    return inputCommands[proxIndLeitura++];
}

void errorParse(){
    perror("Error: command invalid\n");
    exit(EXIT_FAILURE);
}

//Verifica se o ficheiro de input existe e se a diretoria do ficheiro de output e' valida
void checkIfFileExists (FILE *file) {
    if (file == NULL) {
        perror("Um dos ficheiros introduzidos está incorreto");
        exit(EXIT_FAILURE);
    }
}

//Verifica se o numero de Buckets e valido
void validateNumberOfBuckets(int numBuckets) {
    if (numBuckets < 1) {
        perror("O numero de buckets nao e valido\n");
        exit(EXIT_FAILURE);
    }
}


int fileIsOpen(openedFile* openedFiles, int iNumber) {
    for (int i=0; i < MAX_OPENED_FILES; i++) {
        if (openedFiles[i].iNumber != -1) {
            if (openedFiles[i].iNumber == iNumber) {
                return i;
            }
        }
    }
    return -1;
}

int slotsAvailable(openedFile* openedFiles) {
    for (int i=0; i < MAX_OPENED_FILES; i++) {
        if (openedFiles[i].iNumber == -1) {
            return i;
        }
    }
    return -1;
}

int isTheOwner(uid_t user, int bucket) {
    uid_t owner;

    inode_get(0, &owner, 0, 0, NULL, 0, bucket);

    return user == owner;
}

int isOpen(int bucket) {
    return inode_get(0, NULL, NULL, NULL, NULL, 0, bucket);
}

int doesTheClientHavePermition(int iNumber, uid_t user, int* mode, int bucket) {
    uid_t owner;
    permission ownerPermitions;
    permission otherPermitions;

    inode_get(iNumber, &owner, &ownerPermitions, &otherPermitions, NULL, 0, bucket);

    if (user == owner) {
        return *mode == ownerPermitions || ownerPermitions == 3;
    } else {
        return *mode == otherPermitions || otherPermitions == 3;
    }
}

int writeInFile(int iNumber, char* text, int bucket) {
    return inode_set(iNumber, text, strlen(text), bucket);
}

int readFromFile(int iNumber, int len, int bucket, char* fileContents) {
    inode_get(iNumber, NULL, NULL, NULL, fileContents, len, bucket);
    fileContents[len-1] = 0;
    return strlen(fileContents);
}

void* processClient(void* newsockfd) {

    sigset_t mask;

    if (sigemptyset(&mask) == -1) {
        perror("Sigemptyset error");
        exit(EXIT_FAILURE);
    }

    if (sigaddset(&mask, SIGINT) == -1) {
        perror("Sigaddset error");
        exit(EXIT_FAILURE);
    }

    if (pthread_sigmask(SIG_BLOCK, &mask, NULL) < 0) {
        perror("Sigaddset error");
        exit(EXIT_FAILURE);
    }

    int n;
    char line[MAXLINE];
    openedFile openedFiles[MAX_OPENED_FILES];
    char* text = NULL;
    int sockfd = *((int*)newsockfd);

    free(newsockfd);

    for (int i=0; i < MAX_OPENED_FILES; i++) {
        openedFiles[i].iNumber = -1;
        openedFiles[i].mode = 0;
        openedFiles[i].fileName = NULL;
    }

    char token;
    int *mode = NULL;
    char firstTextArgument[MAX_INPUT_SIZE];
    char secondTextArgument[MAX_INPUT_SIZE];
    int firstIntArgument[20];
    int secondIntArgument[20];
    int slotOpenedFile;
    int slotAvailable;
    int numberChar;

    returnOutput(sockfd, 0);

    for (;;) {
        writeLock('c',0);
        printf("Antes do read\n");
        n = read(sockfd,line,MAXLINE);
        printf("Dps do read\n");
        if (n==0) {
            unlock('c',0);
            return NULL;
        }
        else if (n < 0) {
            unlock('c',0);
            perror("str_echo: Readline error\n");
            exit(EXIT_FAILURE);
        }
        line[n] = '\0';

        int iNumber;
        int bucket;
        int searchResult;
        uid_t userID;
        socklen_t socklen = sizeof(userID);
        
        getsockopt(sockfd, SOL_SOCKET, SO_PEERCRED, &userID, &socklen);

        char* resultOfRead;

        text = strtok(line, "\n");

        while(text != NULL) {
            printf("%s\n",text);
            switch(text[0]) {
                case 'c':
                    sscanf(text, "%c %s %d", &token, firstTextArgument, firstIntArgument);
                    bucket = hash(firstTextArgument, numberBuckets);
                    unlock('c',0);

                    readLock('t', bucket);
                    searchResult = lookup(fs, firstTextArgument, bucket);
                    unlock('t', bucket);

                    if (searchResult == -1) {
                        writeLock('t', bucket);
                        iNumber = inode_create(userID, *firstIntArgument/10, *firstIntArgument%10, bucket);
                        create(fs, firstTextArgument, iNumber, bucket);
                        returnOutput(sockfd, 0);
                        unlock('t', bucket);
                    } else {
                        returnOutput(sockfd, TECNICOFS_ERROR_FILE_ALREADY_EXISTS);
                    }
                    break;
                case 'd':
                    sscanf(text, "%c %s", &token, firstTextArgument);
                    bucket = hash(firstTextArgument, numberBuckets);
                    unlock('c',0);

                    readLock('t', bucket);
                    searchResult = lookup(fs, firstTextArgument, bucket);
                    unlock('t', bucket);

                    if (searchResult != -1) {
                        if(isTheOwner(userID, bucket)) {
                            if (isOpen(bucket) == 0) {
                                writeLock('t', bucket);
                                delete(fs, firstTextArgument, bucket);
                                inode_delete(0, bucket);
                                unlock('t', bucket);
                                returnOutput(sockfd, 0);
                            } else {
                                returnOutput(sockfd, TECNICOFS_ERROR_FILE_IS_OPEN);
                            }
                        } else {
                            returnOutput(sockfd, TECNICOFS_ERROR_PERMISSION_DENIED);
                        }
                    } else {
                        returnOutput(sockfd, TECNICOFS_ERROR_FILE_NOT_FOUND);
                    }
                    break;
                case 'r':
                    sscanf(text, "%c %s %s", &token, firstTextArgument, secondTextArgument);
                    command_r(fs, firstTextArgument, secondTextArgument, numberBuckets);
                    break;
                case 'o':
                    mode = malloc(sizeof(int));
                    sscanf(text, "%c %s %d", &token, firstTextArgument, mode);
                    bucket = hash(firstTextArgument, numberBuckets);
                    unlock('c',0);

                    readLock('t', bucket);
                    searchResult = lookup(fs, firstTextArgument, bucket);
                    unlock('t', bucket);

                    if (searchResult != -1) {
                        
                        if (doesTheClientHavePermition(0, userID, mode, bucket)) {

                            slotOpenedFile = fileIsOpen(openedFiles, searchResult);
                            if (slotOpenedFile < 0) {

                                slotAvailable = slotsAvailable(openedFiles);
                                if (slotAvailable >= 0) {
                                        
                                    openedFile file;
                                    file.fileName = malloc(sizeof(char)*(strlen(firstTextArgument)+1));
                                    strcpy(file.fileName,firstTextArgument);
                                    file.iNumber = searchResult;
                                    file.mode = *mode;
                                    openedFiles[slotAvailable] = file;
                                    inode_increase_openedFiles(searchResult, bucket);
                                    returnOutput(sockfd,searchResult);

                                } else {
                                    returnOutput(sockfd,TECNICOFS_ERROR_MAXED_OPEN_FILES);
                                }

                            } else {
                                returnOutput(sockfd, TECNICOFS_ERROR_FILE_IS_OPEN);
                            }
                        } else {
                            returnOutput(sockfd, TECNICOFS_ERROR_PERMISSION_DENIED);
                        }
                    } else {
                        returnOutput(sockfd, TECNICOFS_ERROR_FILE_NOT_FOUND);
                    }
                    free(mode);
                    break;
                case 'x':
                    sscanf(text, "%c %d", &token, firstIntArgument);
                    unlock('c',0);

                    if (openedFiles[*firstIntArgument].iNumber != -1) {
                        inode_decrease_openedFiles(openedFiles[*firstIntArgument].iNumber, bucket);

                        openedFiles[*firstIntArgument].iNumber = -1;
                        openedFiles[*firstIntArgument].mode = 0;
                        free(openedFiles[*firstIntArgument].fileName);
                        openedFiles[*firstIntArgument].fileName = NULL;
                        
                        returnOutput(sockfd, 0);
                    } else {
                        returnOutput(sockfd, TECNICOFS_ERROR_FILE_NOT_OPEN);
                    }
                    break;
                case 'l':
                    sscanf(text, "%c %d %d", &token, firstIntArgument, secondIntArgument);
                    unlock('c',0);

                    if (openedFiles[*firstIntArgument].iNumber != -1) {

                        bucket = hash(openedFiles[*firstIntArgument].fileName, numberBuckets);

                        writeLock('t', bucket);
                        
                        if ((openedFiles[*firstIntArgument].mode == 2) || (openedFiles[*firstIntArgument].mode == 3)) {

                            resultOfRead = malloc((*secondIntArgument+1)*sizeof(char));
                            numberChar = readFromFile(openedFiles[*firstIntArgument].iNumber, *secondIntArgument, bucket, resultOfRead);
                            if (numberChar != -1) {
                                unlock('t', bucket);
                                returnResultOfRead(sockfd, numberChar, resultOfRead);
                            } else {
                                unlock('t', bucket);
                                returnOutput(sockfd, TECNICOFS_ERROR_OTHER);
                            }
                        } else {
                            unlock('t', bucket);
                            returnOutput(sockfd, TECNICOFS_ERROR_INVALID_MODE);
                        }

                    } else {
                        returnOutput(sockfd, TECNICOFS_ERROR_FILE_NOT_OPEN);
                    }
                    
                    break;
                case 'w':
                    sscanf(text, "%c %d %s", &token, firstIntArgument, firstTextArgument);
                    unlock('c',0);

                    if (openedFiles[*firstIntArgument].iNumber != -1) {

                        bucket = hash(openedFiles[*firstIntArgument].fileName, numberBuckets);

                        writeLock('t', bucket);

                        if ((openedFiles[*firstIntArgument].mode == 1) || (openedFiles[*firstIntArgument].mode == 3)) {

                            if (writeInFile(openedFiles[*firstIntArgument].iNumber, firstTextArgument, bucket) == 0) {
                                unlock('t', bucket);
                                returnOutput(sockfd, 0);
                            } else {
                                unlock('t', bucket);
                                returnOutput(sockfd, TECNICOFS_ERROR_OTHER);
                            }
                        } else {
                            unlock('t', bucket);
                            returnOutput(sockfd, TECNICOFS_ERROR_INVALID_MODE);
                        }

                    } else {
                        returnOutput(sockfd, TECNICOFS_ERROR_FILE_NOT_OPEN);
                    }

                    break;
                case 'q':
                    unlock('c', 0);
                    returnOutput(sockfd, 0);
                    return NULL;
                default:
                    unlock('c', 0);
                    exit(EXIT_FAILURE);
            }
            text = strtok(NULL, "\n");
        }
    }
    return NULL;
}

void serverUnmount (int s) {
    for (int i = 0; i < numberClients; i++) {
        if (pthread_join(clients[i], NULL)) {
            fprintf(stderr,"A thread com o id %d terminou de forma abrupta\n", i);
            exit(EXIT_FAILURE);
        }
    }

    free(clients);

    print_tecnicofs_tree(output, fs, numberBuckets);
    fclose(output);

    inode_table_destroy();
    free_tecnicofs(fs, numberBuckets);

    free(newsockfd);

    exit(EXIT_SUCCESS);
}

int serverMount(char * address) {
    signal(SIGINT, serverUnmount);

    //Criar socket
    if ((sockfd = socket(AF_UNIX, SOCK_STREAM,0)) < 0) {
        perror("Server: impossivel abrir socket stream\n");
        exit(EXIT_FAILURE);
    }

    //Elimina o nome caso exista
    unlink(/*UNIXSTR_PATH*/address);

    bzero((char*)&serv_addr, sizeof(serv_addr));
    serv_addr.sun_family = AF_UNIX;
    strcpy(serv_addr.sun_path, address);

    servlen = strlen(serv_addr.sun_path) + sizeof(serv_addr.sun_family);

    if (bind(sockfd, (struct sockaddr *) &serv_addr, servlen) < 0) {
        perror("Server: impossivel fazer bind do endereço local\n");
        exit(EXIT_FAILURE);
    }
    
    listen(sockfd,5);
    
    for (;;) {

        newsockfd = malloc(sizeof(int));
        
        clilen = sizeof(cli_addr);
        *newsockfd = accept(sockfd, (struct sockaddr*) &cli_addr, &clilen);
        if (*newsockfd < 0) {
            perror("Server: Erro ao aceitar\n");
            exit(EXIT_FAILURE);
        }

        clients = realloc(clients, sizeof(pthread_t) * (numberClients + 1));

        //Lanca processo filho para tratar do cliente
        if(pthread_create(&clients[numberClients++], 0, processClient, newsockfd)) {
            perror("server:impossivel criar a thread");
            exit(EXIT_FAILURE);
        }
    }
    
    //close(sockfd);
}

int main(int argc, char* argv[]) {
    parseArgs(argc, argv);

    nomeSocket = argv[1];

    output = fopen(argv[2], "w");
    checkIfFileExists(output);

    numberBuckets = atoi(argv[3]);
    validateNumberOfBuckets(numberBuckets);

    fs = new_tecnicofs(numberBuckets);
    inode_table_init();

    serverMount(nomeSocket);

    exit(EXIT_SUCCESS);
}
