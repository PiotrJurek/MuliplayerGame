#ifndef SERVER_SERVERGAMELOGIC_H
#define SERVER_SERVERGAMELOGIC_H

#include <stdlib.h>
#include "../globalConfig.h"
#include "serverPlayer.h"

extern int campsiteX, campsiteY;
extern int roundCounter;

extern wchar_t map[MAP_SIZE_Y][MAP_SIZE_X + 1];

void movePlayers();

void moveBeasts();

void killPlayer(player *p);

void playersCollisions();

void addObjectOnMap(unsigned char object);

void campsiteSpawn();

#endif //SERVER_SERVERGAMELOGIC_H
