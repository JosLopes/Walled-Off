#include "menu.h"
#include "mapgen.h"
#include "datatypes.h"
#include "defines.h"
#include "movement.h"
#include "vision.h"
#include "MOBs.h"
#include "path_finder.h"
#include "artificial_inteligence.h"
#include "combat.h"
#include <stdio.h>
#include <stdlib.h>
#include <ncurses.h>

/*
* Function that prints the displays
*/
/*
char *personagem[]= {
  "  Personagem                                    ",
  " "
};
int n_personagem = sizeof(personagem)/ sizeof(char *);*/



void print_displays(WINDOW *display_win, Character *character, Enemy *enemies)
{
  int x, y;

  x = MAP_WIDTH + 4;
  y = 2;

  /* Draw a box around the display window*/
  box(display_win, 0, 0);

  /* Set the background color of the display window */
  wbkgd(display_win, COLOR_PAIR(BACKGROUND_COLOR));

  /**/
  wattron(display_win, COLOR_PAIR(BACKGROUND_COLOR));

  mvwprintw(display_win, y, x, "   Personagem"); 
  y++;
  mvwprintw(display_win, y, x, "Vida: %f", character->life);
  //mvprintw(y, x, "Vida: %f", character->life);
  y++;
  //mvprintw(display_win, y, x, "XP: %d", character->xp);
  y++;
  //mvprintw(display_win, y, x, "Arma: %s", character->weapons[character->current_weapon_index]);
  y=y+5;
  mvwprintw(display_win, y, x, "   Inimigos");
  y++;
  mvprintw(y, x, "Nome: %s", enemies->name);
  y++;
  //mvprintw(display_win, y, x, "Vida: %d", enemies->life);
  y++;
  //mvprintw(display_win, y, x, "Dano: %d", enemies->damage);

  wattroff(display_win, COLOR_PAIR(BACKGROUND_COLOR));

  /* Update the display_win */
  wrefresh(display_win);
  refresh(); 
}

WINDOW* start_display(Character *character, Enemy *enemies) {
    WINDOW *display_win;

    int startx = MAP_WIDTH;
    int starty = 0;
    display_win = newwin(MAP_HEIGHT, 40, starty, startx);

    /* Print display */
    print_displays(display_win, character, enemies);

    refresh();
    wrefresh(display_win);

    return display_win;
}











void print_instructions_win(WINDOW *instructions_win, Character *character, Enemy *enemies)
{
  int x, y;

  x = 2;
  y = MAP_HEIGHT+1;

  /* Draw a box around the display window*/
  box(instructions_win, 0, 0);

  /* Set the background color of the display window */
  wbkgd(instructions_win, COLOR_PAIR(BACKGROUND_COLOR));
  mvwprintw(instructions_win, y, x, "YOU KNOW");
  y++;
  mvprintw(y, x, "Vida: %f", character->life);
  y++;
  mvprintw(y, x, "Nome: %s", enemies->name);


  /**/
  wattron(instructions_win, COLOR_PAIR(BACKGROUND_COLOR));

  wattroff(instructions_win, COLOR_PAIR(BACKGROUND_COLOR));

  /* Update the instruction_win display */
  wrefresh(instructions_win);
  refresh(); 
}

WINDOW* start_instructions (Character *character, Enemy *enemies){

  WINDOW *instructions_win;
  int startx = 0;
  int starty = MAP_HEIGHT;
  instructions_win = newwin(9, MAP_WIDTH, starty, startx);

  /* Print display */
  print_instructions_win(instructions_win, character, enemies);

  refresh ();
  wrefresh (instructions_win);

  return instructions_win;
}