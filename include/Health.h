#include <ncurses.h>
#include "datatypes.h"
#ifndef HEALTH_H
#define HEALTH_H

void takeDamage(int map_height, int map_width, char map[][map_width], Character *character, int enemies[][2], int num_enemies);
void place_foods_and_potions(int map_height, int map_width, char map[][map_width], int numRooms, Room rooms[], int foods[][2], int potions[][2]);
void foods_and_potions(int map_height, int map_width, char map[][map_width], Character *character, int foods[][2], int potions[][2], int num_foods, int num_potions);


#endif