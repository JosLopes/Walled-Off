#include <ncurses.h>
#include "datatypes.h"
#include "defines.h"
#ifndef HEALTH_H
#define HEALTH_H

void place_foods_and_potions (char **map, int number_of_conumables, Consumables *consumables, Consumables *available);
void food_and_potions (char **map, Character *character, Consumables *consumables, char *previous_char);


#endif