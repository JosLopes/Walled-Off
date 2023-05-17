#include <ncurses.h>
#include "datatypes.h"
#ifndef DISPLAY_H
#define DISPLAY_H

void print_displays(WINDOW *display_win, Character *character, Awake *is_awake, char traveled_path[][MAP_WIDTH]);
WINDOW* start_display (Character *character, Awake *is_awake);
void print_instructions_win(WINDOW *display_win, Character *character, char traveled_path[][MAP_WIDTH], char *prev);
WINDOW* start_instructions (Character *character, char traveled_path[][MAP_WIDTH]);

#endif