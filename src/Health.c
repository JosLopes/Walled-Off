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

int range (Character *character,int map_height, int map_width, char map[][map_width], WINDOW *main_window) {
  int x = character->x, y = character->y;
  for (y; y < range; y++){
    for (x; x < range; x++){
      if (map[y][x] == ENEMY_G || map[y][x] == ENEMY_O || map[y][x] == ENEMY_S) {
        return 1; /*true -> should receive damage*/
      }
    }
  }
  return 0; /*false -> free path*/
}

void detect_enemy (Character *character,int map_height, int map_width, char map[][map_width], WINDOW *main_window){
  int x = character->x, y = character->y;
  for (y; y < range; y++){
    for (x; x < range; x++){
      if (map[y][x] == ENEMY_G || map[y][x] == ENEMY_O || map[y][x] == ENEMY_S) {
        return 1; /*true -> should receive damage*/
      }
    }
  }

}

void damage (Character *character,int map_height, int map_width, char map[][map_width], WINDOW *main_window){


  if (map [character -> y][character -> x] == FIRE_CHAR)
  {
    character -> life = character -> life - 10;
  }
  else if (range (character, map_height, map_width, map[][map_width], WINDOW *main_window ))
  {
    
  }
}

/*void potion (Character character, WINDOW *main_window) {

}

void food (Character character, WINDOW *main_window) {

}*/
