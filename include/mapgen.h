#include <ncurses.h>
#include "datatypes.h"
#ifndef MAP_GEN_H
#define MAP_GEN_H
#define MAP_HEIGHT 50
#define MAP_WIDTH 100
#define MAX_ROOMS 50
#define MIN_ROOM_HEIGHT 6
#define MAX_ROOM_HEIGHT 15
#define MIN_ROOM_WIDTH 6
#define MAX_ROOM_WIDTH 16
#define WALL_CHAR '#'
#define FLOOR_CHAR '.'
#define FIRE_CHAR ' '
#define PLAYER_CHAR '@'

void fillMap(int map_height, int map_width, char map[][map_width]);
int generateRooms(int map_height, int map_width, char map[][map_width], Room rooms[], int numRooms);
void generateCorridors(int map_width, char map[][map_width], Room rooms[], int numRooms, int number_of_non_overlaping_rooms);
void place_player(int map_height, int map_width, char map[][map_width]);
void place_enemies(int map_height, int map_width, char map[][MAP_WIDTH], int num_goblins, int num_skeletons, int num_orcs);
WINDOW *create_window (int height, int width, int startingX, int startingY);
void display_map (WINDOW *main_window, int map_height, int map_width, char map[][map_width]);

#endif