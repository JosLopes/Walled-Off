#include <ncurses.h>
#include "datatypes.h"
#ifndef VISION_H
#define VISION_H

int sets_range(int life);
void vision_color(WINDOW *main_window, Character *character, char **map, int map_width, char traveled_path[][map_width]);

#endif