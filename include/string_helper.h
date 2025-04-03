#pragma once
#include <stdio.h>
#include "map_handler.h"

char **convert_text(char *text, size_t text_size, size_t **lines_size, size_t **lines_count);
VECTOR *find_char_in_text(char ch, char **text, size_t *lines_size, size_t lines_count);