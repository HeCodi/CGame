#pragma once

#include <stdio.h>

extern size_t buffer_size;

FILE *create_filestream(const char* filepath);

size_t read_text_in_filestream(FILE* filestream, char** ptext);