#include <stdio.h>
#include <stdlib.h>

size_t buffer_size = 32;

size_t *read_text_in_filestream(FILE *filestream, char **ptext)
{
	size_t *text_size = malloc(sizeof(size_t)), local_buffer_size = buffer_size;
	char *data = NULL, *buffer;

	buffer = (char *)malloc(sizeof(char) * local_buffer_size);

	if (buffer == NULL || text_size == NULL)
		return 0;

	*text_size = 0;
	for (size_t read_bytes = 0; !feof(filestream);)
	{
		read_bytes = fread(buffer, sizeof(char),
						   local_buffer_size / sizeof(char), filestream);

		if (!read_bytes && ferror(filestream))
			break;

		*text_size += read_bytes;

		char *tmp_data = (char *)realloc(data, *text_size * sizeof(char));

		if (tmp_data == NULL)
		{
			break;
		}
		data = tmp_data;
		tmp_data = NULL;

		for (size_t i_read_bytes = 0; i_read_bytes < read_bytes; i_read_bytes++)
		{
			size_t data_index = *text_size - read_bytes + i_read_bytes;
			data[data_index] = buffer[i_read_bytes];
		}
	}

	free(buffer);
	*ptext = data;
	return text_size;
}

FILE *create_filestream(const char *filepath)
{
	FILE *file = fopen(filepath, "r+");
	return file;
}