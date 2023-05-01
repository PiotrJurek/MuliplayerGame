#include "serverInterface.h"
#include <ncurses.h>
#include "../globalConfig.h"
#include "serverBeast.h"
#include "serverGameLogic.h"

void printMap() {
    for (int i = 0; i < MAP_SIZE_Y; i++) {
        for (int j = 0; j < MAP_SIZE_X; j++) {
            switch (map[i][j]) {
                case WALL:
                case BUSH:
                    mvprintw(i, j, "%lc", map[i][j]);
                    break;
                case COIN:
                    if (inColor)
                        attron(COLOR_PAIR(COIN_COLOR));
                    mvprintw(i, j, "%lc", map[i][j]);
                    if (inColor)
                        attroff(COLOR_PAIR(COIN_COLOR));
                    break;
                case TREASURE:
                    if (inColor)
                        attron(COLOR_PAIR(TREASURE_COLOR));
                    mvprintw(i, j, "%lc", map[i][j]);
                    if (inColor)
                        attroff(COLOR_PAIR(TREASURE_COLOR));
                    break;
                case LARGE_TREASURE:
                    if (inColor)
                        attron(COLOR_PAIR(LARGE_TREASURE_COLOR));
                    mvprintw(i, j, "%lc", map[i][j]);
                    if (inColor)
                        attroff(COLOR_PAIR(LARGE_TREASURE_COLOR));
                    break;
                case DROPPED_TREASURE:
                    if (inColor)
                        attron(COLOR_PAIR(DROPPED_TREASURE_COLOR));
                    mvprintw(i, j, "%lc", map[i][j]);
                    if (inColor)
                        attroff(COLOR_PAIR(DROPPED_TREASURE_COLOR));
                    break;
                case CAMPSITE:
                    if (inColor)
                        attron(COLOR_PAIR(CAMPSITE_COLOR));
                    mvprintw(i, j, "%lc", map[i][j]);
                    if (inColor)
                        attroff(COLOR_PAIR(CAMPSITE_COLOR));
                    break;
                default:
                    mvprintw(i, j, "%lc", map[i][j]);
                    break;
            }
        }
    }
}

void printInterface() {
    mvaddstr(1, MAP_SIZE_X + MAP_INTERFACE_OFFSET, "Server's PID:");
    mvaddstr(2, MAP_SIZE_X + MAP_INTERFACE_OFFSET + 1, "Campsite X/Y:");
    mvaddstr(3, MAP_SIZE_X + MAP_INTERFACE_OFFSET + 1, "Round number:");

    mvaddstr(5, MAP_SIZE_X + MAP_INTERFACE_OFFSET, "Parameter:   Player1  Player2  Player3  Player4");
    mvaddstr(6, MAP_SIZE_X + MAP_INTERFACE_OFFSET + 1, "PID");
    mvaddstr(7, MAP_SIZE_X + MAP_INTERFACE_OFFSET + 1, "Type");
    mvaddstr(8, MAP_SIZE_X + MAP_INTERFACE_OFFSET + 1, "Curr X/Y");
    mvaddstr(9, MAP_SIZE_X + MAP_INTERFACE_OFFSET + 1, "Deaths");

    mvaddstr(11, MAP_SIZE_X + MAP_INTERFACE_OFFSET, "Coins");
    mvaddstr(12, MAP_SIZE_X + MAP_INTERFACE_OFFSET + 1, "carried");
    mvaddstr(13, MAP_SIZE_X + MAP_INTERFACE_OFFSET + 1, "brought");

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

void printPlayersStats() {
    for (int i = 0; i < NUMBER_OF_PLAYERS; i++) {
        if (players[i].isActive) {
            mvprintw(6, MAP_SIZE_X + MAP_INTERFACE_OFFSET + 13 + (9 * i), "%d       ", players[i].pid);
            mvprintw(7, MAP_SIZE_X + MAP_INTERFACE_OFFSET + 13 + (9 * i), "HUMAN");
            mvprintw(8, MAP_SIZE_X + MAP_INTERFACE_OFFSET + 13 + (9 * i), "%02d/%02d  ", players[i].x, players[i].y);
            mvprintw(9, MAP_SIZE_X + MAP_INTERFACE_OFFSET + 13 + (9 * i), "%d       ", players[i].deaths);

            mvprintw(12, MAP_SIZE_X + MAP_INTERFACE_OFFSET + 13 + (9 * i), "%d       ", players[i].coinsCarried);
            mvprintw(13, MAP_SIZE_X + MAP_INTERFACE_OFFSET + 13 + (9 * i), "%d       ", players[i].coinsBrought);
        } else {
            mvprintw(6, MAP_SIZE_X + MAP_INTERFACE_OFFSET + 13 + (9 * i), "-      ");
            mvprintw(7, MAP_SIZE_X + MAP_INTERFACE_OFFSET + 13 + (9 * i), "-      ");
            mvprintw(8, MAP_SIZE_X + MAP_INTERFACE_OFFSET + 13 + (9 * i), "--/--");
            mvprintw(9, MAP_SIZE_X + MAP_INTERFACE_OFFSET + 13 + (9 * i), "-      ");

            mvprintw(12, MAP_SIZE_X + MAP_INTERFACE_OFFSET + 13 + (9 * i), "       ");
            mvprintw(13, MAP_SIZE_X + MAP_INTERFACE_OFFSET + 13 + (9 * i), "       ");
        }
    }
}

void printGameStats() {
    mvprintw(1, MAP_SIZE_X + MAP_INTERFACE_OFFSET + 14, "%d", myPid);
    mvprintw(2, MAP_SIZE_X + MAP_INTERFACE_OFFSET + 15, "%02d/%02d", campsiteX, campsiteY);
    mvprintw(3, MAP_SIZE_X + MAP_INTERFACE_OFFSET + 15, "%d", roundCounter);
}

void printPlayers() {
    if (inColor)
        attron(COLOR_PAIR(PLAYER_COLOR));
    for (int i = 0; i < NUMBER_OF_PLAYERS; i++)
        if (players[i].isActive)
            mvprintw(players[i].y, players[i].x, "%d", i + 1);
    if (inColor)
        attroff(COLOR_PAIR(PLAYER_COLOR));
}

void printBeasts() {
    if (inColor)
        attron(COLOR_PAIR(BEAST_COLOR));
    for (int i = 0; i < beastsAmount; i++)
        mvaddch(beasts[i].y, beasts[i].x, BEAST);
    if (inColor)
        attroff(COLOR_PAIR(BEAST_COLOR));
}

void printEverything() {
    printMap();
    printPlayers();
    printBeasts();
    printInterface();
    printGameStats();
    printPlayersStats();
    move(MAP_SIZE_Y - 1, MAP_SIZE_X);
    refresh();
}
