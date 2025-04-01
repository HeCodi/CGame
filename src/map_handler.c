#include <stdio.h>
#include "string_helper.h"
#include "filestream.h"

char **map;
size_t *map_line_sizies, *map_lines_count;

int print_map(){  
    for(size_t i = 0; i < *map_lines_count; i++){
        for(size_t j = 0; j < map_line_sizies[i]; j++){
            putchar(map[i][j]);       
        }
        putchar('\n'); 
    }
}

int update_map(const char* path){
    FILE *fs = (create_filestream(path));
    
    size_t map_text_size;
    char *map_text;

    
    map_text_size = read_text_in_filestream(fs, &map_text);
    map = convert_text(map_text, map_text_size, &map_line_sizies, &map_lines_count);

    fclose(fs);
    return map_text_size;
}