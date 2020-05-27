#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <semaphore.h>
#include <unistd.h>
#include <errno.h>
#include "fs.h"
#include "lib/inodes.h"

TYPE* lock_vector;

void returnOutput(int sockfd, int result) {
    char* output = malloc(sizeof(result));
    sprintf(output,"%d", result);

    if(dprintf(sockfd, "%s%c", output, '\0') < 0) {
        perror("Server: erro ao enviar a mensagem com o output");
        exit(EXIT_FAILURE);
    }
    free(output);
}

void returnResultOfRead(int sockfd, int output, char* result) {
    if(dprintf(sockfd, "%d %s", output, result) < 0) {
        perror("Server: erro ao enviar a mensagem com o output");
        exit(EXIT_FAILURE);
    }

    //printf("Return result of read: %d %s\n", output, result);
    free(result);
}

int randomSleep() {
    return (rand() % (3 + 1)); 
}

void command_r(tecnicofs* fs, char name[MAX_INPUT_SIZE], char renameName[MAX_INPUT_SIZE], int numberBuckets) {
    int bucketName = hash(name, numberBuckets);
    int bucketRenameName = hash(renameName, numberBuckets);
    unlock('c',0);

    writeLock('t', bucketName);
    int iNumberName = lookup(fs, name, bucketName);
    if (bucketName == bucketRenameName) {
        delete(fs, name, bucketName);
        create(fs, renameName, iNumberName, bucketName);
        unlock('t', bucketName);
        return;
        
    } else {

        while (1) {
            int tryResult = tryLock(bucketRenameName);

            if (tryResult == 0) {
                //Adquiriu o lock
                int iNumberRenameName = lookup(fs, renameName, bucketRenameName);

                if (iNumberName != -1 && iNumberRenameName == -1) {
                    delete(fs, name, bucketName);
                    unlock('t', bucketName);

                    create(fs, renameName, iNumberName, bucketRenameName);
                    unlock('t', bucketRenameName);
                }
                      
                return ;

            } else if (tryResult == EBUSY) {
                //Nao foi possivel adquirir o lock
                unlock('t', bucketName);
                sleep(randomSleep());

            } else {
                unlock('t', bucketName);
                perror("Erro ao adquirir o lock\n");
                exit(EXIT_FAILURE);
            }
        }
    }
}

//Verifica se o trylock ocorre sem erros
int tryLock(int bucket) {
    if (TRY_LOCK(&lock_vector[bucket]) == 0) {
        return 0;
    } else if (TRY_LOCK(&lock_vector[bucket]) < 0) {
        perror("Erro ao adquirir o lock\n");
        exit(EXIT_FAILURE);
    } else {
        return EBUSY;
    }
}

//Verifica se o delete dos locks dá erro
void deleteLock(int numBuckets) {
    if (pthread_mutex_destroy(lock_command)) {
        perror("Error: delete lock of the command failed");
        exit(EXIT_FAILURE);
    }

    for (int i = 0; i < numBuckets; i++) {
        if (DELETE_LOCK(&lock_vector[i])) {
            perror("Error: delete lock of the command failed");
            exit(EXIT_FAILURE);
        }
    }
}

//Verifica se o init dos locks ocorre sem erros
void initLock(int numBuckets) {
    if (pthread_mutex_init(lock_command, NULL)) {
        perror("Error: init lock of the command failed");
        exit(EXIT_FAILURE);
    }

    for (int i = 0; i < numBuckets; i++) {
        if (INIT_LOCK(&(lock_vector[i]))) {
            perror("Error: init lock of the command failed");
            exit(EXIT_FAILURE);
        }
    }
}

//Verifica se o macro UNLOCK executa sem erros
//Input:
//Se lockType == 't' bloqueia a arvore
//Se lockType == 'c' bloqueia o vetor de comandos
void unlock(char lockType, int bucket) {
    if (lockType == 't') {
        if (UNLOCK(&lock_vector[bucket])) {
            perror("Error: unlock lock of the tree failed");
            exit(EXIT_FAILURE);
        }
    } else {
        if (pthread_mutex_unlock(lock_command)) {
            perror("Error: unlock lock of the command failed");
            exit(EXIT_FAILURE);
        }
    }
}

//Verifica se o macro WRITE_LOCK executa sem erros
//Input:
//Se lockType == 't' bloqueia a arvore
//Se lockType == 'c' bloqueia o vetor de comandos
void writeLock(char lockType, int bucket) {
    if (lockType == 't') {
        if (WRITE_LOCK(&lock_vector[bucket])) {
            perror("Error: write lock of the tree failed");
            exit(EXIT_FAILURE);
        }
    } else {
        if (pthread_mutex_lock(lock_command)) {
            perror("Error: write lock of the command failed");
            exit(EXIT_FAILURE);
        }
    } 
}

//Verifica se o macro READ_LOCK executa sem erros
//Input:
//Se lockType == 't' bloqueia a arvore
//Se lockType == 'c' bloqueia o vetor de comandos
void readLock(char lockType, int bucket) {
    if (lockType == 't') {
        if (READ_LOCK(&lock_vector[bucket])) {
            perror("Error: delete lock of the tree failed");
            exit(EXIT_FAILURE);
        }
    } else {
        if (pthread_mutex_lock(lock_command)) {
            perror("Error: delete lock of the command failed");
            exit(EXIT_FAILURE);
        }
    }
}

int obtainNewInumber(tecnicofs* fs) {
	int newInumber = ++(fs->nextINumber);
	return newInumber;
}

tecnicofs* new_tecnicofs(int numBuckets){
	tecnicofs*fs = malloc(sizeof(tecnicofs));
	if (!fs) {
		perror("failed to allocate tecnicofs");
		exit(EXIT_FAILURE);
	}

	if (!(fs->bstRoot = malloc (numBuckets * sizeof(node*)))) {
        perror("Erro ao dar malloc da hash de árvores\n");
        exit(EXIT_FAILURE);
    }

    if (!(lock_vector = malloc(sizeof(TYPE) * numBuckets))) {
        perror("Erro ao dar malloc do vetor de trincos\n");
        exit(EXIT_FAILURE);
    }

    if (!(lock_command = malloc(sizeof(pthread_mutex_t)))) {
        perror("Erro ao dar malloc do trincos dos comandos\n");
        exit(EXIT_FAILURE);
    }
    
	fs->nextINumber = 0;

    for (int i = 0; i < numBuckets; i++) {
        fs->bstRoot[i] = NULL;
    }

    initLock(numBuckets);
	return fs;
}

void free_tecnicofs(tecnicofs* fs, int numberBuckets){
    for (int i = 0; i < numberBuckets; i++) {
        if(fs->bstRoot[i]!=NULL) {
            free_tree(fs->bstRoot[i]);
        }
    }
    deleteLock(numberBuckets);
    free(lock_vector);
    free(lock_command);
    free(fs->bstRoot);
	free(fs);
}

void create(tecnicofs* fs, char *name, int inumber, int bucket){
	fs->bstRoot[bucket] = insert(fs->bstRoot[bucket], name, inumber);
}

void delete(tecnicofs* fs, char *name, int bucket){
	fs->bstRoot[bucket] = remove_item(fs->bstRoot[bucket], name);
}

int lookup(tecnicofs* fs, char *name, int bucket){
	node* searchNode = search(fs->bstRoot[bucket], name);
	if ( searchNode ) {
        return searchNode->inumber;
    }
	return -1;
}

void print_tecnicofs_tree(FILE * fp, tecnicofs *fs, int numBuckets){
    for (int i = 0; i < numBuckets; i++) {
        print_tree(fp, fs->bstRoot[i]);
    }
}