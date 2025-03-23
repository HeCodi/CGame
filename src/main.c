
#include "filestream.h"

#define _CRT_SECURE_NO_WARNINGS

int main() {
	FILE* fs = create_filestream("map.txt");

	char** text;
	size_t* lines_size, *text_size;

	if ((text_size = read_text_in_file(fs, &text, &lines_size)) > 0) {
		for (size_t i = 0, printed = 0; printed < *text_size; i++) {
			printed += lines_size[i]; 
			for (size_t j = 0; j < lines_size[i]; j++) {
				printf("%c", text[i][j]);
			}					
		}
	}
}