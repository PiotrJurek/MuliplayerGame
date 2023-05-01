#ifndef SERVER_SERVERPLAYER_H
#define SERVER_SERVERPLAYER_H

#include "../globalConfig.h"

typedef struct {
    int isActive;
    int coinsCarried;
    int coinsBrought;
    int x, y;
    int pid;
    int deaths;
    int move;
    int stun;
} player;

extern player players[NUMBER_OF_PLAYERS];

player playerNew(int active);
void playerSpawn(player *p);
void initPlayers();

#endif //SERVER_SERVERPLAYER_H
