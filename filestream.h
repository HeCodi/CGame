#pragma once

size_t buffer_size = 32;

FILE* create_filestream(const char* filepath, char* buffer);

int read_text_in_file(FILE* filestream, char*** ptext, size_t* text_size);