#include <string.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include "inodes.h"
#include "../tecnicofs-api-constants.h"

inode_t inode_table[INODE_TABLE_SIZE];
pthread_mutex_t* inode_table_lock;

void lock_inode_table(int index){
    if(pthread_mutex_lock(&inode_table_lock[index]) != 0){
        perror("Failed to acquire the i-node table lock.\n");
        exit(EXIT_FAILURE);
    }
}

void unlock_inode_table(int index){
    if(pthread_mutex_unlock(&inode_table_lock[index]) != 0){
        perror("Failed to release the i-node table lock.\n");
        exit(EXIT_FAILURE);
    }
}

/*
 * Initializes the i-nodes table and the mutex.
 */
void inode_table_init(){    
    inode_table_lock = malloc(INODE_TABLE_SIZE * sizeof(pthread_mutex_t));
    for(int i = 0; i < INODE_TABLE_SIZE; i++){
        if(pthread_mutex_init(&inode_table_lock[i], NULL) != 0){
            perror("Failed to initialize inode table mutex.\n");
            exit(EXIT_FAILURE);
        }

        inode_table[i].owner = FREE_INODE;
        inode_table[i].fileContent = NULL;
    }
}

/*
 * Releases the allocated memory for the i-nodes tables
 * and destroys the mutex.
 */

void inode_table_destroy(){
    for(int i = 0; i < INODE_TABLE_SIZE; i++){
        if(inode_table[i].owner!=FREE_INODE && inode_table[i].fileContent) {
            free(inode_table[i].fileContent);
        }

        if(pthread_mutex_destroy(&inode_table_lock[i]) != 0){
            perror("Failed to destroy inode table mutex.\n");
            exit(EXIT_FAILURE);
        }
    }
    free(inode_table_lock);
    
    
}

/*
 * Creates a new i-node in the table with the given information.
 * Input:
 *  - owner: uid of the user that created the file
 *  - ownerPerm: permissions of the owner
 *  - othersPerm: permissions of all other users
 * Returns:
 *  inumber: identifier of the new i-node, if successfully created
 *       -1: if an error occurs
 */
int inode_create(uid_t owner, permission ownerPerm, permission othersPerm, int bucket){
    lock_inode_table(bucket);
    for(int inumber = 0; inumber < INODE_TABLE_SIZE; inumber++){
        if(inode_table[inumber].owner == FREE_INODE){
            inode_table[inumber].owner = owner;
            inode_table[inumber].ownerPermissions = ownerPerm;
            inode_table[inumber].othersPermissions = othersPerm;
            inode_table[inumber].fileContent = NULL;
            inode_table[inumber].openedFiles = 0;
            unlock_inode_table(bucket);
            return inumber;
        }
    }
    unlock_inode_table(bucket);
    return -1;
}

/*
 * Deletes the i-node.
 * Input:
 *  - inumber: identifier of the i-node
 * Returns:
 *   0: if successful
 *  -1: if an error occurs
 */
int inode_delete(int inumber, int bucket){
    lock_inode_table(bucket);
    if((inumber < 0) || (inumber > INODE_TABLE_SIZE) || (inode_table[inumber].owner == FREE_INODE)){
        printf("inode_delete: invalid inumber");
        unlock_inode_table(bucket);
        return -1;
    }

    inode_table[inumber].owner = FREE_INODE;
    if(inode_table[inumber].fileContent){
        free(inode_table[inumber].fileContent);
    }
    unlock_inode_table(bucket);
    return 0;
}

/*
 * Copies the contents of the i-node into the arguments.
 * Only the fields referenced by non-null arguments are copied.
 * Input:
 *  - inumber: identifier of the i-node
 *  - owner: pointer to uid_t
 *  - ownerPerm: pointer to permission
 *  - othersPerm: pointer to permission
 *  - fileContent: pointer to a char array with size >= len
 * Returns:
 *    len of content read:if successful
 *   -1: if an error occurs
 */
int inode_get(int inumber,uid_t *owner, permission *ownerPerm, permission *othersPerm,
                     char* fileContents, int len, int bucket){
    lock_inode_table(bucket);
    if((inumber < 0) || (inumber > INODE_TABLE_SIZE) || (inode_table[inumber].owner == FREE_INODE)){
        printf("inode_getValues: invalid inumber %d\n", inumber);
        unlock_inode_table(bucket);
        return -1;
    }

    if(len < 0){
        printf("inode_getValues: invalid len %d\n", len);
        unlock_inode_table(bucket);
        return -1;
    }

    if(owner)
        *owner = inode_table[inumber].owner;

    if(ownerPerm)
        *ownerPerm = inode_table[inumber].ownerPermissions;

    if(othersPerm)
        *othersPerm = inode_table[inumber].othersPermissions;

    if(fileContents && len > 0 && inode_table[inumber].fileContent){
        strncpy(fileContents, inode_table[inumber].fileContent, len);
        fileContents[len] = '\0';
        unlock_inode_table(bucket);
        return strlen(fileContents);
    }

    unlock_inode_table(bucket);
    return inode_table[inumber].openedFiles;
}


/*
 * Updates the i-node file content.
 * Input:
 *  - inumber: identifier of the i-node
 *  - fileContent: pointer to the string with size >= len
 *  - len: length to copy
 * Returns:
 *    0:if successful
 *   -1: if an error occurs
 */
int inode_set(int inumber, char *fileContents, int len, int bucket){
    lock_inode_table(bucket);
    if((inumber < 0) || (inumber > INODE_TABLE_SIZE) || (inode_table[inumber].owner == FREE_INODE)){
        printf("inode_setFileContent: invalid inumber");
        unlock_inode_table(bucket);
        return -1;
    }

    if(!fileContents || len < 0 || strlen(fileContents) < len){
        printf("inode_setFileContent: \
               fileContents must be non-null && len > 0 && strlen(fileContents) > len");
        unlock_inode_table(bucket);
        return -1;
    }
    
    if(inode_table[inumber].fileContent)
        free(inode_table[inumber].fileContent);

    inode_table[inumber].fileContent = malloc(sizeof(char) * (len+1));
    strncpy(inode_table[inumber].fileContent, fileContents, len);
    inode_table[inumber].fileContent[len] = '\0';

    unlock_inode_table(bucket);
    return 0;
}

int inode_decrease_openedFiles(int inumber, int bucket){
    lock_inode_table(bucket);
    if((inumber < 0) || (inumber > INODE_TABLE_SIZE) || (inode_table[inumber].owner == FREE_INODE)){
        printf("inode_decrease: invalid inumber");
        unlock_inode_table(bucket);
        return -1;
    }

    inode_table[inumber].openedFiles--;

    unlock_inode_table(bucket);
    return 0;
}

int inode_increase_openedFiles(int inumber, int bucket){
    lock_inode_table(bucket);
    if((inumber < 0) || (inumber > INODE_TABLE_SIZE) || (inode_table[inumber].owner == FREE_INODE)){
        printf("inode_increase: invalid inumber");
        unlock_inode_table(bucket);
        return -1;
    }

    inode_table[inumber].openedFiles++;

    unlock_inode_table(bucket);
    return 0;
}