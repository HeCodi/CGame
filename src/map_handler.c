#include <stdio.h>
#include <stdlib.h>
#include "string_helper.h"
#include "map_handler.h"
#include "filestream.h"

#define player_char 'C'
#define start_char 'S'
#define finish_char 'F'

char **base_map;

size_t *map_line_sizies, *map_lines_count, *map_text_size;
VECTOR start_map_position, player_position, start_position, finish_position;

char *buffer_1, *buffer_2;
char *frontbuffer, *backbuffer;

int process_move(VECTOR new_player_position)
{
    if (new_player_position.y > *map_lines_count)
        return -1;

    if (new_player_position.x > map_lines_count[new_player_position.y])
        return -1;

    if (base_map[new_player_position.y][new_player_position.x] == '#')
        return 0;

    player_position = new_player_position;
    return 1;
}

int print_map()
{
    printf("\e[?25l");
    size_t i_buffer = 0;
    int bytes_write = sprintf(backbuffer, "\033[2J");
    i_buffer += bytes_write - 1;

    for (size_t y = 0; y < *map_lines_count; y++)
    {
        bytes_write = sprintf(backbuffer + i_buffer, "\033[%zu;%zuH", start_map_position.y + y + 1, start_map_position.x + 1);
        i_buffer += bytes_write - 1;

        for (size_t x = 0; x < map_line_sizies[y]; x++)
        {
            i_buffer++;

            if (player_position.y == y && player_position.x == x)
            {
                backbuffer[i_buffer] = (player_char);
                continue;
            }
            backbuffer[i_buffer] = base_map[y][x];
        }
        i_buffer++;
    }
    backbuffer[i_buffer] = '\0';

    char *last_back_buffer = backbuffer;
    backbuffer = frontbuffer;
    frontbuffer = last_back_buffer;

    printf("%s", frontbuffer);
    fflush(stdout);
}

int update_map(const char *path)
{
    FILE *fs = (create_filestream(path));

    char *map_text;

    map_text_size = read_text_in_filestream(fs, &map_text);
    base_map = convert_text(map_text, *map_text_size, &map_line_sizies, &map_lines_count);
    fclose(fs);

    start_position = *find_char_in_text(start_char, base_map, map_line_sizies, *map_lines_count);
    finish_position = *find_char_in_text(finish_char, base_map, map_line_sizies, *map_lines_count);

    player_position = start_position;

    update_buffer();

    return *map_text_size;
}

int update_buffer()
{
    size_t buffer_size = *map_text_size + ((sizeof("\033[;H") - 1 + (10 * 2)) * (*map_lines_count)) + (sizeof("\033[2J") - 1) + sizeof("");

    free(buffer_1);
    free(buffer_2);

    buffer_1 = malloc(buffer_size);
    buffer_2 = malloc(buffer_size);

    frontbuffer = buffer_1;
    backbuffer = buffer_2;

    return 0;
}
