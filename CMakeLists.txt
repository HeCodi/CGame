cmake_minimum_required(VERSION 3.5)

project(CGame)	

include_directories(include)

set(SOURCE_FILES
    src/filestream.c
    src/main.c
    src/map_handler.c
    src/string_helper.c
    src/player_handler.c
)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c90 -Wall -O3")

add_executable(CGame ${SOURCE_FILES})