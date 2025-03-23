#include <stdio.h>
#include "..\include\filestream.h"
#include "..\include\map_hendler.h"

int print_map(){    
    if(map_text_size == 0) 
        return 0;

    system("cls");
    for(size_t i = 0, printed; printed < map_text_size; i++){
        printed += lines_size[i];

        for(size_t j = 0; j < lines_size[i]; j++){
            putchar(map[i][j]);
        }
    }
    return 1;
}

int update_map(const char* path){
    FILE fs = *(create_filestream(path));

    map_text_size = *(read_text_in_filestream(&fs, &map, &lines_size));

    fclose(&fs);
    return map_text_size;
}