#include <ncurses.h>
#include "datatypes.h"
#include "defines.h"
#ifndef HEALTH_H
#define HEALTH_H
int locate_positions_consumables(int map_heigth, int map_width, char **map, Consumables consumables[], int number_of_enemies, int nor_size, Non_overlaping_rooms no_overlpg[]);
void place_foods_and_potions(int map_heigth, int map_width, char** map, Consumables consumables[], int number_of_enemies, int nor_size, Non_overlaping_rooms no_overlpg[]);
void food_and_potions (char **map, Character* character, Consumables consumables[], char *previous_char);


#endif