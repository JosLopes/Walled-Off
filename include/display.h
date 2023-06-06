#include <ncurses.h>
#include "datatypes.h"
#ifndef DISPLAY_H
#define DISPLAY_H

void print_displays(WINDOW *display_win, Character *character, Awake *is_awake, char traveled_path[][MAP_WIDTH]);
WINDOW* start_display (void);
void print_instructions_win(WINDOW *instructions_win, Character *character, Consumables *available, Awake *is_awake, char traveled_path[][MAP_WIDTH], char *prev, int number_of_consumables);
WINDOW* start_instructions (void);

#endif