cmake_minimum_required(VERSION 3.5)

project(CGame)	

include_directories(include)

set(SOURCE_FILES
    src/main.c
    src/map_handler.c
    src/filestream.c
)

add_executable(CGame ${SOURCE_FILES})