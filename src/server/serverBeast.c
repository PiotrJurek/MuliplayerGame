#include "serverBeast.h"
#include <stdlib.h>
#include <ncurses.h>
#include "serverGameLogic.h"
#include "serverPlayer.h"

beast beasts[MAX_NUMBER_OF_BEASTS];
int beastsAmount = 0;

void beastSpawn(beast *b) {
    int x, y;
    do {
        x = rand() % MAP_SIZE_X;
        y = rand() % MAP_SIZE_Y;
    } while (map[y][x] != L' ');
    b->x = x;
    b->y = y;
    b->stun = 0;
}

void addBeast() {
    if (beastsAmount >= MAX_NUMBER_OF_BEASTS) {
        return;
    }

    beastSpawn(&beasts[beastsAmount]);

    pthread_mutex_init(&((beasts + beastsAmount)->mutex), NULL);
    pthread_create(&((beasts + beastsAmount)->thread), NULL, aiBeast, &beasts[beastsAmount]);

    beastsAmount++;
}

void deleteBeasts() {
    for (int i = 0; i < beastsAmount; i++) {
        pthread_mutex_destroy(&((beasts + i)->mutex));
        pthread_cancel((beasts + i)->thread);
    }
}

void nextBeastsMoveAvailable() {
    for (int i = 0; i < beastsAmount; i++) {
        pthread_mutex_unlock(&((beasts + i)->mutex));
    }
}

int isMovePossible(beast *b) {
    return !(map[b->y - 1][b->x] == WALL && map[b->y + 1][b->x] == WALL &&
             map[b->y][b->x - 1] == WALL && map[b->y][b->x + 1] == WALL);
}

int beastTryMove(beast *b, int move) {
    switch (move) {
        case KEY_DOWN:
            if (map[b->y + 1][b->x] == WALL) {
                return 0;
            }
            break;
        case KEY_UP:
            if (map[b->y - 1][b->x] == WALL) {
                return 0;
            }
            break;
        case KEY_LEFT:
            if (map[b->y][b->x - 1] == WALL) {
                return 0;
            }
            break;
        case KEY_RIGHT:
            if (map[b->y][b->x + 1] == WALL) {
                return 0;
            }
            break;
        default:
            return 0;
    }
    b->move = move;
    return 1;
}

int isReachable(int x1, int y1, int x2, int y2, int possibleMoveX, int possibleMoveY) {
    if (x1 == x2 && y1 == y2) {
        return 1;
    }

    if (possibleMoveY > 0 && map[y1 + 1][x1] != WALL && isReachable(x1, y1 + 1, x2, y2, possibleMoveX, possibleMoveY)) {
        return KEY_DOWN;
    }
    if (possibleMoveY < 0 && map[y1 - 1][x1] != WALL && isReachable(x1, y1 - 1, x2, y2, possibleMoveX, possibleMoveY)) {
        return KEY_UP;
    }
    if (possibleMoveX < 0 && map[y1][x1 - 1] != WALL && isReachable(x1 - 1, y1, x2, y2, possibleMoveX, possibleMoveY)) {
        return KEY_LEFT;
    }
    if (possibleMoveX > 0 && map[y1][x1 + 1] != WALL && isReachable(x1 + 1, y1, x2, y2, possibleMoveX, possibleMoveY)) {
        return KEY_RIGHT;
    }

    return 0;
}

void beastDefaultMove(beast *b) {
    int move;
    while (1) {
        move = rand() % 4 + KEY_DOWN;
        if (beastTryMove(b, move)) {
            return;
        }
    }
}

int beastTryMoveToPlayer(beast *b) {
    int moveX, moveY;
    int move;
    for (int i = 0; i < NUMBER_OF_PLAYERS; i++) {
        if (!players[i].isActive) {
            continue;
        }

        moveX = players[i].x - b->x;
        moveY = players[i].y - b->y;

        if (abs(moveX) > PLAYER_VIEW_RANGE || abs(moveY) > PLAYER_VIEW_RANGE) {
            continue;
        }

        move = isReachable(b->x, b->y, players[i].x, players[i].y, moveX, moveY);

        if (move) {
            b->move = move;
            return 1;
        }
    }
    return 0;
}

void *aiBeast(void *arg) {
    beast *b = arg;

    while (1) {
        pthread_mutex_lock(&b->mutex);
        if (!isMovePossible(b)) {
            continue;
        }
        if (!beastTryMoveToPlayer(b)) {
            beastDefaultMove(b);
        }
    }
}