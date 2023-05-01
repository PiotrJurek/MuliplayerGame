#ifndef SERVER_SERVERCOMMUNICATION_H
#define SERVER_SERVERCOMMUNICATION_H

#include <stdlib.h>
#include <semaphore.h>
#include "../globalConfig.h"
#include "serverPlayer.h"

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

extern connectionClient clientConnection[NUMBER_OF_PLAYERS];
extern sem_t *semConnectionStart, *semCanStartAnotherConnection;
extern connectionStarter *starter;
extern pthread_mutex_t maxPlayersMutex;

void *clientConnector(void *arg);

void closeConnectionWithClients();

void closeConnectionWithClient(int playerNumber);

void checkIfAlive();

void sendDataToClient(int playerNumber, int input);

void sendDataToClients(int input);

void getDataFromClient(int playerNumber);

void getDataFromClients();

#endif //SERVER_SERVERCOMMUNICATION_H
