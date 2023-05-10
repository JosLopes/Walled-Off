#include <ncurses.h>
#include "datatypes.h"
#ifndef MENU_H
#define MENU_H

void print_menu(WINDOW *menu_window, int highlight);
WINDOW *create_menu (int height, int width, int starting_x, int starting_y);

#endif