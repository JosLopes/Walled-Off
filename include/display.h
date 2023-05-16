#include <ncurses.h>
#include "datatypes.h"
#ifndef DISPLAY_H
#define DISPLAY_H

void print_displays(WINDOW *display_win, Character *character, Enemy *enemies);
WINDOW* start_display (Character *character, Enemy *enemies);
void print_instructions_win(WINDOW *display_win, Character *character, Enemy *enemies, char traveled_path[][MAP_WIDTH]);
WINDOW* start_instructions (Character *character, Enemy *enemies, char traveled_path[][MAP_WIDTH]);

#endif