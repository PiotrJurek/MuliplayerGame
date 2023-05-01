#include "clientInterface.h"
#include <ncurses.h>
#include "../globalConfig.h"
#include "clientGameData.h"

void printMyPlayer() {
    if (inColor)
        attron(COLOR_PAIR(PLAYER_COLOR));
    mvaddch(myPlayer.y, myPlayer.x, myPlayerNumber + '0');
    if (inColor)
        attroff(COLOR_PAIR(COIN_COLOR));
}

void clearMap() {
    for (int i = 0; i < MAP_SIZE_Y; i++) {
        for (int j = 0; j < MAP_SIZE_X; j++) {
            mvaddch(i, j, EMPTY);
        }
    }
}

void printMap() {
    for (int i = 0; i < PLAYER_VIEW_RANGE * 2 + 2; i++) {
        for (int j = 0; j < PLAYER_VIEW_RANGE * 2 + 2; j++) {
            if (!(i + myPlayer.y - PLAYER_VIEW_RANGE >= 0 && i + myPlayer.y - PLAYER_VIEW_RANGE < MAP_SIZE_Y &&
                  j + myPlayer.x - PLAYER_VIEW_RANGE >= 0 && j + myPlayer.x - PLAYER_VIEW_RANGE < MAP_SIZE_X)) {
                continue;
            }
            if (map[i][j] >= '0' && map[i][j] <= '9') {
                if (inColor)
                    attron(COLOR_PAIR(PLAYER_COLOR));
                mvaddch(i + myPlayer.y - PLAYER_VIEW_RANGE, j + myPlayer.x - PLAYER_VIEW_RANGE, map[i][j]);
                if (inColor)
                    attroff(COLOR_PAIR(PLAYER_COLOR));
                continue;
            }
            switch (map[i][j]) {
                case COIN:
                    if (inColor)
                        attron(COLOR_PAIR(COIN_COLOR));
                    mvaddch(i + myPlayer.y - PLAYER_VIEW_RANGE, j + myPlayer.x - PLAYER_VIEW_RANGE, map[i][j]);
                    if (inColor)
                        attroff(COLOR_PAIR(COIN_COLOR));
                    break;
                case TREASURE:
                    if (inColor)
                        attron(COLOR_PAIR(TREASURE_COLOR));
                    mvaddch(i + myPlayer.y - PLAYER_VIEW_RANGE, j + myPlayer.x - PLAYER_VIEW_RANGE, map[i][j]);
                    if (inColor)
                        attroff(COLOR_PAIR(TREASURE_COLOR));
                    break;
                case LARGE_TREASURE:
                    if (inColor)
                        attron(COLOR_PAIR(LARGE_TREASURE_COLOR));
                    mvaddch(i + myPlayer.y - PLAYER_VIEW_RANGE, j + myPlayer.x - PLAYER_VIEW_RANGE, map[i][j]);
                    if (inColor)
                        attroff(COLOR_PAIR(LARGE_TREASURE_COLOR));
                    break;
                case DROPPED_TREASURE:
                    if (inColor)
                        attron(COLOR_PAIR(DROPPED_TREASURE_COLOR));
                    mvaddch(i + myPlayer.y - PLAYER_VIEW_RANGE, j + myPlayer.x - PLAYER_VIEW_RANGE, map[i][j]);
                    if (inColor)
                        attroff(COLOR_PAIR(DROPPED_TREASURE_COLOR));
                    break;
                case CAMPSITE:
                    campsiteX = j + myPlayer.x - PLAYER_VIEW_RANGE;
                    campsiteY = i + myPlayer.y - PLAYER_VIEW_RANGE;
                    if (inColor)
                        attron(COLOR_PAIR(CAMPSITE_COLOR));
                    mvaddch(i + myPlayer.y - PLAYER_VIEW_RANGE, j + myPlayer.x - PLAYER_VIEW_RANGE, map[i][j]);
                    if (inColor)
                        attroff(COLOR_PAIR(CAMPSITE_COLOR));
                    break;
                case BEAST:
                    if (inColor)
                        attron(COLOR_PAIR(BEAST_COLOR));
                    mvaddch(i + myPlayer.y - PLAYER_VIEW_RANGE, j + myPlayer.x - PLAYER_VIEW_RANGE, map[i][j]);
                    if (inColor)
                        attroff(COLOR_PAIR(BEAST_COLOR));
                    break;
                default:
                    mvprintw(i + myPlayer.y - PLAYER_VIEW_RANGE, j + myPlayer.x - PLAYER_VIEW_RANGE, "%lc", map[i][j]);
            }
        }
    }
}

