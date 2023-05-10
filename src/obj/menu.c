#include <stdio.h>
#include <stdlib.h>
#include <ncurses.h>
#include "datatypes.h"
#include "defines.h"
#include "movement.h"
#include "mapgen.h"
#include "vision.h"
#include "menu.h"

/*definir para tirar som (ex)*/
/* Create options of menu */
char *options[4] = {
  "START",
  "HOW TO PLAY",
  "QUIT",
  NULL
};

/* Count the number of options */
int n_options = sizeof(options) / sizeof(char *);

/* Print the menu options to the screen */
void print_menu(WINDOW *menu, int highlight) {
  int x, y, i;
  
  x = 2;
  y = 2;

  /* Draw a box around the menu */
  box(menu, 0, 0);

  /* Print each option */
  for (i = 0; i < n_options; ++i) {
    if (highlight == i + 1) { /* Highlight the current choice */
      wattron(menu, A_REVERSE);
      mvwprintw(menu, y, x, "%s", options[i]);
      wattroff(menu, A_REVERSE);
    }
    else /* Don't highlight other choices */
      mvwprintw(menu, y, x, "%s", options[i]);
    ++y;
  }
  /* Update the menu display */
  wrefresh(menu);
}

/* Create the menu window */
WINDOW *create_menu (int height, int width, int starting_x, int starting_y) {
  WINDOW *menu;

  /* Create menu window */
  menu = newwin (height, width, starting_y, starting_x);
  keypad(menu, TRUE);  /* Enable keyboard input for the menu */
  
  /* Initialize colors */
  if(has_colors() == TRUE) {
    start_color();

    init_color (1, 148, 0, 300); /*dark purple background*/
    init_color (2, 300, 0, 551); /*color of the selected option*/

    /*Define color pairs*/
    init_pair(BACKGROUND_COLOR, COLOR_WHITE, 1);
    init_pair(SELECTED_OPTION_COLOR, COLOR_WHITE, 2);
  }

  /* Calculate center of screen */
  int start_x = (80 - width) / 2;
  int start_y = (24 - height) / 2;

  /*selecting option*/
  int option = 0;
  int highlight = 1;
  int c;


  /* Print menu */
  print_menu(menu_win, highlight);
  wborder (menu, '|', '|', '-', '-', '*', '*', '*', '*');

  /* Wait for user input */
  while (1) {
    c = wgetch(menu_win);
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
    print_menu(menu_win, highlight); /* Update menu display */
    if (option != 0) /* User made a choice */
      break;
  }

  /* Clean up */
  refresh ();
  wrefresh (menu);

  return menu;
}