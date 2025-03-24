#pragma once

char **map;
size_t *lines_size,  map_text_size;

int print_map();
int update_map(const char* path);