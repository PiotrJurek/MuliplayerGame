cmake_minimum_required(VERSION 3.22)
project(client C)

set(CMAKE_C_STANDARD 99)
set(CURSES_NEED_NCURSES TRUE)
set(CURSES_NEED_WIDE TRUE)

include(FindCurses)

find_package(Curses REQUIRED)
include_directories(${CURSES_INCLUDE_DIR})

add_executable(client main.c ../globalConfig.h ../globalConfig.c clientCommunication.c clientCommunication.h clientGameData.c clientGameData.h clientInterface.c clientInterface.h)
target_link_libraries(${PROJECT_NAME} -lncursesw)