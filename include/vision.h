#include <ncurses.h>
#include "datatypes.h"
#ifndef VISION_H
#define VISION_H

int sets_range(int life);
void vision (WINDOW *main_window, Character *character, int map_height, int map_width, char map[][map_width]);

#endif