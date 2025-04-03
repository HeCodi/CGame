#pragma once
#include "map_handler.h"

extern char forvard, back, right, left;
extern VECTOR player_move;

char read_input();
int set_move_input(char _forvard, char _back, char _right, char _left);
int process_input(char input);