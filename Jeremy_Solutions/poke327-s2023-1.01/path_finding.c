#include "heap.h"

typedef enum __attribute__ ((__packed__)) terrain_type {
  ter_boulder,
  ter_tree,
  ter_path,
  ter_mart,
  ter_center,
  ter_grass,
  ter_clearing,
  ter_mountain,
  ter_forest,
  ter_water,
  ter_gate,
  num_terrain_types
} terrain_type_t;

typedef enum character_type {
  char_pc,
  char_hiker,
  char_rival,
  num_character_types
} character_type_t;

typedef struct path {
  heap_node_t *hn;
  int32_t cost;
} path_t;

typedef struct map {
  terrain_type_t map[MAP_Y][MAP_X];
  uint8_t height[MAP_Y][MAP_X];
  uint8_t n, s, e, w;
} map_t;

static int32_t path_cmp(const void *key, const void *with) {
  return ((path_t *) key)->cost - ((path_t *) with)->cost;
}

int move_costs[num_character_types][num_terrain_types] = {
// Boulder     Tree     Path     Mart   Center    Grass Clearing Mountain   Forest    Water     Gate
  {INT_MAX, INT_MAX,      10,      10,      10,      20,      10, INT_MAX, INT_MAX, INT_MAX,      10}, // PC
  {INT_MAX, INT_MAX,      10,      50,      50,      15,      10,      15,      15, INT_MAX, INT_MAX}, // Hiker
  {INT_MAX, INT_MAX,      10,      50,      50,      20,      10, INT_MAX, INT_MAX, INT_MAX, INT_MAX}  // Rival
};

static void dijkstra_path(map_t *m)
{
  static path_t path[MAP_Y][MAP_X], *p;
  static uint32_t initialized = 0;
  heap_t h;
  uint32_t x, y;

  if (!initialized) {
    for (y = 0; y < MAP_Y; y++) {
      for (x = 0; x < MAP_X; x++) {
        path[y][x].pos[dim_y] = y;
        path[y][x].pos[dim_x] = x;
      }
    }
    initialized = 1;
  }
  
  for (y = 0; y < MAP_Y; y++) {
    for (x = 0; x < MAP_X; x++) {
      path[y][x].cost = INT_MAX;
    }
  }

  heap_init(&h, path_cmp, NULL);

  for (y = 1; y < MAP_Y - 1; y++) {
    for (x = 1; x < MAP_X - 1; x++) {
      path[y][x].hn = heap_insert(&h, &path[y][x]);
    }
  }

  while ((p = heap_remove_min(&h))) {
    p->hn = NULL;

    if ((path[p->pos[dim_y] - 1][p->pos[dim_x]    ].hn) &&
        (path[p->pos[dim_y] - 1][p->pos[dim_x]    ].cost >
         ((p->cost + heightpair(p->pos))))) {
      path[p->pos[dim_y] - 1][p->pos[dim_x]    ].cost =
        ((p->cost + heightpair(p->pos)));
      heap_decrease_key_no_replace(&h, path[p->pos[dim_y] - 1]
                                           [p->pos[dim_x]    ].hn);
    }
    if ((path[p->pos[dim_y]    ][p->pos[dim_x] - 1].hn) &&
        (path[p->pos[dim_y]    ][p->pos[dim_x] - 1].cost >
         ((p->cost + heightpair(p->pos))))) {
      path[p->pos[dim_y]][p->pos[dim_x] - 1].cost =
        ((p->cost + heightpair(p->pos)));
      heap_decrease_key_no_replace(&h, path[p->pos[dim_y]    ]
                                           [p->pos[dim_x] - 1].hn);
    }
    if ((path[p->pos[dim_y]    ][p->pos[dim_x] + 1].hn) &&
        (path[p->pos[dim_y]    ][p->pos[dim_x] + 1].cost >
         ((p->cost + heightpair(p->pos))))) {
      path[p->pos[dim_y]][p->pos[dim_x] + 1].cost =
        ((p->cost + heightpair(p->pos)));
      heap_decrease_key_no_replace(&h, path[p->pos[dim_y]    ]
                                           [p->pos[dim_x] + 1].hn);
    }
    if ((path[p->pos[dim_y] + 1][p->pos[dim_x]    ].hn) &&
        (path[p->pos[dim_y] + 1][p->pos[dim_x]    ].cost >
         ((p->cost + heightpair(p->pos))))) {
      path[p->pos[dim_y] + 1][p->pos[dim_x]    ].cost =
        ((p->cost + heightpair(p->pos)));
      heap_decrease_key_no_replace(&h, path[p->pos[dim_y] + 1]
                                           [p->pos[dim_x]    ].hn);
    }
    // y - 1, x - 1
    if ((path[p->pos[dim_y] - 1][p->pos[dim_x] - 1].hn) &&
        (path[p->pos[dim_y] - 1][p->pos[dim_x] - 1].cost >
         ((p->cost + heightpair(p->pos))))) {
      path[p->pos[dim_y] - 1][p->pos[dim_x] - 1].cost =
        ((p->cost + heightpair(p->pos)));
      heap_decrease_key_no_replace(&h, path[p->pos[dim_y] - 1]
                                           [p->pos[dim_x] - 1].hn);
    }
    // y + 1, x - 1
    if ((path[p->pos[dim_y] + 1][p->pos[dim_x] - 1].hn) &&
        (path[p->pos[dim_y] + 1][p->pos[dim_x] - 1].cost >
         ((p->cost + heightpair(p->pos))))) {
      path[p->pos[dim_y] + 1][p->pos[dim_x] - 1].cost =
        ((p->cost + heightpair(p->pos)));
      heap_decrease_key_no_replace(&h, path[p->pos[dim_y] + 1]
                                           [p->pos[dim_x] - 1].hn);
    }
    // y + 1, x + 1
    if ((path[p->pos[dim_y] + 1][p->pos[dim_x] + 1].hn) &&
        (path[p->pos[dim_y] + 1][p->pos[dim_x] + 1].cost >
         ((p->cost + heightpair(p->pos))))) {
      path[p->pos[dim_y] + 1][p->pos[dim_x] + 1].cost =
        ((p->cost + heightpair(p->pos)));
      heap_decrease_key_no_replace(&h, path[p->pos[dim_y] + 1]
                                           [p->pos[dim_x] + 1].hn);
    }
    // y - 1, x - 1
    if ((path[p->pos[dim_y] - 1][p->pos[dim_x] - 1].hn) &&
        (path[p->pos[dim_y] - 1][p->pos[dim_x] - 1].cost >
         ((p->cost + heightpair(p->pos))))) {
      path[p->pos[dim_y] - 1][p->pos[dim_x] - 1].cost =
        ((p->cost + heightpair(p->pos)));
      heap_decrease_key_no_replace(&h, path[p->pos[dim_y] - 1]
                                           [p->pos[dim_x] - 1].hn);
    }
  }
}

int main(int argc, char *argv[]){
    return 0;
}