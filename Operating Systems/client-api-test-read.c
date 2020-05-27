#include "../tecnicofs-api-constants.h"
#include "../tecnicofs-client-api.h"
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>


int main(int argc, char** argv) {
     if (argc != 2) {
        printf("Usage: %s sock_path\n", argv[0]);
        exit(0);
    }
    char readBuffer[10] = {0};
    printf("0 ");
    tfsMount(argv[1]);
    printf("0 ");
    tfsCreate("abc", RW, READ);
    int fd = -1;
    printf("0 ");
    fd = tfsOpen("abc", RW);
    printf("0 ");
    tfsWrite(fd, "12345", 5);
    
    printf("5 ");
    tfsRead(fd, readBuffer, 6);
    printf("Content read: %s\n", readBuffer);
    
    memset(readBuffer, 0, 10*sizeof(char));
    printf("3 ");
    tfsRead(fd, readBuffer, 4);
    printf("Content read: %s\n", readBuffer);
    
    memset(readBuffer, 0, 10*sizeof(char));
    printf("5 ");
    tfsRead(fd, readBuffer, 10);
    printf("Content read: %s\n", readBuffer);

    printf("0 ");
    tfsClose(fd);

    printf("%d ", TECNICOFS_ERROR_FILE_NOT_OPEN);
    tfsRead(fd, readBuffer, 6);

    printf("0 ");
    fd = tfsOpen("abc", WRITE);
    printf("%d ", TECNICOFS_ERROR_INVALID_MODE);
    tfsRead(fd, readBuffer, 6);

    printf("0 ");
    tfsClose(fd);

    printf("0 ");
    tfsDelete("abc");
    printf("0 ");
    tfsUnmount();

    return 0;
}
