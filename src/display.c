#include "defines.h"
#include <stdio.h>
#include <stdlib.h>
#include <ncurses.h>


/*
* Function that prints the displays
*/
void print_displays(WINDOW *display_win)
{
  int x, y, i;

  x = (MAP_WIDTH-12)/2;
  y = MAP_HEIGHT/2 ;

  /* Set the background color of the display window */
  wbkgd(display_win, COLOR_PAIR(BACKGROUND_COLOR));

  /* Draw a box around the display window*/
  box(display_win, 0, 0);

  /* Print titulo*/
  int x1 = (MAP_WIDTH - 65)/2;
  int y1 = 2;
  wattron(display_win, COLOR_PAIR(TITLE_COLOR));

  for (i = 0; i < n_titulo; ++i)
  {
    /* Highlight the current choice */
    mvwprintw(menu_win, y1, x1, "%s", titulo[i]);
    y1++;
  }
  wattroff(menu_win, COLOR_PAIR(TITLE_COLOR));

  /* Print each option */
  for (i = 0; i < n_choices; ++i)
  {
    if (highlight == i + 1)
    { /* Highlight the current choice */
      wattron(menu_win, COLOR_PAIR(SELECTED_OPTION_COLOR));
      mvwprintw(menu_win, y, x, "%s", choices[i]);
      wattroff(menu_win, COLOR_PAIR(SELECTED_OPTION_COLOR));
    }
    else
    { /* Don't highlight other choices */
      wattron(display_win, COLOR_PAIR(BACKGROUND_COLOR));
      mvwprintw(display_win, y, x, "%s", choices[i]);
      wattroff(display_win, COLOR_PAIR(BACKGROUND_COLOR));
    }
    ++y;
  }
  /* Update the menu display */
  wrefresh(display_win);
}
int start_displays (void)
{
  WINDOW *display_win;
  int startx = 0;
  int starty = 0;
  
  /* Create display window */
  display_win = newwin(MAP_HEIGHT, MAP_WIDTH, starty, startx);
  keypad(display_win, TRUE);

  /* Print display */
  print_displays(display_win);


}