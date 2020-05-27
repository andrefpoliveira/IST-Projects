#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <semaphore.h>
#include "sema.h"

#define MAX_COMMANDS 10

//Verifica se a inicializacao dos semaforos ocorre sem erros
void initializeSemaphores() {
    if (sem_init(&prod, 0, MAX_COMMANDS)) {
        perror("O semáforo não pode ser criado\n");
        exit(EXIT_FAILURE);
    }

    if (sem_init(&cons, 0, 0)) {
        perror("O semáforo não pode ser criado\n");
        exit(EXIT_FAILURE);
    }
}

//Verifica se a destruicao dos semaforos ocorre sem erros
void destroySemaphores() {
    if (sem_destroy(&prod)) {
        perror("O semáforo não pode ser destruido\n");
        exit(EXIT_FAILURE);
    }

    if (sem_destroy(&cons)) {
        perror("O semáforo não pode ser destruido\n");
        exit(EXIT_FAILURE);
    }
}

//Verifica se a funcao wait do produtor ocorre sem erros
void waitProducer() {
    if (sem_wait(&prod)) {
        perror("Erro na operação de espera do produtor\n");
        exit(EXIT_FAILURE);
    }
}

//Verifica se a funcao wait dos consumidores ocorre sem erros
void waitConsumer() {
    if (sem_wait(&cons)) {
        perror("Erro na operação de espera do consumidor\n");
        exit(EXIT_FAILURE);
    }
}

//Verifica se a funcao post do produtor ocorre sem erros
void postProducer() {
    if (sem_post(&prod)) {
        perror("Erro ao tentar assinalar o produtor\n");
        exit(EXIT_FAILURE);
    }
}

//Verifica se a funcao post dos consumidores ocorre sem erros
void postConsumer() {
    if (sem_post(&cons)) {
        perror("Erro ao tentar assinalar o consumidor\n");
        exit(EXIT_FAILURE);
    }
}