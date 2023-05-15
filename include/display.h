#include <ncurses.h>
#include "datatypes.h"
#ifndef MENU_H
#define MENU_H

void print_displays(WINDOW *display_win, Character *character, Enemy *enemies);
WINDOW* start_display (Character *character, Enemy *enemies);
void print_instructions_win(WINDOW *display_win, Character *character, Enemy *enemies);
WINDOW* start_instructions (Character *character, Enemy *enemies);

#endif