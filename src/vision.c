#include <stdio.h>
#include <stdlib.h>
#include <ncurses.h>
#include <math.h>
#include "datatypes.h"
#include "defines.h"
#include "movement.h"
#include "mapgen.h"

/*
* Function that sets the range according to the character life
*/
int sets_range (int life){

  int range;

  if(life>50){
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
* Function that defines and print the range of vision around
* the character and the different colors that are displayed. 
* Displays the vision of the character on the screen according
* some restrictions like step up above the enemies or eliminate
* the charactes that define the differents lands.
******************************************************************/
void vision_color (WINDOW *main_window, Character *character, int map_height, int map_width, char map[][map_width],char traveled_path[][map_width], int range){
  /*defines x_min, x_max, y_min and y_max according to the range*/
  int x_min = fmax(character->x - range, 0), x_max = fmin(character->x + range, map_width - 1);
  int y_min = fmax(character->y - range+1, 0), y_max = fmin(character->y + range-1, map_height - 1);

  for (int x = x_min; x <= x_max; x++) {
    for (int y = y_min; y <= y_max; y++) {
      /* add the viwed places to the list traveled_path to make they apper on the screen */
      traveled_path[y][x] = map[y][x];
      /*case character position*/
      if (x == character->x && y == character->y){
        wattron(main_window, COLOR_PAIR(PLAYER_VISION_COLOR1)); 
        mvwaddch(main_window, y, x, traveled_path[y][x]); 
        wattroff(main_window, COLOR_PAIR(PLAYER_VISION_COLOR1));
      }
      else
      {
        /*sets the colors according to the distance to the character*/
        int dist = sqrt(pow(x - character ->x, 2) + pow(y - character->y, 2));
        switch (dist)
        {
          case 4:
            wattron(main_window, COLOR_PAIR(PLAYER_VISION_COLOR4)); 
            mvwaddch(main_window, y, x, traveled_path[y][x]); 
            wattroff(main_window, COLOR_PAIR(PLAYER_VISION_COLOR4));
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
            wattron(main_window,COLOR_PAIR(OBSCURE_COLOR)); 
            mvwaddch(main_window, y, x, traveled_path[y][x]); 
            wattroff(main_window,COLOR_PAIR(OBSCURE_COLOR));
            break;
        }
      }
      switch (traveled_path[y][x])
      {
        case ENEMY_G || ENEMY_S || ENEMY_O:
          wattron(main_window,COLOR_PAIR(ENEMY_COLOR)); 
          mvwaddch(main_window, y, x, traveled_path[y][x]); 
          wattroff(main_window,COLOR_PAIR(ENEMY_COLOR));
          break;
        case WALL_CHAR:
          wattron(main_window,COLOR_PAIR(WALL_COLOR)); 
          mvwaddch(main_window, y, x, traveled_path[y][x]); 
          wattroff(main_window,COLOR_PAIR(WALL_COLOR));
          break;
      }
    }
  }
}

/*
* Main function of the vision module.
* Save the path that the character have discovered already on the array
* traveled_path so it can be displayed on the screen.
*/
void vision (WINDOW *main_window, Character *character, int map_height, int map_width, char map[][map_width], char traveled_path[][map_width])
{
  int range = sets_range(character->life);
  int x_min = fmax(character->x - range, 0), x_max = fmin(character->x + range, map_width - 1);
  int y_min = fmax(character->y - range+1, 0), y_max = fmin(character->y + range-1, map_height - 1);

  /* set values within circle of radius range */
  for (int x = x_min; x <= x_max; x++) {
    for (int y = y_min; y <= y_max; y++) {

      /*case of range of vison of character*/
      if (pow(x - character->x, 2) + pow(y - character->y, 2) <= pow(range, 2)) {     
        vision_color(main_window, character, map_height, map_width, map, traveled_path, range);
      }
      else 
      {
        wattron(main_window,COLOR_PAIR(OBSCURE_COLOR)); 
        mvwaddch(main_window, y, x, traveled_path[y][x]); 
        wattroff(main_window,COLOR_PAIR(OBSCURE_COLOR));
      }
    }
  }
}