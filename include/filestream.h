#pragma once

#include <stdio.h>

size_t buffer_size;

FILE *create_filestream(const char* filepath);

size_t *read_text_in_filestream(FILE* filestream, char*** ptext, size_t **lines_size);