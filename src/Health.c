#include <stdio.h>
#include <stdlib.h>
#include <ncurses.h>
#include <math.h>
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
-Mobs until a certain %
*/

/* function that takes in a map, a character, and an array of enemies, and 
decrements the character's life if they are attacked by an enemy within range */
void take_damage(char map[][MAP_WIDTH], Character* character, Enemy enemies[], int num_enemies) 
{
  /*case character is crossing the fire*/
  if (map [character -> y][character -> x] == FIRE_CHAR)
  {
    character -> life = character -> life - 10;
  }

  /*cross the array of enemies checking if there's any attacking the character*/
  for (int i = 0; i < num_enemies; i++) 
  {
    Enemy* enemy = &enemies[i];
    int distance = sqrt(pow(character->x - enemy->x, 2) + pow(character->y - enemy->y, 2));
    
    /*case character is on the range of attack of enemy*/
    if (distance <= enemy->tag.range) 
    {
      /*Enemy is within range, so decrement character's life*/
      character->life -= enemy->damage;
      
      /*Check if character is dead*/
      if (character->life <= 0) 
      {
        exit(0);
      }
    }
  }
}