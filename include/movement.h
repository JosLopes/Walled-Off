#include <ncurses.h>
#include "datatypes.h"
#ifndef MOVEMENT_H
#define MOVEMENT_H

void movement (Character *character, int map_width, char map[][map_width], WINDOW *main_window, int ch);

#endif