#include <stdio.h>
#include <stdlib.h>
#include <ncurses.h>
#include "datatypes.h"
#include "defines.h"
#include "movement.h"
#include "mapgen.h"

/*
HEALTH is going to be affected by:
-Attacks
-Potions

xP is going to be affected by:
-Potions
-
*/

/* Function to check if the character is within range */
bool isWithinRange(Enemy enemy, int charX, int charY, int range) {
  int distX = charX - enemy.x;
  int distY = charY - enemy.y;
  int distance = sqrt(distX * distX + distY * distY);
  return distance <= range;
}


void damage (Character *character,int map_height, int map_width, char map[][map_width], WINDOW *main_window){


  if (map [character -> y][character -> x] == FIRE_CHAR)
  {
    character -> life = character -> life - 20;
  }
}

/*void potion (Character character, WINDOW *main_window) {

}

void food (Character character, WINDOW *main_window) {

}*/
