#include <stdio.h>
#include "map_handler.h"

int main()
{
	update_map("map.txt");
	while (1)
	{
		print_map();
	}
}