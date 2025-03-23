#include <stdio.h>
#include "..\include\map_hendler.h"

int main() {
	int a;
	if(update_map("map.txt")){
		print_map();
	}
	scanf("%d", &a);
}