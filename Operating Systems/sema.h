#ifndef SEMA_H
#define SEMA_H
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <semaphore.h>

sem_t prod;
sem_t cons;

void initializeSemaphores();
void destroySemaphores();
void waitProducer();
void waitConsumer();
void postProducer();
void postConsumer();

#endif /* SEMA_H */