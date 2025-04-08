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

size_t *map_line_sizies, *map_lines_count, *map_text_size, *point_count, map_end = 0;
VECTOR start_map_position, player_position, start_position, finish_position, *points;

char *buffer_1, *buffer_2;
char *frontbuffer, *backbuffer;

int process_move(VECTOR new_player_position)
{
    if (new_player_position.y > *map_lines_count)
        return -1;

    if (new_player_position.x > map_lines_count[new_player_position.y])
        return -1;

    if (base_map[new_player_position.y][new_player_position.x] == wall_char)
        return 0;

    player_position = new_player_position;

    update_map();

    process_get_point();
    print_buffer();

    process_exit();

    return 1;
}

int process_exit()
{
    if (finish_position.x == player_position.x && finish_position.y == player_position.y)
        if (coins == (*point_count))
            exit(0);
}

int process_get_point()
{
    for (size_t i = 0; i <= (*point_count - coins); i++)
        if (player_position.x == points[i].x && player_position.y == points[i].y)
        {
            coins++;
            base_map[points[i].y][points[i].x] = ' ';

            VECTOR *tmp_points = malloc(sizeof(VECTOR) * (*point_count - coins));

            for (size_t j = 0, curr = 0; curr <= (*point_count - coins); curr++)
            {
                if (curr == i)
                    continue;

                tmp_points[j] = points[curr];
                j++;
            }
            free(points);
            points = tmp_points;

            break;
        }

    update_backpack();
}

void print_buffer()
{
    printf("\e[?25l");
    backbuffer[buffer_size - 1] = '\0';

    char *last_back_buffer = backbuffer;
    backbuffer = frontbuffer;
    frontbuffer = last_back_buffer;
    printf(frontbuffer);
    fflush(stdout);
}

int update_map()
{
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
    map_end = i_buffer;
}

int update_backpack()
{
    size_t chars_write = sprintf(backbuffer + map_end, "\033[%zu;4H", *map_lines_count + 1);
    chars_write += sprintf(backbuffer + map_end + chars_write, "%s", "Backpack: ");

    for (size_t i = 0; i < coins; i++)
    {
        chars_write += sprintf(backbuffer + map_end + chars_write, "%c ", ((char)point_char));
    }
}

int load_map(const char *path)
{
    FILE *fs = (create_filestream(path));

    char *map_text;

    map_text_size = read_text_in_filestream(fs, &map_text);
    base_map = convert_text(map_text, *map_text_size, &map_line_sizies, &map_lines_count);
    fclose(fs);

    start_position = *find_char_in_text(start_char, base_map, map_line_sizies, *map_lines_count);
    base_map[start_position.y][start_position.x] = ' ';

    finish_position = *find_char_in_text(finish_char, base_map, map_line_sizies, *map_lines_count);

    player_position = start_position;

    point_count = malloc(sizeof(size_t));
    points = find_chars_in_text(point_char, base_map, map_line_sizies, *map_lines_count, -1, point_count);

    update_buffer();
    update_map();
    update_backpack();

    print_buffer();
    return *map_text_size;
}

int update_buffer()
{
    buffer_size = *map_text_size + ((sizeof(point_char + ' ') - 1) * (*point_count)) + ((sizeof("\033[;H") - 1 + ((10 * 2))) * (*map_lines_count + 1)) + sizeof("Backpack: ") - 1 + (sizeof("\033[2J") - 1) + sizeof("");
    map_end = *map_text_size + ((sizeof("\033[;H") - 1 + (10 * 2)) * (*map_lines_count + 1)) + (sizeof("\033[2J") - 1) + sizeof("");

    free(buffer_1);
    free(buffer_2);

    buffer_1 = malloc(buffer_size);
    buffer_2 = malloc(buffer_size);

    frontbuffer = buffer_1;
    backbuffer = buffer_2;

    for (size_t i = 0; i < buffer_size; i++)
    {
        backbuffer[i] = ' ';
        frontbuffer[i] = ' ';
    }

    return 0;
}