void printInterface() {
    mvaddstr(1, MAP_SIZE_X + MAP_INTERFACE_OFFSET, "Server's PID:");
    mvaddstr(2, MAP_SIZE_X + MAP_INTERFACE_OFFSET + 1, "Campsite X/Y:");
    mvaddstr(3, MAP_SIZE_X + MAP_INTERFACE_OFFSET + 1, "Round number:");

    mvaddstr(5, MAP_SIZE_X + MAP_INTERFACE_OFFSET, "Player:");
    mvaddstr(6, MAP_SIZE_X + MAP_INTERFACE_OFFSET + 1, "Number");
    mvaddstr(7, MAP_SIZE_X + MAP_INTERFACE_OFFSET + 1, "Type");
    mvaddstr(8, MAP_SIZE_X + MAP_INTERFACE_OFFSET + 1, "Curr X/Y");
    mvaddstr(9, MAP_SIZE_X + MAP_INTERFACE_OFFSET + 1, "Deaths");

    mvaddstr(12, MAP_SIZE_X + MAP_INTERFACE_OFFSET + 1, "Coins found");
    mvaddstr(13, MAP_SIZE_X + MAP_INTERFACE_OFFSET + 1, "Coins brought");

    mvaddstr(15, MAP_SIZE_X + MAP_INTERFACE_OFFSET, "Legend:");
    mvaddstr(16, MAP_SIZE_X + MAP_INTERFACE_OFFSET + 1, "1234 - players");
    mvprintw(17, MAP_SIZE_X + MAP_INTERFACE_OFFSET + 1, "%ls", L"█    - wall");
    mvaddstr(18, MAP_SIZE_X + MAP_INTERFACE_OFFSET + 1, "#    - bushes (slow down)");
    mvaddstr(19, MAP_SIZE_X + MAP_INTERFACE_OFFSET + 1, "*    - wild beast");
    mvaddstr(20, MAP_SIZE_X + MAP_INTERFACE_OFFSET + 1, "c    - one coin                  D – dropped treasure ");
    mvaddstr(21, MAP_SIZE_X + MAP_INTERFACE_OFFSET + 1, "t    - treasure (10 coins)");
    mvaddstr(22, MAP_SIZE_X + MAP_INTERFACE_OFFSET + 1, "T    - large treasure (50 coins)");
    mvaddstr(23, MAP_SIZE_X + MAP_INTERFACE_OFFSET + 1, "A    - campsite");

    if (!inColor) {
        return;
    }

    attron(COLOR_PAIR(PLAYER_COLOR));
    mvaddstr(16, MAP_SIZE_X + MAP_INTERFACE_OFFSET + 1, "1234");
    attroff(COLOR_PAIR(PLAYER_COLOR));
    attron(COLOR_PAIR(BEAST_COLOR));
    mvaddch(19, MAP_SIZE_X + MAP_INTERFACE_OFFSET + 1, '*');
    attroff(COLOR_PAIR(BEAST_COLOR));
    attron(COLOR_PAIR(COIN_COLOR));
    mvaddch(20, MAP_SIZE_X + MAP_INTERFACE_OFFSET + 1, 'c');
    attroff(COLOR_PAIR(COIN_COLOR));
    attron(COLOR_PAIR(DROPPED_TREASURE_COLOR));
    mvaddch(20, MAP_SIZE_X + MAP_INTERFACE_OFFSET + 34, 'D');
    attroff(COLOR_PAIR(DROPPED_TREASURE_COLOR));
    attron(COLOR_PAIR(TREASURE_COLOR));
    mvaddch(21, MAP_SIZE_X + MAP_INTERFACE_OFFSET + 1, 't');
    attroff(COLOR_PAIR(TREASURE_COLOR));
    attron(COLOR_PAIR(LARGE_TREASURE_COLOR));
    mvaddch(22, MAP_SIZE_X + MAP_INTERFACE_OFFSET + 1, 'T');
    attroff(COLOR_PAIR(LARGE_TREASURE_COLOR));
    attron(COLOR_PAIR(CAMPSITE_COLOR));
    mvaddch(23, MAP_SIZE_X + MAP_INTERFACE_OFFSET + 1, 'A');
    attroff(COLOR_PAIR(CAMPSITE_COLOR));
}

