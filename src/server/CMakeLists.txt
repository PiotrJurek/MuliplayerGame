cmake_minimum_required(VERSION 3.22)
project(server C)

set(CMAKE_C_STANDARD 99)
set(CURSES_NEED_NCURSES TRUE)
set(CURSES_NEED_WIDE TRUE)

include(FindCurses)

find_package(Curses REQUIRED)
include_directories(${CURSES_INCLUDE_DIR})

add_executable(server main.c ../globalConfig.h ../globalConfig.c serverInterface.c serverInterface.h serverCommunication.h serverCommunication.c serverDroppedTreasure.c serverDroppedTreasure.h serverBeast.c serverBeast.h serverGameLogic.c serverGameLogic.h serverPlayer.c serverPlayer.h)
target_link_libraries(${PROJECT_NAME} -lncursesw)