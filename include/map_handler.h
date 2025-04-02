#pragma once

#include <stdio.h>

extern char *map;
extern size_t map_text_size;

int print_map();
int update_map(const char *path);