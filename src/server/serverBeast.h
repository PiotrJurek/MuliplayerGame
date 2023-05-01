#ifndef SERVER_SERVERBEAST_H
#define SERVER_SERVERBEAST_H

#include "../globalConfig.h"
#include <pthread.h>

typedef struct {
    int x, y;
    int move;
    int stun;
    pthread_mutex_t mutex;
    pthread_t thread;
} beast;

extern beast beasts[MAX_NUMBER_OF_BEASTS];
extern int beastsAmount;

void addBeast();
void deleteBeasts();
void* aiBeast(void* arg);
void nextBeastsMoveAvailable();

#endif //SERVER_SERVERBEAST_H
