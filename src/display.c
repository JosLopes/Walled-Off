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
#include <math.h>

/*
* Function that prints the displays
*/

char *personagem[]= {
  "  Personagem                                    ",
  " "
};
int n_personagem = sizeof(personagem)/ sizeof(char *);



/********************************************************
* Vertical window that gives information about the state 
* of the character and the enemies around him.
*********************************************************/

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
  mvwprintw(display_win, y, x, "%s", personagem[0]); 

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



char *instrucoes[10] = {
"You're on the water. Watch out for drownings!                       ",
"D:                                                                  ",
"S:                                                                  ",
"G:                                                                  ",
"F: food                                                             ",
"P: potion                                                           ",
"                                                                    ",
"                                                                    ",
"                                                                    ",
"                                                                    "
};

/**************************************************************
* Horizontal window that gives information/instructions during 
* the course of the game.
***************************************************************/
/**************************************************************
* Function that sets the instruction according to visible 
* things for character
***************************************************************/
void print_instructions_win(WINDOW *instructions_win, Character *character, Enemy *enemies, char traveled_path[][MAP_WIDTH])
{
  int x, y;
  x = 2;
  y = MAP_HEIGHT + 1;

  int indice = 0;
  int range = sets_range(character->life);
  int x_min = fmax(character->x - range, 0), x_max = fmin(character->x + range, MAP_WIDTH - 1);
  int y_min = fmax(character->y - range, 0), y_max = fmin(character->y + range, MAP_HEIGHT - 1);
  
  /* Draw a box around the display window*/
  box(instructions_win, 0, 0);

  /* Set the background color of the display window */
  wbkgd(instructions_win, COLOR_PAIR(BACKGROUND_COLOR));

  /*Print the instructions according the game*/
  wattron(instructions_win, COLOR_PAIR(BACKGROUND_COLOR));
  wprintw(y, x, "Hello and welcome to walled off adventure!");
  y++;

  /* range of vision of character */
  for (int i = x_min; i < x_max; i++) {
    for (int j = y_min; j < y_max; j++) {

      if (y == MAP_HEIGHT + 11) {
        y = MAP_HEIGHT + 1;
      }

      switch (traveled_path[j][i])
      {
      case WATER_CHAR:
        wprintw(y, x, "%s", instrucoes[0]);
        y++;
        break;
      case 'D':
        wprintw(y, x, "%s", instrucoes[1]);
        y++;
        break;
      case 'S':
        wprintw(y, x, "%s", instrucoes[2]);
        y++;
        break;
      case 'G':
        wprintw(y, x, "%s", instrucoes[3]);
        y++;
        break;
      case 'F':
        wprintw(y, x, "%s", instrucoes[4]);
        y++;
        break;
      case 'P':
        wprintw(y, x, "%s", instrucoes[5]);
        y++;
        break;      
      default:
        break;
      }
    }
  }

  wattroff(instructions_win, COLOR_PAIR(BACKGROUND_COLOR));

  /* Update the instruction_win display */
  wrefresh(instructions_win);
  refresh(); 
}

WINDOW* start_instructions (Character *character, Enemy *enemies, char traveled_path[][MAP_WIDTH])
{

  WINDOW *instructions_win;
  int startx = 0;
  int starty = MAP_HEIGHT;
  instructions_win = newwin(9, MAP_WIDTH, starty, startx);

  /* Print display */
  print_instructions_win(instructions_win, character, enemies, traveled_path);

  refresh ();
  wrefresh (instructions_win);

  return instructions_win;
}