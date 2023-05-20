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
  int index;
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
  mvwprintw(display_win, y, x, "Vida: %d", (int) character->life);
  y++;
  mvwprintw(display_win, y, x, "XP: %d", (int) character->xp);
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
        case WALL_CHAR:
        case WATER_CHAR:
        case FLOOR_CHAR:
        case FOOD_CHAR:
        case POTION_CHAR:  
          break;
        default:
          for (index = 0; index <  is_awake -> current_size; index ++)
          {
            if (j == is_awake -> enemies_awaken[index].y && i == is_awake -> enemies_awaken[index].x)
            {
              mvwprintw (display_win, y, x, "%s", is_awake -> enemies_awaken[index].name[0]);
              y ++;
              mvwprintw (display_win, y, x, "%d", (int) is_awake -> enemies_awaken[index].life);
              y ++;
              mvwprintw (display_win, y, x, "%d", (int) is_awake -> enemies_awaken[index].damage);
              y ++;
            }
          }
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
"You're on the water. Watch out for drownings!    ",
"D:                                                                  ",
"S:                                                                  ",
"G:                                                                  ",
"=: food                                                             ",
"@: potion                                                           ",
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

void print_instructions_win(WINDOW *instructions_win, Character *character, Consumables *available, Awake *is_awake, char traveled_path[][MAP_WIDTH], char *prev, int number_of_consumables)
{
  int x = 1, y = 1;
  int index1, index2;

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

      if (y == 10) {
        y = 1;
      }

      switch (traveled_path[j][i])
      {
      case WALL_CHAR:
      case WATER_CHAR:
      case FLOOR_CHAR:
        break;
      case FOOD_CHAR:
      case POTION_CHAR:
        for (index2 = 0; index2 < number_of_consumables; index2 ++)
        {
          if (j == available[index2].y && i == available[index2].x)
          {
            mvwprintw (instructions_win, y, x, "%s", available[index2].instruction[0]);
            y ++;
          }
        }
        break;      
      default:
        for (index1 = 0; index1 <  is_awake -> current_size; index1 ++)
        {
          if (j == is_awake -> enemies_awaken[index1].y && i == is_awake -> enemies_awaken[index1].x)
          {
            mvwprintw (instructions_win, y, x, "%s", is_awake -> enemies_awaken[index1].instruction[0]);
            y ++;
          }
        }
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
