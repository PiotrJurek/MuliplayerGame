#ifndef SERVER_SERVERDROPPEDTREASURE_H
#define SERVER_SERVERDROPPEDTREASURE_H

#include <stddef.h>
#include "serverPlayer.h"

typedef struct {
    int x, y;
    int amount;
    wchar_t itemUnder;
} droppedTreasure;

extern droppedTreasure* droppedTreasures;
extern int droppedTreasuresAmount;
extern int droppedTreasuresSize;

void addDroppedTreasure(int x, int y, int amount);
void deleteDroppedTreasure(int x, int y);
void collectDroppedTreasure(player* p);

#endif //SERVER_SERVERDROPPEDTREASURE_H
