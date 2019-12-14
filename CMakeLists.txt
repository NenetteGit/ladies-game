cmake_minimum_required(VERSION 3.10)

set(PROJECT ladies-game)

project(${PROJECT})

# set (HEADERS headers/board.h)

set(SRCS src/game.c src/board.c src/data_type.c src/board.h src/data_type.h)

add_executable(${PROJECT} ${SRCS} ${HEADERS})

include_directories(${CMAKE_CURRENT_SOURCE_DIR})

target_compile_options(${PROJECT} PRIVATE -Wall -Wextra -pedantic)