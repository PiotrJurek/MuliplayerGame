#include "clientCommunication.h"
#include <fcntl.h>
#include <sys/mman.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <curses.h>

connectionClient clientConnection;
sem_t *semConnectionStart, *semCanStartAnotherConnection;

void* connectToServer(void* arg) {

    connectionStarter *starter;

    int fileConnection;

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

    mvprintw(0, 0, "Connecting to server...");

    sem_wait(semCanStartAnotherConnection);

    serverPid = starter->serverPID;
    starter->clientPID = myPid;

    sem_post(semConnectionStart);

    char number[10];
    char name[30];
    snprintf(number, 10, "%d", myPid);
    strcpy(name, "semClient");
    strcat(name, number);
    clientConnection.sem = sem_open(name, O_CREAT, 0777, 0);
    if (clientConnection.sem == NULL) {
        printf("sem_open failed");
        exit(-1);
    }
    strcpy(name, "shmClient");
    strcat(name, number);
    clientConnection.fileDescriptor = shm_open(name, O_CREAT | O_RDWR, 0777);
    if (ftruncate(clientConnection.fileDescriptor, sizeof(dataForClient)) == -1) {
        printf("ftruncate failed");
        exit(-1);
    }

    clientConnection.data = mmap(NULL, sizeof(dataForClient), PROT_READ | PROT_WRITE, MAP_SHARED,
                                 clientConnection.fileDescriptor, 0);

    if (clientConnection.data == NULL) {
        printf("shm_open failed");
        exit(-1);
    }

    sem_close(semConnectionStart);
    sem_close(semCanStartAnotherConnection);

    munmap(starter, sizeof(connectionStarter));
    *(int*)arg = 1;
}

void closeConnectionWithServer() {
    char number[10];
    char name[30];
    snprintf(number, 10, "%d", myPid);
    strcpy(name, "semClient");
    strcat(name, number);
    sem_close(clientConnection.sem);
    sem_unlink(name);
    strcpy(name, "shmClient");
    strcat(name, number);
    shm_unlink(name);
    munmap(clientConnection.data, sizeof(dataForClient));
}

int getDataFromServer() {
    sem_wait(clientConnection.sem);
    myPlayer = clientConnection.data->playerData;
    myPlayerNumber = clientConnection.data->playerNumber;
    roundCounter = clientConnection.data->roundCounter;
    for (int j = 0; j < PLAYER_VIEW_RANGE * 2 + 1; j++) {
        for (int k = 0; k < PLAYER_VIEW_RANGE * 2 + 1; k++) {
            map[j][k] = clientConnection.data->map[j][k];
        }
    }

    int command = clientConnection.data->commandFromServer;

    if (command == 'q' || command == 'Q') {
        return 1;
    }
    return 0;
}

void sendDataToServer() {
    clientConnection.data->playerData.move = myPlayer.move;
}
