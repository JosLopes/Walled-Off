#include <ncurses.h>
#include "datatypes.h"
#ifndef MAP_GEN_H
#define MAP_GEN_H

void fillMap(int map_height, int map_width, char map[][map_width]);
int generateRooms(int map_height, int map_width, char map[][map_width], Room rooms[], int numRooms);
void init_non_overlaping_rooms (Room rooms[], int numRooms, char no_overlpg[], int nor_size);
void generateCorridors(int map_width, char map[][map_width], Non_overlaping_rooms rooms[], int nor_size);
void place_player(int map_height, int map_width, char map[][map_width], Character *character);
void place_enemies(int map_height, int map_width, char map[][map_width], int num_goblins, int num_skeletons, int num_orcs);
WINDOW *create_window (int height, int width, int startingX, int startingY);
void display_map (WINDOW *main_window, int map_height, int map_width, char map[][map_width]);

#endif