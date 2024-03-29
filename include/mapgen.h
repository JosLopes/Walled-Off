#include <ncurses.h>
#include "datatypes.h"
#ifndef MAP_GEN_H
#define MAP_GEN_H

void fillMap(int map_height, int map_width, char **map);
void fillTraveledPath(int map_height, int map_width, char traveled_path[][map_width]);
int generateRooms(int map_height, int map_width, char **map, Room rooms[], int numRooms);
void init_non_overlaping_rooms (Room rooms[], int numRooms, Non_overlaping_rooms no_overlpg[], int nor_size);
void generateCorridors(char **map, Non_overlaping_rooms rooms[], int nor_size);
void place_player(Non_overlaping_rooms room, char **map, Character *character);
WINDOW *create_window (int height, int width, int startingX, int startingY);
void map_colors (WINDOW *main_window, int map_width, int j, int i, char traveled_path[][map_width]);
void display_map (WINDOW *main_window, Character *character, char **map, int map_width, char traveled_path[][map_width]);

#endif