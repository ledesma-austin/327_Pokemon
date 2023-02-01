#include <stdio.h> //includes printf
#include <stdlib.h> // includes malloc, srand
#include <time.h>

// size of map on terminal
#define SIZE_X 21
#define SIZE_Y 80

enum cell_type{Mountain = '%', Tree = '^', Road = '#', PokeMart = 'M', PokeCenter = 'C',
                    Long_Grass = ':', Clearing = '.', Water = '~', PC = '@', Empty = ' '};

// To hold any info a cell may need
typedef struct cell {
    enum cell_type type;
} cell_t;

// Holds a single map
typedef struct map {
    cell_t map_chars[SIZE_X][SIZE_Y];
    int N_path_start, S_path_start, E_path_start, W_path_start; 
} map_t;

// places random tiles onto the map
void rand_seed(map_t *map){
    int i, j, found, rand_type, rand_spot_i, rand_spot_j;
    enum cell_type seed_list[12] = {Empty, Empty, Empty, Empty, Empty, Empty, 
                                Empty, Empty, Empty, Empty, Empty, Empty};
    int next_spot = rand() % 12;
    int next_type = Long_Grass;
    
    // places the required areas in the list
    for(i = 0; i < 5; i++){
        found = 1;
        switch(i){
            case 0:
            case 1:
                next_type = Long_Grass;
                break;
            case 2:
            case 3:
                next_type = Clearing;
                break;
            case 4:
                next_type = Water;
        }
        // keep getting a random array spot until you find an open spot
        while(found){
            next_spot = rand() % 12;
            if(seed_list[next_spot] == Empty){
                seed_list[next_spot] = next_type;
                found = 0;
            }
        }
    }
    
    // fill in the remaining slots
    for(i = 0; i < 12; i++){
        if(seed_list[i] == Empty){
            rand_type = rand() % 5;
            switch(rand_type){
                case 0:
                    next_type = Mountain;
                    break;
                case 1:
                    next_type = Tree;
                    break;
                case 2:
                    next_type = Clearing;
                    break;
                case 3:
                    next_type = Long_Grass;
                    break;
                case 4:
                    next_type = Water;
                    break;
            }
            seed_list[i] = next_type;
        }
    }

    // splits the map into 12 spots and places one in each of those
    for(i = 0; i < 3; i++){
        for(j = 0; j < 4; j++){
            found = 1;
            // make sure that the random spot isn't on the border
            while(found){
                rand_spot_i = (rand() % 7) + (i * 7);
                rand_spot_j = (rand() % 20) + (j * 20);
                if(rand_spot_i != 0 && rand_spot_i != 79 && rand_spot_j != 0 && rand_spot_j != 20){
                    found = 0;
                }
            }
            map->map_chars[rand_spot_i][rand_spot_j].type = seed_list[i + j];
        }
    }

}

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
                map->map_chars[i][j].type = Empty;
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
    srand(time(NULL));

    init_map(&map);
    rand_seed(&map);
    print_map(&map);
    
    return 0;
}