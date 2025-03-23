#include <stdio.h>

size_t buffer_size = 32;

size_t* read_text_in_filestream(FILE* filestream, char*** ptext, size_t** lines_size) {
	size_t local_buffer_size = buffer_size;
	char* data = NULL, *buffer = (char*)malloc(local_buffer_size);
	size_t line_index = 0, *text_size = (size_t*)malloc(sizeof(size_t));

	if (buffer == NULL)
		*text_size = 0;
		return text_size;

	if (text_size == NULL) {
		free(buffer);
		*text_size = 0;
		return text_size;
	}

	if (((*lines_size) = (size_t*)malloc(sizeof(size_t))) == NULL) {
		free(buffer);
		*text_size = 0;
		return text_size;
	}

	if ((*ptext = (char*)malloc(sizeof(char**))) == NULL) {
		free(*lines_size);
		free(buffer);
		*text_size = 0;
		return text_size;
	}

	**lines_size = 0;
	*text_size = 0;

	for (size_t bytes_read = 0; !feof(filestream);) {

		bytes_read = fread(buffer, sizeof(char),
			sizeof(*buffer) * local_buffer_size, filestream);

		if (bytes_read == 0) break;

		for (size_t i_read_bytes = 0; i_read_bytes < bytes_read; i_read_bytes++, (*lines_size)[line_index]++) {
			(*text_size)++;

			char* data_tmp = (char*)realloc(data, sizeof(char) * (*text_size));
			if (data_tmp == NULL)
			{
				free(*lines_size);
				free(data);
				free(*ptext);
				free(buffer);
				*text_size = 0;
				return text_size;
			}
			data = data_tmp;
			data_tmp = NULL;

			data[(*text_size) - 1] = buffer[i_read_bytes];

			if (**ptext != data) {
				for (size_t i_p = 0, offset_p = 0; i_p <= line_index; i_p++) {
					offset_p = (size_t)(*ptext)[line_index - i_p] - (size_t)(**ptext);

					(*ptext)[line_index - i_p] = (size_t)data + offset_p;
				}
			}

			if ((*text_size) >= 2) {
				if (data[(*text_size) - 2] == '\n') {
					char** temp_ptext = (char**)realloc((*ptext), sizeof(char*) * (line_index + 2));

					if (temp_ptext == NULL) {
						free(*lines_size);
						free(data);
						free(*ptext);
						free(buffer);

						*text_size = 0;
						return text_size;
					}
					for (size_t i_pline = 0; i_pline <= line_index; i_pline++) {
						temp_ptext[i_pline] = (*ptext)[i_pline];
					}
					*ptext = temp_ptext;
					temp_ptext = NULL;

					size_t* temp_lines_size = (size_t*)realloc(*lines_size, sizeof(size_t) * (line_index + 2));
					if (temp_lines_size == NULL) {
						free(*lines_size);
						free(data);
						free(*ptext);
						free(buffer);
						*text_size = 0;
						return text_size;
					}
					for (size_t i_line_size = 0; i_line_size <= line_index; i_line_size++) {
						temp_lines_size[i_line_size] = (*lines_size)[i_line_size];
					}

					*lines_size = temp_lines_size;
					temp_lines_size = NULL;

					line_index++;
					(*ptext)[line_index] = &(data[(*text_size) - 1]);
					(*lines_size)[line_index] = 0;
				}
			}
		}
	}
	free(buffer);
	
	return text_size;
}

FILE* create_filestream(const char* filepath) {
	FILE* file = fopen(filepath, "r+");
	return file;
}