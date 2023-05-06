#include <ncurses.h>
#include "datatypes.h"
#ifndef HEALTH_H
#define HEALTH_H

void take_damage(char map[][MAP_WIDTH], Character* character, Enemy enemies[], int num_enemies);
void place_foods_and_potions (int map_heigth, int map_width, char map[][map_width], int nor_size, Room *rooms, Consumables *Foods[], Consumables *Potions[]);
void food_and_potions (char map[][MAP_WIDTH], Character* character, Consumables Foods[], Consumables Potions[]);


#endif