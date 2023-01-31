#include <stdio.h> //includes printf
#include <stdlib.h> // includes malloc

#define SIZE_X 21
#define SIZE_Y 80

enum cell_type{Mountain = '%', Tree = '^', Road = '#', PokeMart = 'M', PokeCenter = 'C',
                    Long_Grass = ':', Clearing = '.', PC = '@', Empty = ' '};

// To hold any info a cell may need
typedef struct cell {
    enum cell_type type;
} cell_t;

// Holds a single map
typedef struct map {
    cell_t map_chars[SIZE_X][SIZE_Y];
    int N_path_start, S_path_start, E_path_start, W_path_start; 
} map_t;

// initializes the starting map with borders
void init_map(map_t *map){
    int i, j;
    for(i = 0; i < SIZE_X; i++){
        for(j = 0 ; j < SIZE_Y; j++){
            // if current cell is on the edge, make it a mountain
            // Mountain and Empty are enum cell_type
            if(!i || !j || i == SIZE_X - 1 || j == SIZE_Y - 1){
                map->map_chars[i][j].type = Mountain;
            }
            else{
                map->map_chars[i][j].type = Empty ;
            }
        }
    }
}

// prints map using enum cell_type values 
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

    init_map(&map);
    print_map(&map);
    
    return 0;
}