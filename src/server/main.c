#include <ncurses.h>
#include <pthread.h>
#include <unistd.h>
#include "../globalConfig.h"
#include "serverInterface.h"
#include "serverCommunication.h"
#include "serverDroppedTreasure.h"
#include "serverBeast.h"
#include "serverGameLogic.h"

pthread_mutex_t inputMutex;

void *takeInputFromServer(void *arg);

int main() {
    int input = ' ';
    pthread_t inputThread, connectionThread;
    long start, stop;
    initConfig();
    srand(time(NULL));
    campsiteSpawn();
    initPlayers();

    pthread_mutex_init(&inputMutex, NULL);
    pthread_create(&inputThread, NULL, takeInputFromServer, &input);
    pthread_create(&connectionThread, NULL, clientConnector, &input);


    while (input != 'Q' && input != 'q') {
        start = clock();

        pthread_mutex_lock(&inputMutex);
        checkIfAlive();
        getDataFromClients();
        movePlayers();
        moveBeasts();
        playersCollisions();
        roundCounter++;
        sendDataToClients(input);
        nextBeastsMoveAvailable();
        printEverything();
        pthread_mutex_unlock(&inputMutex);

        stop = clock();

        usleep(TIME_PRE_TICK - (stop - start) / CLOCKS_PER_SEC * 1000000);
    }

    sendDataToClients(input);
    sem_post(semConnectionStart);
    closeConnectionWithClients();
    pthread_join(inputThread, NULL);
    pthread_join(connectionThread, NULL);
    pthread_mutex_destroy(&inputMutex);
    endwin();
    deleteBeasts();
    free(droppedTreasures);
    return 0;
}

void *takeInputFromServer(void *arg) {

    int *input = arg;

    while (true) {
        pthread_mutex_lock(&inputMutex);
        *input = getch();
        switch (*input) {
            case 'b':
            case 'B':
                addBeast();
                break;
            case 'c':
            case 't':
            case 'T':
                addObjectOnMap(*input);
                printMap();
                move(MAP_SIZE_Y - 1, MAP_SIZE_X);
                refresh();
                break;
            case 'q':
            case 'Q':
                return NULL;
        }
        pthread_mutex_unlock(&inputMutex);
    }
}