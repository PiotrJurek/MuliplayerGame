#ifndef CLIENT_CLIENTGAMEDATA_H
#define CLIENT_CLIENTGAMEDATA_H

#include <stddef.h>
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

extern int serverPid;
extern int campsiteX, campsiteY;
extern int roundCounter;

extern wchar_t map[PLAYER_VIEW_RANGE * 2 + 2][PLAYER_VIEW_RANGE * 2 + 2];
extern player myPlayer;
extern int myPlayerNumber;

#endif //CLIENT_CLIENTGAMEDATA_H
