#include "serverCommunication.h"
#include <fcntl.h>
#include <sys/mman.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <signal.h>
#include "serverBeast.h"
#include "serverGameLogic.h"

connectionClient clientConnection[NUMBER_OF_PLAYERS];
sem_t *semConnectionStart, *semCanStartAnotherConnection;
connectionStarter *starter;
pthread_mutex_t maxPlayersMutex;

void *clientConnector(void *arg) {
    int *input = arg;

    int fileConnection;

    pthread_mutex_init(&maxPlayersMutex, NULL);

    semConnectionStart = sem_open("semConnectStart", O_CREAT, 0777, 0);
    if (semConnectionStart == NULL) {
        printf("sem_open failed");
        exit(-1);
    }

    semCanStartAnotherConnection = sem_open("semCanStartAnotherConnection", O_CREAT, 0777, 0);
    if (semCanStartAnotherConnection == NULL) {
        printf("sem_open failed");
        exit(-1);
    }

    fileConnection = shm_open("shmConnect", O_CREAT | O_RDWR, 0777);
    if (ftruncate(fileConnection, sizeof(connectionStarter)) == -1) {
        printf("ftruncate failed");
        exit(-1);
    }

    starter = mmap(NULL, sizeof(connectionStarter), PROT_READ | PROT_WRITE, MAP_SHARED, fileConnection, 0);
    starter->serverPID = myPid;
    int i;
    while (*input != 'q' && *input != 'Q') {
        i = 0;
        while (players[i].isActive && i < NUMBER_OF_PLAYERS) {
            i++;
        }

        if(i < NUMBER_OF_PLAYERS)
        {
            pthread_mutex_unlock(&maxPlayersMutex);
        }

        pthread_mutex_lock(&maxPlayersMutex);

        sem_post(semCanStartAnotherConnection);

        if(*input == 'q' || *input == 'Q')
        {
            break;
        }

        sem_wait(semConnectionStart);

        if(*input == 'q' || *input == 'Q')
        {
            break;
        }

        i = 0;
        while (players[i].isActive && i < NUMBER_OF_PLAYERS) {
            i++;
        }

        players[i] = playerNew(1);
        players[i].pid = starter->clientPID;

        char number[10];
        char name[30];
        snprintf(number, 10, "%d", players[i].pid);
        strcpy(name, "semClient");
        strcat(name, number);
        clientConnection[i].sem = sem_open(name, O_CREAT, 0777, 0);
        if (clientConnection[i].sem == NULL) {
            printf("sem_open failed");
            exit(-1);
        }

        strcpy(name, "shmClient");
        strcat(name, number);
        clientConnection[i].fileDescriptor = shm_open(name, O_CREAT | O_RDWR, 0777);
        if (ftruncate(clientConnection[i].fileDescriptor, sizeof(dataForClient)) == -1) {
            printf("ftruncate failed");
            exit(-1);
        }

        clientConnection[i].data = mmap(NULL, sizeof(dataForClient), PROT_READ | PROT_WRITE, MAP_SHARED,
                                        clientConnection[i].fileDescriptor, 0);
        if (clientConnection[i].data == NULL) {
            printf("shm_open failed");
            exit(-1);
        }
    }

    sem_unlink("semConnectStart");
    sem_unlink("semCanStartAnotherConnection");
    sem_close(semConnectionStart);
    sem_close(semCanStartAnotherConnection);

    shm_unlink("shmConnect");
    munmap(starter, sizeof(connectionStarter));
}

void closeConnectionWithClients() {
    for (int i = 0; i < NUMBER_OF_PLAYERS; i++) {
        if (players[i].isActive) {
            closeConnectionWithClient(i);
        }
    }
}

void closeConnectionWithClient(int playerNumber) {
    char number[10];
    char name[30];
    snprintf(number, 10, "%d", clientConnection[playerNumber].data->playerData.pid);
    strcpy(name, "semClient");
    strcat(name, number);
    sem_close(clientConnection[playerNumber].sem);
    munmap(clientConnection[playerNumber].data, sizeof(dataForClient));

    players[playerNumber] = playerNew(0);
    pthread_mutex_unlock(&maxPlayersMutex);
}

void checkIfAlive()
{
    for (int i = 0; i < NUMBER_OF_PLAYERS; i++) {
        if (players[i].isActive && kill(players[i].pid, 0) == -1) {
            closeConnectionWithClient(i);
        }
    }
}

void sendDataToClient(int playerNumber, int input) {
    if (clientConnection[playerNumber].data == NULL || !players[playerNumber].isActive) {
        return;
    }

    clientConnection[playerNumber].data->playerNumber = playerNumber + 1;
    clientConnection[playerNumber].data->roundCounter = roundCounter;
    clientConnection[playerNumber].data->playerData = players[playerNumber];
    for (int j = -PLAYER_VIEW_RANGE; j <= PLAYER_VIEW_RANGE; j++) {
        for (int k = -PLAYER_VIEW_RANGE; k <= PLAYER_VIEW_RANGE; k++) {
            if (players[playerNumber].y + j >= 0 && players[playerNumber].y + j < MAP_SIZE_Y &&
                players[playerNumber].x + k >= 0 && players[playerNumber].x + k < MAP_SIZE_X) {
                clientConnection[playerNumber].data->map[j + PLAYER_VIEW_RANGE][k + PLAYER_VIEW_RANGE] =
                        map[players[playerNumber].y + j][players[playerNumber].x + k];
            }
        }
    }

    for (int i = 0; i < NUMBER_OF_PLAYERS; i++) {
        if (i != playerNumber && players[i].isActive &&
            players[playerNumber].x + PLAYER_VIEW_RANGE >= players[i].x &&
            players[playerNumber].x - PLAYER_VIEW_RANGE <= players[i].x &&
            players[playerNumber].y + PLAYER_VIEW_RANGE >= players[i].y &&
            players[playerNumber].y - PLAYER_VIEW_RANGE <= players[i].y) {
            clientConnection[playerNumber].data->map[players[i].y - players[playerNumber].y + PLAYER_VIEW_RANGE][
                    players[i].x - players[playerNumber].x + PLAYER_VIEW_RANGE] = i + '1';
        }
    }

    for (int i = 0; i < beastsAmount; i++) {
        if (players[playerNumber].x + PLAYER_VIEW_RANGE >= beasts[i].x &&
            players[playerNumber].x - PLAYER_VIEW_RANGE <= beasts[i].x &&
            players[playerNumber].y + PLAYER_VIEW_RANGE >= beasts[i].y &&
            players[playerNumber].y - PLAYER_VIEW_RANGE <= beasts[i].y) {
            clientConnection[playerNumber].data->map[beasts[i].y - players[playerNumber].y + PLAYER_VIEW_RANGE][
                    beasts[i].x - players[playerNumber].x + PLAYER_VIEW_RANGE] = '*';
        }
    }

    clientConnection[playerNumber].data->commandFromServer = input;
    sem_post(clientConnection[playerNumber].sem);
}

void sendDataToClients(int input) {
    for (int i = 0; i < NUMBER_OF_PLAYERS; i++) {
        sendDataToClient(i, input);
    }
}

void getDataFromClient(int playerNumber) {
    if (clientConnection[playerNumber].data == NULL || !players[playerNumber].isActive) {
        return;
    }
    players[playerNumber].move = clientConnection[playerNumber].data->playerData.move;
}

void getDataFromClients() {
    for (int i = 0; i < NUMBER_OF_PLAYERS; i++) {
        getDataFromClient(i);
    }
}
