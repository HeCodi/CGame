#include <stdio.h>
#include <stdlib.h>

char **convert_text(char *text, size_t text_size, size_t **lines_size, size_t **lines_count)
{
    char **lines = NULL, *data, error = 0;
    size_t data_size = sizeof(size_t), offset_address;

    *lines_size = NULL;

    data = malloc(data_size);
    *lines_count = (size_t *)data;
    **lines_count = 0;

    for (size_t i = 0, i_char_line = 0; i < text_size; i++, i_char_line++)
    {
        if (text[i] == '\n' || i == (text_size - 1))
        {
            if (i >= (text_size - 1))
            {
                i++;
                i_char_line++;
            }

            (**lines_count)++;

            data_size += (sizeof(size_t) + sizeof(char *));

            char *tmp_data = malloc(data_size);
            if (tmp_data == NULL)
            {
                error = 1;
                break;
            }

            *((size_t *)tmp_data) = **lines_count;
            *lines_count = ((size_t *)tmp_data);

            offset_address = sizeof(size_t);
            for (size_t i = 0; i < (**lines_count - 1); i++)
                *((size_t *)(tmp_data + offset_address + (i * sizeof(size_t)))) = *((size_t *)(data + offset_address + (i * sizeof(size_t))));

            *((size_t *)(tmp_data + offset_address + ((**lines_count - 1) * sizeof(size_t)))) = (i_char_line);

            *lines_size = (size_t *)(tmp_data + offset_address);
            lines = (char **)(tmp_data + offset_address + (**lines_count * sizeof(size_t)));

            offset_address += **lines_count * sizeof(size_t);
            for (size_t i = 0; i < (**lines_count - 1); i++)
                *((char **)(tmp_data + offset_address + (i * sizeof(char *)))) = *((char **)(data + offset_address - sizeof(size_t) + (i * sizeof(char *))));

            *((char **)(tmp_data + offset_address + ((**lines_count - 1) * sizeof(char *)))) = &(text[i - i_char_line]);

            i_char_line = -1;
            free(data);
            data = tmp_data;
            tmp_data = NULL;
        }
    }

    return lines;
}
