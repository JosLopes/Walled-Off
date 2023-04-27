#include <ncurses.h>
#include "datatypes.h"
#ifndef VISION_H
#define VISION_H

void vision (Character *character, int map_height, int map_width, char map[][map_width], WINDOW *main_window);

#endif