#include <stdio.h>
#include "map_handler.h"
#include "player_handler.h"

int main()
{
	update_map("map.txt");
	set_move_input('w', 's', 'd', 'a');

	while (1)
	{
		process_input(read_input());
		print_map();
	}
}