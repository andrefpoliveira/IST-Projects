#include "tecnicofs-client-api.h"
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <assert.h>
#include "unix.h"

#define MAXLINE 512

void err_dump(char* msg) {
    perror(msg);
    exit(1);
}

int readline(int sockfd, char* recvline, int maxCap) {
    char c[2];
    int status = 0;
    int i = 0;

    while(1)
    {
        status = recv(sockfd,c, 1,0);

        if(status == 0)
        {
            //closed ?
            return -1;
        }
        else if(status > 0)
        {
            recvline[i] = *c;

            i++;

            if(*c == '\n')
            {
                break;
            }
        }
    }
    return i;

}

void str_cli(fp, sockfd)
FILE *fp;
int sockfd;
{
    int n;
    char sendline[MAXLINE],
    recvline[MAXLINE+1];

    while(fgets(sendline, MAXLINE, fp) != NULL) {
        n = strlen(sendline);
        if (write(sockfd,sendline,n) != n)
            err_dump("str_cli: write error on socket");


        recvline[n] = '\0';
    }

    if (ferror(fp))
        err_dump("str_cli: error reading file");
}

void main(char* argv[]) {
    int sockfd, servlen;
    struct sockaddr_un serv_addr;

    if ((sockfd = socket(AF_UNIX, SOCK_STREAM,0)) < 0)
        err_dump("Client: impossivel abrir socket stream\n");    

    bzero((char*) &serv_addr, sizeof(serv_addr));
    serv_addr.sun_family = AF_UNIX;
    printf("Aqui\n");
    strcpy(serv_addr.sun_path, &argv[1]);
    servlen = strlen(serv_addr.sun_path) + sizeof(serv_addr.sun_family);

    if (connect(sockfd, (struct sockaddr *) &serv_addr, servlen) < 0)
        err_dump("Client: Nao e possivel conectar ao server");

    str_cli(stdin, sockfd);

    close(sockfd);
    exit(0);
}
