#include <ncurses.h>
#include <pthread.h>
#include <stdlib.h>
#include "../globalConfig.h"
#include "clientCommunication.h"
#include "clientInterface.h"


void* takeInput(void* arg);

int main() {
    int input = ' ';
    int connected = 0;
    initConfig();
    pthread_t connectThread;

    pthread_create(&connectThread, NULL, connectToServer, &connected);

    timeout(TIME_PRE_TICK / 1000);

    while (input != 'Q' && input != 'q') {
        if(!connected)
        {
            input = getch();
            continue;
        }
        if (getDataFromServer()) {
            pthread_cancel(connectThread);
            closeConnectionWithServer();
            endwin();
            return 0;
        }
        printEverything();
        flushinp();

        input = getch();
        myPlayer.move = input;
        printUpdatedPlayerLocation();

        sendDataToServer();
    }

    pthread_cancel(connectThread);
    if(connected)
    {
        sendDataToServer();
    }
    closeConnectionWithServer();
    endwin();
    return 0;
}

void* takeInput(void* arg)
{
    char input;
    do {
        input = getch();
    }
    while(input != 'q' && input != 'Q');
    closeConnectionWithServer();
    endwin();
    printf("Connection ended");
    exit(0);
}