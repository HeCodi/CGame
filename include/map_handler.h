#pragma once

#include <stdio.h>

typedef struct vector
{
    size_t x;
    size_t y;
} VECTOR;

extern struct MapData
{
    char **base_map;

    size_t *coins;

    size_t *map_line_sizies, *map_lines_count, *map_text_size, *point_count, *map_end;
    VECTOR *start_map_position, *player_position, *start_position, *finish_position, *points;
} MapData;
extern struct BufferData
{
    char *buffer_1, *buffer_2;
    char *frontbuffer, *backbuffer;
} BufferData;

int process_move(VECTOR *new_player_position);
int update_map();
int update_backpack();
int update_buffer();
int load_map(const char *path);
int process_get_point();
int process_exit();

void print_buffer();