#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <sys/un.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h> 
#include "unix.h"
#include "tecnicofs-api-constants.h"

#define MAXLINE 512

int sockfd, newsockfd, clilen, childpid, servlen;
struct sockaddr_un cli_addr, serv_addr;

int receiveOutput() {
    char line[5];

    if(read(sockfd,line,5) < 0) {
        perror("Client: erro no readOutput");
        exit(EXIT_FAILURE);
    }

    int result = atoi(line);
    printf("%d\n",result);
    bzero(line, sizeof(line));
    
    return result;
}

int receiveReadOutput(char* buffer) {
    char* text = NULL;
    char line[MAXLINE];

    if(read(sockfd,line,MAXLINE) < 0) {
        perror("Client: erro no readOutput");
        exit(EXIT_FAILURE);
    }

    text = strtok(line, " ");

    int result = atoi(text);
    printf("%d\n",result);

    if (result > 0) {
        text = strtok(NULL, " ");
        strcpy(buffer, text);
    }

    return result;
}

int tfsCreate(char *filename, permission ownerPermissions, permission othersPermissions) {
    if(sockfd < 0) {
        perror("tfsUnmount: Nao existe sessao ativa");
        exit(0);
    }

    char res[512];
    
    sprintf(res, "c %s %d\n", filename, ownerPermissions*10+othersPermissions);
    if(dprintf(sockfd, "%s", res) < 0) {
        perror("Client: erro ao enviar a mensagem com o output");
        exit(EXIT_FAILURE);
    }

    return receiveOutput();    
}

int tfsDelete(char *filename) {
    if(sockfd < 0) {
        perror("tfsUnmount: Nao existe sessao ativa");
        exit(0);
    }
    char res[MAXLINE];
    sprintf(res, "d %s\n", filename);

    if(dprintf(sockfd, "%s", res) < 0) {
        perror("Client: erro ao enviar a mensagem com o output");
        exit(EXIT_FAILURE);
    }

    return receiveOutput();
}

int tfsRename(char *filenameOld, char *filenameNew) {
    if(sockfd < 0) {
        perror("tfsUnmount: Nao existe sessao ativa");
        exit(0);
    }
    char res[MAXLINE];
    sprintf(res, "r %s %s\n", filenameOld, filenameNew);

    if(dprintf(sockfd, "%s", res) < 0) {
        perror("Client: erro ao enviar a mensagem com o output");
        exit(EXIT_FAILURE);
    }

    return receiveOutput();
}

int tfsOpen(char *filename, permission mode) {
    printf("1 open\n");
    if(sockfd < 0) {
        perror("tfsUnmount: Nao existe sessao ativa");
        exit(0);
    }
    printf("2 open\n");
    char res[MAXLINE];
    printf("2 open\n");
    sprintf(res, "o %s %d\n", filename, mode);

    printf("Antes do write\n");
    if(dprintf(sockfd, "%s", res) < 0) {
        perror("Client: erro ao enviar a mensagem com o output");
        exit(EXIT_FAILURE);
    }
    printf("Depois do write\n");

    return receiveOutput();
}

int tfsClose(int fd) {
    if(sockfd < 0) {
        perror("tfsUnmount: Nao existe sessao ativa");
        exit(0);
    }
    char res[MAXLINE];
    sprintf(res, "x %d\n", fd);


    if(dprintf(sockfd, "%s", res) < 0) {
        perror("Client: erro ao enviar a mensagem com o output");
        exit(EXIT_FAILURE);
    }

    return receiveOutput();
}

int tfsRead(int fd, char *buffer, int len) {
    if(sockfd < 0) {
        perror("tfsUnmount: Nao existe sessao ativa");
        exit(0);
    }

    char res[MAXLINE];
    sprintf(res, "l %d %d\n", fd, len);
    if(dprintf(sockfd, "%s", res) < 0) {
        perror("Client: erro ao enviar a mensagem com o output");
        exit(EXIT_FAILURE);
    }
    
    int result = receiveReadOutput(buffer);
    // if (result >= 0) {
    //     if(read(sockfd,buffer,len) < 0) {
    //         perror("Client: erro no receiveText");
    //         exit(EXIT_FAILURE);
    //     }

    //     printf("Buffer: %s\n", buffer);
    // }

    return result;
}

int tfsWrite(int fd, char *buffer, int len) {
    if(sockfd < 0) {
        perror("tfsUnmount: Nao existe sessao ativa");
        exit(0);
    }

    char res[MAXLINE];
    sprintf(res, "w %d %.*s\n", fd, len, buffer);
    if(dprintf(sockfd, "%s", res) < 0) {
        perror("Client: erro ao enviar a mensagem com o output");
        exit(EXIT_FAILURE);
    }
    
    return receiveOutput();
}

int tfsMount(char* address) {

    if ((sockfd = socket(AF_UNIX, SOCK_STREAM,0)) < 0) {
        perror("Client: impossivel abrir socket stream\n");  
        exit(EXIT_FAILURE);
    }

    bzero((char*) &serv_addr, sizeof(serv_addr));
    serv_addr.sun_family = AF_UNIX;
    strcpy(serv_addr.sun_path, address);
    servlen = strlen(serv_addr.sun_path) + sizeof(serv_addr.sun_family);

    if (connect(sockfd, (struct sockaddr *) &serv_addr, servlen) < 0) {
        perror("Client: Nao e possivel conectar ao server");
        exit(EXIT_FAILURE);
    }
    
    return receiveOutput();
}

int tfsUnmount() {
    if(sockfd < 0) {
        perror("tfsUnmount: Nao existe sessao ativa");
        exit(0);
    }

    char res[4];
    sprintf(res, "q\n");

    if(dprintf(sockfd, "%s", res) < 0) {
        perror("Client: erro ao enviar a mensagem com o output");
        exit(EXIT_FAILURE);
    }
    
    int out = receiveOutput();
    close(sockfd);
    return out;
}