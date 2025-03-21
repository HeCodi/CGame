#include <stdio.h>
#include <stdlib.h>
#include "filestream.h"

#define _CRT_SECURE_NO_WARNINGS

int main() {
	char buffer[32];
	FILE* f = create_filestream("map.txt", &buffer);

	char** text;
	size_t text_size = NULL;

	read_text_in_file(f, &text, &text_size);

}

int read_text_in_file(FILE* filestream, char*** ptext, size_t* text_size) {
	size_t local_buffer_size = buffer_size;
	char* data = NULL, * buffer = (char*)malloc(local_buffer_size);

	if (buffer == NULL)
		return -1;

	if ((*ptext = (char*)malloc(sizeof(char**))) == NULL) {
		free(buffer);
		return -1;
	}

	for (size_t line_index = 0, bytes_read = 0; !feof(filestream);) {

		bytes_read = fread(buffer, sizeof(char),
			sizeof(*buffer) * local_buffer_size, filestream);
		if (bytes_read == 0) break;


		for (size_t i_read_bytes = 0; i_read_bytes < bytes_read; i_read_bytes++) {
			(*text_size)++;

			char* data_tmp = (char*)realloc(data, sizeof(char) * *text_size);

			if (data_tmp == NULL)
			{
				free(buffer);
				return -1;
			}
			data = data_tmp;
			data_tmp = NULL;

			data[*text_size - 1] = buffer[i_read_bytes];

			if (**ptext != data) {
				for (size_t i_p = 0, offset_p = 0; i_p <= line_index; i_p++) {
					offset_p = (size_t)(*ptext)[line_index - i_p] - (size_t)(**ptext);
					
					(*ptext)[line_index - i_p] = (size_t)data + offset_p;						
				}
			}

			if (buffer[i_read_bytes] == '\n') {
				if ((**ptext = (char*)realloc(**ptext, sizeof(**ptext) * line_index + 1)) == NULL) {
					free(buffer);
					return -1;
				}
				
				line_index++;
				(*ptext)[line_index] = &data[*text_size];
			}
		}
	}
	free(buffer);
	return 0;
}

FILE* create_filestream(const char* filepath, char* buffer) {
	FILE* file;

	file = fopen(filepath, "r+");

	if (setvbuf(file, buffer, _IOFBF, sizeof(buffer)) != 0) {
		fclose(file);
		file = NULL;
		return (FILE*)NULL;
	}

	return file;
}