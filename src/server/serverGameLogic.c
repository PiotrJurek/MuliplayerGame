#include "serverGameLogic.h"
#include <ncurses.h>
#include <stddef.h>
#include "serverBeast.h"
#include "serverDroppedTreasure.h"
#include "serverCommunication.h"

wchar_t map[MAP_SIZE_Y][MAP_SIZE_X + 1] = {L"███████████████████████████████████████████████████",
                                           L"█c  █       █   #####       █         █ccccccc█   █",
                                           L"█ █ ███ ███ ███████████ ███ █ ███████ ███ █████ t █",
                                           L"█ █   █ █ █           █ █ █   █     █     █   █   █",
                                           L"█ ███ █ █ ███###█████ █ █ █████ █████████████ ███ █",
                                           L"█ █T█   █           █ █ █ ##  █      c█       █ █ █",
                                           L"█ █ █████ ███ ███████ █ █ █ ███ ███ ███ ███ █ █ █ █",
                                           L"█ █         █ █       █ █ █     █   █   █ █ █   █ █",
                                           L"█ █ ███████ ███ ███████ █████ ███ ███ ███ █ ███ █ █",
                                           L"█ █T█     █   █   █     █   █   █         █ █ █ █ █",
                                           L"█ ███ ███ ███ ███ ███ ███ █ ███ ███████████ █ █ █ █",
                                           L"█ █   █       █ █  c█     █ █   █ █       █ █   █ █",
                                           L"█ █ ██████#██ █ ███ ███ ███ ███ █ █ █████ █ █ ███ █",
                                           L"█ █    #█   █ █   █   █   █   █   █ █     █ █ █   █",
                                           L"█ █ █ ##█ ███ ███ ███ ███████ ███ █ ███ ███ █ █ ███",
                                           L"█ █ █## █    #  █   █ █  ###  █   █   █     █ █ █ █",
                                           L"█ █ █#  ███████ █ █ █ █ ██#████ █████ ███████ █ █ █",
                                           L"█ █ █#      █   █ █ █   █     █   █ █       ##█   █",
                                           L"█ █████████ █ ███ ███████ ███████ █ █████ █ ##███ █",
                                           L"█ █#      █ █     █     █       █   █   █ █ ##  █ █",
                                           L"█ █ █████ █ ███████ █ ███ █████ ███ █ █ ███#█████ █",
                                           L"█###█     █         █  ###█## █     █ █   █###### █",
                                           L"█c███ █████████████████████#█ ███████ ███ █#    # █",
                                           L"█ccc█                 ######█##         █    ##   █",
                                           L"███████████████████████████████████████████████████"};

int campsiteX, campsiteY;
int roundCounter = 0;

void movePlayers() {
    for (int i = 0; i < NUMBER_OF_PLAYERS; i++) {
        if (!players[i].isActive) {
            continue;
        }

        if (players[i].stun) {
            players[i].stun--;
            continue;
        }

        switch (players[i].move) {
            case KEY_UP:
                if (map[players[i].y - 1][players[i].x] != WALL) {
                    players[i].y--;
                }
                break;
            case KEY_DOWN:
                if (map[players[i].y + 1][players[i].x] != WALL) {
                    players[i].y++;
                }
                break;
            case KEY_LEFT:
                if (map[players[i].y][players[i].x - 1] != WALL) {
                    players[i].x--;
                }
                break;
            case KEY_RIGHT:
                if (map[players[i].y][players[i].x + 1] != WALL) {
                    players[i].x++;
                }
                break;
            case 'q':
            case 'Q':
                closeConnectionWithClient(i);
                break;
        }

        if (map[players[i].y][players[i].x] == BUSH) {
            players[i].stun += 1;
        }

        players[i].move = ' ';
    }
}

void moveBeasts() {
    for (int i = 0; i < beastsAmount; i++) {

        if (beasts[i].stun) {
            beasts[i].stun--;
            continue;
        }

        switch (beasts[i].move) {
            case KEY_UP:
                if (map[beasts[i].y - 1][beasts[i].x] != WALL) {
                    beasts[i].y--;
                }
                break;
            case KEY_DOWN:
                if (map[beasts[i].y + 1][beasts[i].x] != WALL) {
                    beasts[i].y++;
                }
                break;
            case KEY_LEFT:
                if (map[beasts[i].y][beasts[i].x - 1] != WALL) {
                    beasts[i].x--;
                }
                break;
            case KEY_RIGHT:
                if (map[beasts[i].y][beasts[i].x + 1] != WALL) {
                    beasts[i].x++;
                }
                break;
            case 'q':
            case 'Q':
                closeConnectionWithClient(i);
                break;
        }

        if (map[beasts[i].y][beasts[i].x] == BUSH) {
            beasts[i].stun += 1;
        }

        beasts[i].move = ' ';
    }
}

void killPlayer(player *p) {
    p->coinsCarried = 0;
    p->deaths += 1;
    playerSpawn(p);
}

void playersCollisions() {
    for (int i = 0; i < NUMBER_OF_PLAYERS; i++) {
        if (!players[i].isActive) {
            continue;
        }

        wchar_t object = map[players[i].y][players[i].x];

        switch (object) {
            case COIN:
                players[i].coinsCarried += 1;
                map[players[i].y][players[i].x] = EMPTY;
                break;
            case TREASURE:
                players[i].coinsCarried += 10;
                map[players[i].y][players[i].x] = EMPTY;
                break;
            case LARGE_TREASURE:
                players[i].coinsCarried += 50;
                map[players[i].y][players[i].x] = EMPTY;
                break;
            case DROPPED_TREASURE:
                collectDroppedTreasure(&players[i]);
                break;
            case CAMPSITE:
                players[i].coinsBrought += players[i].coinsCarried;
                players[i].coinsCarried = 0;
                break;
            default:
                break;
        }

        for (int j = 0; j < NUMBER_OF_PLAYERS; j++) {
            if (!players[j].isActive || i == j) {
                continue;
            }

            if (players[i].y == players[j].y && players[i].x == players[j].x) {
                addDroppedTreasure(players[i].x, players[i].y, players[i].coinsCarried + players[j].coinsCarried);
                killPlayer(&players[i]);
                killPlayer(&players[j]);
            }
        }

        for (int j = 0; j < beastsAmount; j++) {
            if (players[i].y == beasts[j].y && players[i].x == beasts[j].x) {
                addDroppedTreasure(players[i].x, players[i].y, players[i].coinsCarried);
                killPlayer(&players[i]);
            }
        }
    }
}

void addObjectOnMap(unsigned char object) {
    int x, y;
    do {
        x = rand() % MAP_SIZE_X;
        y = rand() % MAP_SIZE_Y;
    } while (map[y][x] != EMPTY);

    map[y][x] = object;
}

void campsiteSpawn() {
    do {
        campsiteX = rand() % MAP_SIZE_X;
        campsiteY = rand() % MAP_SIZE_Y;
    } while (map[campsiteY][campsiteX] != EMPTY);
    map[campsiteY][campsiteX] = CAMPSITE;
}
