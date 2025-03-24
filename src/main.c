#include <stdio.h>
#include "map_handler.h"


int main() {
	int a = 0;
	if(update_map("map.txt")){
		print_map();
	}
	scanf("%d", &a);
}