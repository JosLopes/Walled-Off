#include <stdio.h>
#include <stdlib.h>
#include <ncurses.h>
#include <math.h>
#include "datatypes.h"
#include "defines.h"
#include "movement.h"
#include "mapgen.h"

/************************************************
 * NOTAS:
 * -> decrementar range conforme a perda de vida 
 * -> mudar cor do fundo
 * ->  
*************************************************/
void vision (Character *character, int map_height, int map_width, char map[][map_width], WINDOW *main_window)
{
  int range = PLAYER_VISION;
  int x_min = fmax(character->x - range, 0), x_max = fmin(character->x + range, map_width - 1);
  int y_min = fmax(character->y - range, 0), y_max = fmin(character->y + range, map_height - 1);

  /* Redraw map */
  for (int i = 0; i < map_width; i++) {
    for (int j = 0; j < map_height; j++) {
      
      /*character and vision*/
      if(i == x_min && j == y_min)
      {
        /* set values within circle of radius range */
        for (int x = x_min; x <= x_max; x++) 
        {
          for (int y = y_min; y <= y_max; y++) 
          {
            attron(COLOR_PAIR(WATER_PAIR)); // Turn on color pair 2
            mvwaddch(main_window, y, x, map[y][x]); /*print the character at the given position*/
            attroff(COLOR_PAIR(WATER_PAIR)); // Turn off color pair 2
          }
        }
      }
      attron(COLOR_PAIR(PLAYER_VISION_COLOR)); // Turn on color pair 2
      mvwaddch(main_window, j, i, map[j][i]); // print the character at the given position
      attroff(COLOR_PAIR(PLAYER_VISION_COLOR)); // Turn off color pair 1
    }
  }
    wrefresh(main_window); /*refresh the window to display changes*/
}