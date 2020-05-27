#ifndef FS_H
#define FS_H
#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include "lib/bst.h"
#include "lib/hash.h"

#define MAX_COMMANDS 10
#define MAX_INPUT_SIZE 100
#define MAX_OPENED_FILES 5

//Macros com as funcoes relativas as threads
#ifdef MUTEX
    pthread_mutex_t* lock_command;
    #define TYPE pthread_mutex_t
    #define TRY_LOCK(lockType) pthread_mutex_trylock(lockType)
    #define INIT_LOCK(lockType) pthread_mutex_init(lockType, NULL)
    #define DELETE_LOCK(lockType) pthread_mutex_destroy(lockType)
    #define WRITE_LOCK(lockType) pthread_mutex_lock(lockType)
    #define READ_LOCK(lockType) WRITE_LOCK(lockType)
    #define UNLOCK(lockType) pthread_mutex_unlock(lockType)

#elif RWLOCK
    
    pthread_mutex_t* lock_command;
    #define TYPE pthread_rwlock_t
    #define TRY_LOCK(lockType) pthread_rwlock_trywrlock(lockType)
    #define INIT_LOCK(lockType) pthread_rwlock_init(lockType, NULL)
    #define DELETE_LOCK(lockType) pthread_rwlock_destroy(lockType)
    #define WRITE_LOCK(lockType) pthread_rwlock_wrlock(lockType)
    #define READ_LOCK(lockType) pthread_rwlock_rdlock(lockType)
    #define UNLOCK(lockType) pthread_rwlock_unlock(lockType)

#else
    pthread_mutex_t* lock_command;
    #define TYPE void*
    #define TRY_LOCK(lockType) NULL
    #define INIT_LOCK(lockType) NULL
    #define DELETE_LOCK(lockType) NULL
    #define WRITE_LOCK(lockType) NULL
    #define READ_LOCK(lockType) NULL
    #define UNLOCK(lockType) NULL

#endif

typedef struct tecnicofs {
    node** bstRoot;
    int nextINumber;
} tecnicofs;

typedef struct openedFile {
    char* fileName;
    int iNumber;
    int mode;
} openedFile;

int tryLock(int bucket);
void command_r(tecnicofs* fs, char name[MAX_INPUT_SIZE], char renameName[MAX_INPUT_SIZE], int numberBuckets);
void deleteLock(int numBuckets);
void initLock(int numBuckets);
void unlock(char lockType, int bucket);
void writeLock(char lockType, int bucket);
void readLock(char lockType, int bucket);
int obtainNewInumber(tecnicofs* fs);
tecnicofs* new_tecnicofs();
void free_tecnicofs(tecnicofs* fs, int numBuckets);
void create(tecnicofs* fs, char *name, int inumber, int numberBuckets);
void delete(tecnicofs* fs, char *name, int numberBuckets);
int lookup(tecnicofs* fs, char *name, int numberBuckets);
void print_tecnicofs_tree(FILE * fp, tecnicofs *fs, int numBuckets);
void returnOutput(int sockfd, int result);
void returnResultOfRead(int sockfd, int output, char* result);

#endif /* FS_H */
