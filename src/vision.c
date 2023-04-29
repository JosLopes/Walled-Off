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
void vision (Character *character, int map_height, int map_width, char map[][map_width])
{
  //int life = character->life;
  int range = sets_range(character->life);
  int x_min = fmax(character->x - range, 0), x_max = fmin(character->x + range, map_width - 1);
  int y_min = fmax(character->y - range+1, 0), y_max = fmin(character->y + range-1, map_height - 1);

  /* set values within circle of radius range */
  for (int x = x_min; x <= x_max; x++) {
    for (int y = y_min; y <= y_max; y++) {
      /*case character position*/
      if (x == character->x && y == character->y){

      }
      else if (pow(x - character->x, 2) + pow(y - character->y, 2) <= pow(range, 2)) {
        if (map[y][x] == ENEMY_G ||
            map[y][x] == ENEMY_S ||
            map[y][x] == ENEMY_O ||
            map[y][x] == WALL_CHAR){}
        else{
          map[y][x] = '+';
        }
      }
    }
  }
}