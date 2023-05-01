#include "globalConfig.h"
#include <ncurses.h>
#include <locale.h>
#include <unistd.h>

int inColor;
int myPid;

void initConfig() {
    setlocale(LC_ALL, "");
    initscr();
    keypad(stdscr, TRUE);
    nodelay(stdscr, TRUE);
    cbreak();
    noecho();

    inColor = has_colors();

    if (inColor) {
        start_color();

        init_pair(BEAST_COLOR, BEAST_TEXT_COLOR, BEAST_BG_COLOR);
        init_pair(COIN_COLOR, COIN_TEXT_COLOR, COIN_BG_COLOR);
        init_pair(TREASURE_COLOR, TREASURE_TEXT_COLOR, TREASURE_BG_COLOR);
        init_pair(LARGE_TREASURE_COLOR, LARGE_TREASURE_TEXT_COLOR, LARGE_TREASURE_BG_COLOR);
        init_pair(DROPPED_TREASURE_COLOR, DROPPED_TREASURE_TEXT_COLOR, DROPPED_TREASURE_BG_COLOR);
        init_pair(CAMPSITE_COLOR, CAMPSITE_TEXT_COLOR, CAMPSITE_BG_COLOR);
        init_pair(PLAYER_COLOR, PLAYER_TEXT_COLOR, PLAYER_BG_COLOR);
    }

    myPid = getpid();
}