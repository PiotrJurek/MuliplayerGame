#include <stdlib.h>
#include "serverPlayer.h"
#include "serverGameLogic.h"

player players[NUMBER_OF_PLAYERS];

player playerNew(int active) {
    player p = {
            .isActive = active,
            .coinsCarried = 0,
            .coinsBrought = 0,
            .deaths = 0,
            .move = ' ',
            .stun = 0
    };
    if (p.isActive) {
        playerSpawn(&p);
    }
    return p;
}

void playerSpawn(player *p) {
    int x, y;
    do {
        x = rand() % MAP_SIZE_X;
        y = rand() % MAP_SIZE_Y;
    } while (map[y][x] != EMPTY);
    p->x = x;
    p->y = y;
    p->stun = 0;
}

void initPlayers() {
    for (int i = 0; i < NUMBER_OF_PLAYERS; i++) {
        players[i] = playerNew(0);
    }
}