#include <stdio.h>
#include <stdlib.h>
#include "map_handler.h"

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

VECTOR *find_char_in_text(char ch, char **text, size_t *lines_size, size_t lines_count)
{
    VECTOR *pos = malloc(sizeof(VECTOR));

    pos->x = 0;
    pos->y = 0;

    for (size_t i = 0; i < lines_count; i++, pos->y++, pos->x = 0)
        for (size_t j = 0; j < lines_size[i]; j++, pos->x++)
            if (text[i][j] == ch)
                return pos;

    pos->y = lines_count;
    return pos;
}

VECTOR *find_chars_in_text(char ch, char **text, size_t *lines_size, size_t lines_count, size_t count, size_t *count_finded)
{
    if (count == 0)
        return NULL;

    VECTOR *pos = NULL;
    *count_finded = 0;

    for (size_t line = 0; *count_finded < count; (*count_finded)++)
    {
        VECTOR *tmp_pos = (VECTOR *)malloc(sizeof(VECTOR) * (*count_finded + 1));

        if (tmp_pos == NULL)
            return pos;

        if (pos != NULL)
            for (size_t i = 0; i < *count_finded; i++)
            {
                tmp_pos[i] = pos[i];
            }

        if (tmp_pos == NULL)
            return NULL;

        if (*count_finded == 0)
            tmp_pos[*count_finded].x = 0;
        else
            tmp_pos[*count_finded].x = tmp_pos[*count_finded - 1].x + 1;

        tmp_pos[*count_finded].y = line;
        char exit = 1;

        for (; line < lines_count; line++, tmp_pos[*count_finded].y++, tmp_pos[*count_finded].x = 0)
        {
            for (size_t j = tmp_pos[*count_finded].x; j < lines_size[line]; j++, tmp_pos[*count_finded].x++)
            {
                if (text[line][j] == ch)
                {
                    exit = 0;
                    break;
                }
            }
            if (!exit)
                break;
        }
        if (exit)
        {
            free(tmp_pos);
            break;
        }

        free(pos);
        pos = tmp_pos;
    }
    return pos;
}