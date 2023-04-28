#include <ncurses.h>
#include "datatypes.h"
#ifndef MAP_GEN_H
#define MAP_GEN_H

void fillMap(int map_height, int map_width, char map[][map_width]);
int generateRooms(int map_height, int map_width, char map[][map_width], Room rooms[], int numRooms);
void init_non_overlaping_rooms (Room rooms[], int numRooms, Non_overlaping_rooms no_overlpg[], int nor_size);
void generateCorridors(int map_width, char map[][map_width], Non_overlaping_rooms rooms[], int nor_size);
void place_player(int map_height, int map_width, char map[][map_width], Character *character);
WINDOW *create_window (int height, int width, int startingX, int startingY);
void display_map (WINDOW *main_window, int map_height, int map_width, char map[][map_width]);

#endif