#include <stdio.h>
#include <stdlib.h>
#include "string_helper.h"
#include "map_handler.h"
#include "filestream.h"

#define player_char 'C'

#define start_char 'S'
#define finish_char 'F'
#define wall_char '#'
#define point_char '*'

char **base_map;

size_t coins = 0;

int process_move(VECTOR *new_player_position)
{
    if ((*new_player_position).y > *(MapData.map_lines_count))
        return -1;

    if ((*new_player_position).x > MapData.map_lines_count[(*new_player_position).y])
        return -1;

    if (base_map[(*new_player_position).y][(*new_player_position).x] == wall_char)
        return 0;

    *(MapData.player_position) = *new_player_position;

    free(new_player_position);
    new_player_position = NULL;

    update_map();

    process_get_point();
    print_buffer();

    process_exit();

    return 1;
}

int process_exit()
{
    if ((*MapData.finish_position).x == (*MapData.player_position).x && (*MapData.finish_position).y == (*MapData.player_position).y)
        if (coins == (*MapData.point_count))
            exit(0);

    return 0;
}

int process_get_point()
{
    for (size_t i = 0; i <= ((*MapData.point_count) - coins); i++)
        if ((*MapData.player_position).x == MapData.points[i].x && (*MapData.player_position).y == MapData.points[i].y)
        {
            coins++;
            base_map[MapData.points[i].y][MapData.points[i].x] = ' ';

            VECTOR *tmp_points = malloc(sizeof(VECTOR) * ((*MapData.point_count) - coins));

            for (size_t j = 0, curr = 0; curr <= ((*MapData.point_count) - coins); curr++)
            {
                if (curr == i)
                    continue;

                tmp_points[j] = (MapData.points)[curr];
                j++;
            }
            free((*MapData.point_count));
            (*MapData.point_count) = tmp_points;
            tmp_points = NULL;

            break;
        }

    update_backpack();

    return 0;
}

void print_buffer()
{
    printf("\e[?25l");
    BufferData.backbuffer[buffer_size - 1] = '\0';

    char *last_back_buffer = BufferData.backbuffer;
    BufferData.backbuffer = BufferData.frontbuffer;
    BufferData.frontbuffer = last_back_buffer;
    printf(BufferData.frontbuffer);
    fflush(stdout);

    return 0;
}

int update_map()
{
    size_t i_buffer = 0;
    int bytes_write = sprintf(BufferData.backbuffer, "\033[2J");
    i_buffer += bytes_write - 1;

    for (size_t y = 0; y < *MapData.map_lines_count; y++)
    {
        bytes_write = sprintf(BufferData.backbuffer + i_buffer, "\033[%zu;%zuH", (*MapData.start_map_position).y + y + 1, (*MapData.start_map_position).x + 1);
        i_buffer += bytes_write - 1;

        for (size_t x = 0; x < MapData.map_line_sizies[y]; x++)
        {
            i_buffer++;

            if ((*MapData.player_position).y == y && (*MapData.player_position).x == x)
            {
                BufferData.backbuffer[i_buffer] = (player_char);
                continue;
            }
            BufferData.backbuffer[i_buffer] = base_map[y][x];
        }
        i_buffer++;
    }
    (*MapData.map_end) = i_buffer;

    return 0;
}

int update_backpack()
{
    size_t chars_write = sprintf(BufferData.backbuffer + *(MapData.map_end), "\033[%zu;4H", *MapData.map_lines_count + 1);
    chars_write += sprintf(BufferData.backbuffer + *(MapData.map_end) + chars_write, "%s", "Backpack: ");

    for (size_t i = 0; i < coins; i++)
    {
        chars_write += sprintf(BufferData.backbuffer + *(MapData.map_end) + chars_write, "%c ", ((char)point_char));
    }

    return 0;
}

int load_map(const char *path)
{
    FILE *fs = (create_filestream(path));
    char *map_text;

    MapData.map_end = maloc(sizeof(size_t));

    MapData.map_text_size = read_text_in_filestream(fs, &map_text);
    base_map = convert_text(map_text, *MapData.map_text_size, &MapData.map_line_sizies, &MapData.map_lines_count);
    fclose(fs);

    MapData.start_position = find_char_in_text(start_char, base_map, MapData.map_line_sizies, *MapData.map_lines_count);
    base_map[(*MapData.start_position).y][(*MapData.start_position).x] = ' ';

    MapData.finish_position = find_char_in_text(finish_char, base_map, MapData.map_line_sizies, *MapData.map_lines_count);

    *MapData.player_position = *MapData.start_position;

    MapData.point_count = malloc(sizeof(size_t));
    MapData.points = find_chars_in_text(point_char, base_map, MapData.map_line_sizies, *MapData.map_lines_count, -1, MapData.point_count);

    update_buffer();
    update_map();
    update_backpack();

    print_buffer();

    free(MapData.start_position);
    MapData.start_position = NULL;

    return *MapData.map_text_size;
}

int freeMapData()
{
    for (size_t i = 0; i < sizeof(MapData) / sizeof(void *); i++)
    {
        size_t offSet = (i * sizeof(void *));
        free(&MapData + offSet);
    }
}

int update_buffer()
{
    buffer_size = *MapData.map_text_size + ((sizeof(point_char + ' ') - 1) * (*MapData.point_count)) + ((sizeof("\033[;H") - 1 + ((10 * 2))) * (*MapData.map_lines_count + 1)) + sizeof("Backpack: ") - 1 + (sizeof("\033[2J") - 1) + sizeof("");
    *MapData.map_end = *MapData.map_text_size + ((sizeof("\033[;H") - 1 + (10 * 2)) * (*MapData.map_lines_count + 1)) + (sizeof("\033[2J") - 1) + sizeof("");

    free(BufferData.buffer_1);
    free(BufferData.buffer_2);

    BufferData.buffer_1 = malloc(buffer_size);
    BufferData.buffer_2 = malloc(buffer_size);

    BufferData.frontbuffer = BufferData.buffer_1;
    BufferData.backbuffer = BufferData.buffer_2;

    for (size_t i = 0; i < buffer_size; i++)
    {
        BufferData.backbuffer[i] = ' ';
        BufferData.frontbuffer[i] = ' ';
    }

    return 0;
}

int freeBufferData()
{
    free(BufferData.buffer_1);
    free(BufferData.buffer_2);

    return 0;
}