#include <stdio.h> //includes printf
#include <stdlib.h> // includes malloc, srand
#include <time.h>

// size of map on terminal
#define MAP_SIZE_X 21
#define MAP_SIZE_Y 80
#define WORLD_SIZE_X 400
#define WORLD_SIZE_Y 400

enum cell_type{Mountain = '%', Tree = '^', Road = '#', PokeMart = 'M', PokeCenter = 'C',
                    Long_Grass = ':', Clearing = '.', Water = '~', PC = '@', Empty = ' '};

typedef struct node {
    struct node *prev;
    int x, y;
} node_t;

// To hold any info a cell may need
typedef struct cell {
    enum cell_type type;
} cell_t;

// Holds a single map
typedef struct map {
    cell_t map_chars[MAP_SIZE_X][MAP_SIZE_Y];
    int N_path_start, S_path_start, E_path_start, W_path_start; 
} map_t;

typedef struct world {
    map_t world_maps[WORLD_SIZE_X][WORLD_SIZE_Y];
} world_t;

// adds pokemart and pokecenter to the map, making sure they are next to a road
void add_buildings(map_t *map){
    int rand_x, rand_y, found, i, j;
    
    found = 1;
    while(found){
        rand_x = (rand() % 19) + 1;
        rand_y = (rand() % 78) + 1;
        // checks surrounding spots for a road
        for(i = -1; i < 2; i++){
            for(j = -1; j < 2; j++){
                if(j != 0 && i != 0 && map->map_chars[rand_x + i][rand_y + j].type == Road){
                    map->map_chars[rand_x][rand_y].type = PokeMart;
                    found = 0;
                }
            }
        }
    }
    found = 1;
    while(found){
        rand_x = (rand() % 19) + 1;
        rand_y = (rand() % 78) + 1;
        
        for(i = -1; i < 2; i++){
            for(j = -1; j < 2; j++){
                // also makes sure to not override PokeMart
                if(j != 0 && i != 0 && map->map_chars[rand_x + i][rand_y + j].type == Road && map->map_chars[rand_x][rand_y].type != PokeMart){
                    map->map_chars[rand_x][rand_y].type = PokeCenter;
                    found = 0;
                }
            }
        }
    }
}

// gets a random value for N/S and E/W facing roads and makes a straight path across
void add_straight_road(map_t *map){
    int N_S_path, E_W_path, i, j;

    N_S_path = (rand() % (MAP_SIZE_Y - 10)) + 5;
    E_W_path = (rand() % (MAP_SIZE_X - 5)) + 2;
    
    map->N_path_start = N_S_path;
    map->S_path_start = N_S_path;
    map->E_path_start = E_W_path;
    map->W_path_start = E_W_path;

    for(i = 0; i < MAP_SIZE_X; i++){
        map->map_chars[i][N_S_path].type = Road;
    }

    for(j = 0; j < MAP_SIZE_Y; j++){
        map->map_chars[E_W_path][j].type = Road;
    }
}

// grows each cell left, right, up and down until the screen is filled
void grow(map_t *map){
    int i, j, x, y, next_i, next_j;
    enum cell_type current_symbol;
    int wasUpdated = 1;
    // holds 1 if [i][j] cell on map was updated this scan, 0 otherwise
    int isNew[80][21];

    // scans the map starting from top left to bottom right (excluding borders)
    // if you come across a cell that isn't empty and that cell wasn't grown this scan, grow it in cardinal directions
    // continue until nothing grows (board is filled)
    while(wasUpdated){
        wasUpdated = 0;
        // clear isNew
        for(i = 0; i < MAP_SIZE_X; i++){
            for(j = 0; j < MAP_SIZE_Y; j++){
                isNew[i][j] = 0;
            }
        }
        // scanning loops
        for(i = 1; i < MAP_SIZE_X - 1; i++){
            for(j = 1; j < MAP_SIZE_Y - 1; j++){
                current_symbol = map->map_chars[i][j].type;
                if(current_symbol != Empty && !isNew[i][j]){
                    // only does the cardinal directions
                    for(x = -1; x < 2; x++){
                        for(y = -1; y < 2; y++){
                            // if x + y is not even
                            if((x + y) % 2){
                                next_i = i + x;
                                next_j = j + y;
                                if(map->map_chars[next_i][next_j].type == Empty){
                                    map->map_chars[next_i][next_j].type = current_symbol;
                                    isNew[next_i][next_j] = 1;
                                    wasUpdated = 1;
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    
}

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
                rand_spot_i = (rand() % (MAP_SIZE_X / 3)) + (i * (MAP_SIZE_X / 3));
                rand_spot_j = (rand() % (MAP_SIZE_Y / 4)) + (j * (MAP_SIZE_Y / 4));
                if(rand_spot_i != 0 && rand_spot_i != (MAP_SIZE_X - 1) && rand_spot_j != 0 && rand_spot_j != (MAP_SIZE_Y - 1)){
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
    for(i = 0; i < MAP_SIZE_X; i++){
        for(j = 0 ; j < MAP_SIZE_Y; j++){
            // if current cell is on the edge, make it a mountain
            // Mountain and Empty are enum cell_type
            if(!i || !j || i == MAP_SIZE_X - 1 || j == MAP_SIZE_Y - 1){
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

    for(i = 0; i < MAP_SIZE_X; i++){
        for(j = 0 ; j < MAP_SIZE_Y; j++){
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
    // print_map(&map)
    grow(&map);
    add_straight_road(&map);
    add_buildings(&map);
    print_map(&map);
    
    return 0;
}