void printGameStats() {
    mvprintw(1, MAP_SIZE_X + MAP_INTERFACE_OFFSET + 14, "%d %d", serverPid, myPid);
    if (campsiteX < 0 || campsiteY < 0) {
        mvprintw(2, MAP_SIZE_X + MAP_INTERFACE_OFFSET + 15, "--/--");
    } else {
        mvprintw(2, MAP_SIZE_X + MAP_INTERFACE_OFFSET + 15, "%02d/%02d", campsiteX, campsiteY);
    }
    mvprintw(3, MAP_SIZE_X + MAP_INTERFACE_OFFSET + 15, "%d", roundCounter);

    mvprintw(6, MAP_SIZE_X + MAP_INTERFACE_OFFSET + 15, "%d", myPlayerNumber);
    mvprintw(7, MAP_SIZE_X + MAP_INTERFACE_OFFSET + 15, "%s", "HUMAN");
    mvprintw(8, MAP_SIZE_X + MAP_INTERFACE_OFFSET + 15, "%02d/%02d  ", myPlayer.x, myPlayer.y);
    mvprintw(9, MAP_SIZE_X + MAP_INTERFACE_OFFSET + 15, "%d       ", myPlayer.deaths);

    mvprintw(12, MAP_SIZE_X + MAP_INTERFACE_OFFSET + 15, "%d       ", myPlayer.coinsCarried);
    mvprintw(13, MAP_SIZE_X + MAP_INTERFACE_OFFSET + 15, "%d       ", myPlayer.coinsBrought);
}

void printUpdatedPlayerLocation() {
    if (myPlayer.stun) {
        return;
    }
    printMap();
    switch (myPlayer.move) {
        case KEY_UP:
            if (map[PLAYER_VIEW_RANGE - 1][PLAYER_VIEW_RANGE] != WALL) {
                myPlayer.y--;
            }
            break;
        case KEY_DOWN:
            if (map[PLAYER_VIEW_RANGE + 1][PLAYER_VIEW_RANGE] != WALL) {
                myPlayer.y++;
            }
            break;
        case KEY_LEFT:
            if (map[PLAYER_VIEW_RANGE][PLAYER_VIEW_RANGE - 1] != WALL) {
                myPlayer.x--;
            }
            break;
        case KEY_RIGHT:
            if (map[PLAYER_VIEW_RANGE][PLAYER_VIEW_RANGE + 1] != WALL) {
                myPlayer.x++;
            }
            break;
    }
    printMyPlayer();
    move(MAP_SIZE_Y - 1, MAP_SIZE_X);
    refresh();
}

void printEverything() {
    clearMap();
    printMap();
    printMyPlayer();
    printInterface();
    printGameStats();
    move(MAP_SIZE_Y - 1, MAP_SIZE_X);
    refresh();
}
