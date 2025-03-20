#include<stdio.h>
#include <stdlib.h>

#define _CRT_SECURE_NO_WARNINGS

FILE* create_filestream(const char* filepath, char* buffer);
int read_text_in_file(FILE* filestream, char*** ptext, size_t* text_size);

int main() {
	char* buffer[32];
	FILE* f = create_filestream("map.txt", buffer);

	char **text;
	size_t text_size = NULL;

	read_text_in_file(f, &text, &text_size);
}
  
int read_text_in_file(FILE* filestream, char*** ptext, size_t* text_size){
	char* buffer[32];
	size_t bytes_read, nums = 0, j = 0;
	*text_size = 0;

	*ptext = malloc(sizeof(*ptext));

	while (!feof(filestream)) {
		bytes_read = fread(buffer, sizeof(char), sizeof(*buffer), filestream);
		
		if (ferror(filestream) || bytes_read == 0) return -1;

		*text_size += bytes_read;

		for (size_t i = 0; i < bytes_read; i++) {
			j++;
			*ptext[nums] = (char*)realloc((*ptext)[nums], sizeof((*ptext)[nums]) * j + 1);
			(*ptext)[nums][j] = *(buffer[i]);

			if (*(buffer[i]) == '\0') {
				nums++;
				j = 0;
				*ptext = (char**)realloc(*ptext, sizeof(*ptext) * nums + 1);
			}						 
		}
	}
	printf(ptext[0][0]);
	//for (size_t i = 0; i < *text_size; i++) {
		
	//}
	

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