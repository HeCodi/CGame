#pragma once
#include<stdio.h>

FILE* create_filestream(const char* filepath, char* buffer);

int read_text_in_file(FILE* filestream, char* buffer);