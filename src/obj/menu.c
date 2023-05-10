#include <panel.h>
#include <stdlib.h>
#include <time.h>
#include <ncurses.h>
#include <math.h>
#include "mapgen.h"
#include "datatypes.h"
#include "defines.h"
#include "movement.h"
#include "vision.h"
#include "MOBs.h"
#include "menu.h"

/* Create options of menu */
char *options[] = {
  "START",
  "HOW TO PLAY",
  "QUIT",
  NULL
};

/* Count the number of options */
int n_options = sizeof(options) / sizeof(char *);

/* 
* Print the menu options to the screen 
*/
void print_menu(WINDOW *menu_window, int highlight) {
  int x, y, i;
  
  x = 2;
  y = 2;

  /* Draw a box around the menu */
  box(menu_window, 0, 0);

  /* Print each option */
  for (i = 0; i < n_options; ++i) {
    if (highlight == i + 1) { /* Highlight the current choice */
      wattron(menu_window, COLOR_PAIR(SELECTED_OPTION_COLOR));
      mvwprintw(menu_window, y, x, "%s", options[i]);
      wattroff(menu_window, COLOR_PAIR(SELECTED_OPTION_COLOR));
    }
    else /* Don't highlight other choices */
      wattron(menu_window, COLOR_PAIR(BACKGROUND_COLOR));
      mvwprintw(menu_window, y, x, "%s", options[i]);
      wattroff(menu_window, COLOR_PAIR(BACKGROUND_COLOR));

    ++y;
  }
  /* Update the menu display */
  wrefresh(menu_window);
}

/* 
* Create the menu window 
*/
WINDOW *create_menu (int height, int width, int starting_x, int starting_y) {
  WINDOW *menu_window;

  int highlight = 1;
  int option = 0;
  int c;

  initscr();
  clear();
  noecho();
  cbreak(); /* Line buffering disabled. pass on everything */

  /* Calculate center of screen */
  starting_x = (80 - width) / 2;
  starting_y = (24 - height) / 2;

  /* Create menu window */
  menu_window = newwin (height, width, starting_y, starting_x);
  keypad(menu_window, TRUE);

  
  /* Initialize colors */
  if(has_colors() == TRUE) {
    start_color();

    init_color (1, 148, 0, 300); /*dark purple background*/
    init_color (2, 300, 0, 551); /*color of the selected option*/

    /*Define color pairs*/
    init_pair(BACKGROUND_COLOR, COLOR_WHITE, 1);
    init_pair(SELECTED_OPTION_COLOR, COLOR_WHITE, 2);
  }

  /* Print menu */
  print_menu(menu_window, highlight);

  /* Wait for user input */
  while (1) {
    c = wgetch(menu_window);
    switch(c) {
      case KEY_UP:  /* Move the highlight up */
        if (highlight == 1)
          highlight = n_options;
        else
          --highlight;
        break;
      case KEY_DOWN:  /* Move the highlight down */
        if (highlight == n_options)
          highlight = 1;
        else 
          ++highlight;
        break;
      case 10: /* Enter key */
        option = highlight;
        break;        
      default:
        /* Do nothing */
        refresh();
        break;
    }
    print_menu(menu_window, highlight); /* Update menu display */
    if (option != 0) /* User made a choice */
      break;
  }

  /* Clean up */
  clrtoeol();
  refresh ();
  wrefresh (menu_window);

  return menu_window;
}