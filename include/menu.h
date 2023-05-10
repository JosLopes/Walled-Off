#include <ncurses.h>
#include "datatypes.h"

void print_menu(WINDOW *menu, int highlight);
WINDOW *create_menu (int height, int width, int starting_x, int starting_y);
