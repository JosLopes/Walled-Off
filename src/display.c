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

/*char *personagem[]= {
  "  Personagem                                    ",
  " "
};
int n_personagem = sizeof(personagem)/ sizeof(char *);*/



/********************************************************
* Vertical window that gives information about the state 
* of the character and the enemies around him.
*********************************************************/
WINDOW* start_display(void) {
    WINDOW *display_win;

    int startx = MAP_WIDTH;
    int starty = 0;
    display_win = newwin(MAP_HEIGHT, 40, starty, startx);

    refresh();
    wrefresh(display_win);

    return display_win;
}

void print_displays(WINDOW *display_win, Character *character, Awake *is_awake, char traveled_path[][MAP_WIDTH])
{
  int x, y;
  x = 1;
  y = 1;

  int range = sets_range(character->life);
  int x_min = fmax(character->x - range, 0), x_max = fmin(character->x + range, MAP_WIDTH - 1);
  int y_min = fmax(character->y - range, 0), y_max = fmin(character->y + range, MAP_HEIGHT - 1);
  
  /* Draw a box around the display window*/
  box(display_win, 0, 0);

  /* Set the background color of the display window */
  wbkgd(display_win, COLOR_PAIR(BACKGROUND_COLOR));

  wattron(display_win, COLOR_PAIR(BACKGROUND_COLOR));

  mvwprintw(display_win, y, x, "   Personagem");
  y++;
  mvwprintw(display_win, y, x, "Vida: %f", character->life);
  y++;
  mvwprintw(display_win, y, x, "XP: %f", character->xp);
  y++;
  mvwprintw(display_win, y, x, "Arma: Calhau");
  y=y+5;
  mvwprintw(display_win, y, x, "   Inimigos");
  y++;
  
  /* range of vision of character */
  for (int i = x_min; i < x_max; i++) {
    for (int j = y_min; j < y_max; j++) {
      int dist = sqrt(pow(i - character ->x, 2) + pow(j - character->y, 2));
      
      if (dist<=range){
        switch (traveled_path[j][i])
        {
        case 'D':
        case 'S':
        case 'G': 
          /*walks through the array of awake enemies*/
          for(int u = 0; u < is_awake->current_size; u++)
          { /*if enemy is on range of vison and is awake -> print is parameters*/
            if (y == 28) {
              y = 10;
            }
            mvwprintw(display_win, y, x, "Nome: %s                      ", is_awake -> enemies_awaken[u].name[0]);
            y++;
            mvwprintw(display_win, y, x, "Vida: %d                      ", is_awake -> enemies_awaken[u].life);
            y++;
            mvwprintw(display_win, y, x, "Dano: %d                      ", is_awake -> enemies_awaken[u].damage);
            y+=2;
          }
          break;
        default:
          break;
        }
      }
    }
  }

  wattroff(display_win, COLOR_PAIR(BACKGROUND_COLOR));

  /* Update the display_win */
  wrefresh(display_win);
  refresh(); 
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
WINDOW* start_instructions (void)
{

  WINDOW *instructions_win;
  int startx = 0;
  int starty = MAP_HEIGHT;
  instructions_win = newwin(9, MAP_WIDTH, starty, startx);

  refresh ();
  wrefresh (instructions_win);

  return instructions_win;
}
void print_instructions_win(WINDOW *instructions_win, Character *character, char traveled_path[][MAP_WIDTH], char *prev)
{
  int x, y;
  x = 1;
  y = 1;

  int range = sets_range(character->life);
  int x_min = fmax(character->x - range, 0), x_max = fmin(character->x + range, MAP_WIDTH - 1);
  int y_min = fmax(character->y - range, 0), y_max = fmin(character->y + range, MAP_HEIGHT - 1);
  
  /* Draw a box around the display window*/
  box(instructions_win, 0, 0);

  /* Set the background color of the display window */
  wbkgd(instructions_win, COLOR_PAIR(BACKGROUND_COLOR));

  /*Print the instructions according the game*/
  wattron(instructions_win, COLOR_PAIR(BACKGROUND_COLOR));
  mvwprintw(instructions_win, y, x, "Hello and welcome to walled off adventure!");
  y++;

  /* range of vision of character */
  for (int i = x_min; i < x_max; i++) {
    for (int j = y_min; j < y_max; j++) {

      if (y == 8) {
        y = 1;
      }

      switch (traveled_path[j][i])
      {
      case 'D':
        mvwprintw(instructions_win, y, x, "%s", instrucoes[1]);
        y++;
        break;
      case 'S':
        mvwprintw(instructions_win, y, x, "%s", instrucoes[2]);
        y++;
        break;
      case 'G':
        mvwprintw(instructions_win, y, x, "%s", instrucoes[3]);
        y++;
        break;
      case 'F':
        mvwprintw(instructions_win, y, x, "%s", instrucoes[4]);
        y++;
        break;
      case 'P':
        mvwprintw(instructions_win, y, x, "%s", instrucoes[5]);
        y++;
        break;      
      default:
        break;
      }
    }
  }
  if (*prev == WATER_CHAR)
  {
    mvwprintw(instructions_win, y, x, "%s", instrucoes[0]);
    y++;
  }

  wattroff(instructions_win, COLOR_PAIR(BACKGROUND_COLOR));

  /* Update the instruction_win display */
  wrefresh(instructions_win);
  refresh(); 
}
