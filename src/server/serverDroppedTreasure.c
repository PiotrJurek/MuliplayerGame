#include "serverDroppedTreasure.h"
#include "serverGameLogic.h"
#include <malloc.h>
#include <stdlib.h>

droppedTreasure* droppedTreasures = NULL;
int droppedTreasuresAmount = 0;
int droppedTreasuresSize = 0;

void addDroppedTreasure(int x, int y, int amount)
{
    if(amount <= 0)
    {
        return;
    }

    droppedTreasure* temp;

    droppedTreasuresAmount++;
    if(droppedTreasuresAmount > droppedTreasuresSize)
    {
        if((temp = realloc(droppedTreasures, droppedTreasuresAmount * sizeof(droppedTreasure))) == NULL)
        {
            exit(-1);
        }
        droppedTreasures = temp;
    }

    (droppedTreasures + droppedTreasuresAmount - 1)->amount = amount;
    (droppedTreasures + droppedTreasuresAmount - 1)->x = x;
    (droppedTreasures + droppedTreasuresAmount - 1)->y = y;
    (droppedTreasures + droppedTreasuresAmount - 1)->itemUnder = map[y][x];

    map[y][x] = DROPPED_TREASURE;
}

void deleteDroppedTreasure(int x, int y)
{
    int i = 0;
    for(; i < droppedTreasuresAmount; i++)
    {
        if((droppedTreasures + i)->x == x && (droppedTreasures + i)->y == y)
        {
            break;
        }
    }

    map[y][x] = (droppedTreasures + i)->itemUnder;

    for(; i < droppedTreasuresAmount - 1; i++)
    {
        *(droppedTreasures + i) = *(droppedTreasures + i + 1);
    }

    droppedTreasuresAmount--;
}

void collectDroppedTreasure(player* p)
{
    for(int i = 0; i < droppedTreasuresAmount; i++)
    {
        if((droppedTreasures + i)->x == p->x && (droppedTreasures + i)->y == p->y)
        {
            p->coinsCarried += (droppedTreasures + i)->amount;
            deleteDroppedTreasure(p->x, p->y);
            break;
        }
    }
}