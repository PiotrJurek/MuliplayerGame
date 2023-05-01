#include "clientGameData.h"

int serverPid;
int campsiteX = -1, campsiteY = -1;
int roundCounter;

wchar_t map[PLAYER_VIEW_RANGE * 2 + 2][PLAYER_VIEW_RANGE * 2 + 2];
player myPlayer;
int myPlayerNumber;