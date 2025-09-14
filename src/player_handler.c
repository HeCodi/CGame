#include <stdio.h>
#include <stdlib.h>
#include "player_handler.h"
#include "map_handler.h"

#if _WIN32
#include <conio.h>
#endif

char forvard, back, right, left;
VECTOR player_move;

int process_input(char input)
{
    VECTOR *new_player_position = (VECTOR *)malloc(sizeof(VECTOR));
    *new_player_position = *(MapData.player_position);

    if (input == forvard)
        (*new_player_position).y--;
    else if (input == back)
        (*new_player_position).y++;
    else if (input == right)
        (*new_player_position).x++;
    else if (input == left)
        (*new_player_position).x--;

    process_move(new_player_position);
}

char read_input()
{
#if _WIN32
    return getch();
#endif
}

int set_move_input(char _forvard, char _back, char _right, char _left)
{
    forvard = _forvard;
    back = _back;
    right = _right;
    left = _left;
}