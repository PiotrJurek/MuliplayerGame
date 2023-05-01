#ifndef CLIENT_CLIENTCOMMUNICATION_H
#define CLIENT_CLIENTCOMMUNICATION_H

#include <stddef.h>
#include <semaphore.h>
#include "../globalConfig.h"
#include "clientGameData.h"

typedef struct {
    wchar_t map[PLAYER_VIEW_RANGE * 2 + 2][PLAYER_VIEW_RANGE * 2 + 2];
    player playerData;
    int playerNumber;
    int roundCounter;
    int commandFromServer;
} dataForClient;

typedef struct {
    int clientPID;
    int serverPID;
} connectionStarter;

typedef struct {
    sem_t *sem;
    int fileDescriptor;
    dataForClient *data;
} connectionClient;

extern connectionClient clientConnection;
extern sem_t *semConnectionStart, *semCanStartAnotherConnection;

void* connectToServer(void*);

void closeConnectionWithServer();

int getDataFromServer();
void sendDataToServer();
#endif //CLIENT_CLIENTCOMMUNICATION_H