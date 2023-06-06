#include <ncurses.h>
#include "datatypes.h"
#ifndef COMBAT_H
#define COMBAT_h


void attack(Character *character, WINDOW *instructions_win, Enemy *enemy, Awake *is_awake, char **map, int ch);


#endif
