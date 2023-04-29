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
 * -> se não conseguir com as cores posso tentar
 * meter a dar print do mapa só por onde o 
 * personagem passar
 * -> mudar cor do fundo
*************************************************/
/*sets the range according to the character life*/
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

void vision_color (WINDOW *main_window, Character *character, int map_height, int map_width, char map[][map_width], int range){
  int x_min = fmax(character->x - range, 0), x_max = fmin(character->x + range, map_width - 1);
  int y_min = fmax(character->y - range+1, 0), y_max = fmin(character->y + range-1, map_height - 1);


for (int x = x_min; x <= x_max; x++) {
    for (int y = y_min; y <= y_max; y++) {
    
      int dist = sqrt(pow(x - character ->x, 2) + pow(y - character->y, 2));
      if(dist==4){
        wattron(main_window, COLOR_PAIR(PLAYER_VISION_COLOR4)); 
        mvwaddch(main_window, y, x, map[y][x]); 
        wattroff(main_window, COLOR_PAIR(PLAYER_VISION_COLOR4));
      }
      else if(dist==3){
        wattron(main_window, COLOR_PAIR(PLAYER_VISION_COLOR3));  
        mvwaddch(main_window, y, x, map[y][x]); 
        wattroff(main_window, COLOR_PAIR(PLAYER_VISION_COLOR3));
      }
      else if(dist==2){
        wattron(main_window, COLOR_PAIR(PLAYER_VISION_COLOR2)); 
        mvwaddch(main_window, y, x, map[y][x]); 
        wattroff(main_window, COLOR_PAIR(PLAYER_VISION_COLOR2));
      }
      else if(dist==1){
        wattron(main_window, COLOR_PAIR(PLAYER_VISION_COLOR1)); 
        mvwaddch(main_window, y, x, map[y][x]); 
        wattroff(main_window, COLOR_PAIR(PLAYER_VISION_COLOR1));

      }
    }
  }
}
void vision (WINDOW *main_window, Character *character, int map_height, int map_width, char map[][map_width])
{
  int range = sets_range(character->life);
  int x_min = fmax(character->x - range, 0), x_max = fmin(character->x + range, map_width - 1);
  int y_min = fmax(character->y - range+1, 0), y_max = fmin(character->y + range-1, map_height - 1);

  /* set values within circle of radius range */
  for (int x = x_min; x <= x_max; x++) {
    for (int y = y_min; y <= y_max; y++) {
      /*case character position*/
      if (x == character->x && y == character->y){
        wattron(main_window, COLOR_PAIR(PLAYER_VISION_COLOR1)); 
        mvwaddch(main_window, y, x, map[y][x]); 
        wattroff(main_window, COLOR_PAIR(PLAYER_VISION_COLOR1));
      }
      else if (pow(x - character->x, 2) + pow(y - character->y, 2) <= pow(range, 2)) {
        if (map[y][x] == ENEMY_G || map[y][x] == ENEMY_S || map[y][x] == ENEMY_O ){
          wattron(main_window,COLOR_PAIR(ENEMY_COLOR)); 
          mvwaddch(main_window, y, x, map[y][x]); 
          wattroff(main_window,COLOR_PAIR(ENEMY_COLOR));
            }
        else if (map[y][x] == WALL_CHAR){
          wattron(main_window,COLOR_PAIR(WALL_COLOR)); 
          mvwaddch(main_window, y, x, map[y][x]); 
          wattroff(main_window,COLOR_PAIR(WALL_COLOR));
            }
        else{
          vision_color(main_window, character, map_height, map_width, map, range);
        }
      }
    }
  }
}