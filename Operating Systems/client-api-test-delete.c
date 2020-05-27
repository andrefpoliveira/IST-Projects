#include "../tecnicofs-api-constants.h"
#include "../tecnicofs-client-api.h"
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>


int main(int argc, char** argv) {
     if (argc != 2) {
        printf("Usage: %s sock_path\n", argv[0]);
        exit(0);
    }
    int fd = -1;
    printf("0 ");
    tfsMount(argv[1]);
    printf("0 ");
    tfsCreate("a", RW, READ);

    printf("0 ");
    tfsDelete("a");
    
    printf("%d ", TECNICOFS_ERROR_FILE_NOT_FOUND);
    tfsDelete("b");
    printf("0 ");
    tfsUnmount();
    return 0;
}
