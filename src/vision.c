#include <stdio.h>
#include <stdlib.h>
#include <ncurses.h>
#include <math.h>
#include "datatypes.h"
#include "defines.h"
#include "movement.h"
#include "mapgen.h"
#include "MOBs.h"

/*
* Function that sets the range according to the character life
*/
int sets_range (int life){

  int range;

  if(life>70){
    range = 5;
  }
  else if(life>50 && life<70){
    range = 4;
  }
  else if(life>30 && life<50){
    range = 3;
  }
  else {
    range = 2;
  }
  return range;
}

/******************************************************************
*   Function that defines and print the range of vision around
* the character and the different colors that are displayed. 
*   Displays the vision of the character on the screen according
* some restrictions like step up above the enemies or eliminate
* the charactes that define the differents lands.
*   Save the path that the character have discovered already on 
* the array traveled_path so it can be displayed on the screen.
******************************************************************/
void vision_color (WINDOW *main_window, Character *character, char **map, int map_width ,char traveled_path[][map_width])
{
  int range = sets_range(character->life);

  /*defines x_min, x_max, y_min and y_max according to the range*/
  int x, y;

  for (x = 0; x < MAP_WIDTH; x++) {
    for (y = 0; y < MAP_HEIGHT; y++) {
  
    int dist = sqrt(pow(x - character ->x, 2) + pow(y - character->y, 2));
    int x_min = fmax(character->x - dist, 0), x_max = fmin(character->x + dist, MAP_WIDTH - 1);
    int y_min = fmax(character->y - dist, 0), y_max = fmin(character->y + dist, MAP_HEIGHT - 1);
  
      if (dist<=range && x >= x_min && y >= y_min && x <= x_max && y <= y_max)
      {
        /* add the viwed places to the list traveled_path to make they apper on the screen */
        traveled_path[y][x] = map[y][x];
      }

      /*case character position*/
      if (x == character->x && y == character->y)
      {
        wattron(main_window, COLOR_PAIR(PLAYER_VISION_COLOR1)); 
        mvwaddch(main_window, y, x, traveled_path[y][x]); 
        wattroff(main_window, COLOR_PAIR(PLAYER_VISION_COLOR1));
      }
      else if (traveled_path[y][x] == WALL_CHAR){
        map_colors(main_window, map_width, y, x, traveled_path);
      }
      else
      {
        /*sets the colors according to the distance to the character*/
        switch (dist)
        {
          case 5:
            wattron(main_window, COLOR_PAIR(PLAYER_VISION_COLOR4)); 
            mvwaddch(main_window, y, x, traveled_path[y][x]); 
            wattroff(main_window, COLOR_PAIR(PLAYER_VISION_COLOR4));
            break;
          case 4:
            wattron(main_window, COLOR_PAIR(PLAYER_VISION_COLOR3)); 
            mvwaddch(main_window, y, x, traveled_path[y][x]); 
            wattroff(main_window, COLOR_PAIR(PLAYER_VISION_COLOR3));
            break;
          case 3:
            wattron(main_window, COLOR_PAIR(PLAYER_VISION_COLOR3));  
            mvwaddch(main_window, y, x, traveled_path[y][x]); 
            wattroff(main_window, COLOR_PAIR(PLAYER_VISION_COLOR3));
            break;
          case 2:
            wattron(main_window, COLOR_PAIR(PLAYER_VISION_COLOR2)); 
            mvwaddch(main_window, y, x, traveled_path[y][x]); 
            wattroff(main_window, COLOR_PAIR(PLAYER_VISION_COLOR2));
            break;
          case 1:
            wattron(main_window, COLOR_PAIR(PLAYER_VISION_COLOR1)); 
            mvwaddch(main_window, y, x, traveled_path[y][x]); 
            wattroff(main_window, COLOR_PAIR(PLAYER_VISION_COLOR1));
            break;
          default:
            map_colors(main_window, map_width, y, x, traveled_path);
            break;
        }
      }
    }
  }
}