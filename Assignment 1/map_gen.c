#include <stdio.h> //includes printf
#include <stdlib.h> // includes malloc

#define SIZE_X 21
#define SIZE_Y 80

typedef struct cell {
    char type;
} cell_t;

typedef struct map {
    cell_t map_chars[SIZE_X][SIZE_Y];
    int N_path_start, S_path_start, E_path_start, W_path_start; 
} map_t;


void init_map(map_t *map){
    int i, j;
    for(i = 0; i < SIZE_X; i++){
        for(j = 0 ; j < SIZE_Y; j++){
            // cell_t *cell_map = map->map_chars[i][j];
            if(!i || !j || i == SIZE_X - 1 || j == SIZE_Y - 1){
                // cell_map->type = '%';
                map->map_chars[i][j].type = '%';
            }
            else{
                // cell_map->type = ' ';
                map->map_chars[i][j].type = ' ';
            }
        }
    }
}

void print_map(map_t *map){
    int i, j;

    for(i = 0; i < SIZE_X; i++){
        for(j = 0 ; j < SIZE_Y; j++){
            printf("%c", map->map_chars[i][j].type);
        }
        printf("\n");
    }
}

int main(int argc, char *argv[]){
    map_t map;
    // printf("%s", map->map_chars[0][0]->type);
    init_map(&map);
    print_map(&map);
    int a = 0;
    a++;
    return 0;
}