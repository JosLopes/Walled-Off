#include <ncurses.h>
#include "datatypes.h"
#include "defines.h"
#ifndef HEALTH_H
#define HEALTH_H

void place_foods_and_potions (char **map, int number_of_conumables, Consumables *consumables, Consumables *available);
void food_and_potions (Character *character, Consumables *available, char *previous_char, int number_of_consumables);
void water_damage(char** map, Character *character, char *previous_char, int *count_water);

#endif