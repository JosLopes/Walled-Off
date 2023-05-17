#include <stdio.h>
#include <stdlib.h>
#include <ncurses.h>
#include <math.h>
#include <time.h>
#include <string.h>
#include "datatypes.h"
#include "defines.h"
#include "movement.h"
#include "mapgen.h"
#include "consumables.h"
#include "MOBs.h"
#include "vision.h"

/*
* Function to spawn foods and potions in the map (WITHOUT ROOMS)
*/
void place_foods_and_potions(char** map, int number_of_enemies)
{
  /* Use this random number to determine the number of foods/potions in the map (it needs to be > 5) */
  int number_of_consumables = number_of_enemies / 3;

  /* Loop that works between 0 and number of consumables */
  for (int i = 0; i < number_of_consumables; i++)
  {
    int x;
    int y;

    /* Check if the randomly generated position is valid for placing a consumable */
    do
    {
      x = rand() % MAP_WIDTH;
      y = rand() % MAP_HEIGHT;
    } while (map[y][x] != FLOOR_CHAR);
  
    /* Determine the food/potion we are going to put on the map. Generate a random number between 0 and 4 */
    int consumable_index = rand() % NUM_CONSUMABLES;

    /* Check if the consumable is a food or a potion */
    if (consumable_index < 3)
    {
      map[y][x] = FOOD_CHAR;
    }
    else
    {
      map[y][x] = POTION_CHAR;
    }
  }
}

/*
* Function responsible for eating
*/
void food_and_potions(char **map, Character *character, Consumables *consumables, char *previous_char)
{
  int x = character->x;
  int y = character->y;

  /*in case the position of character is the same as a consumable*/
  if(map[y][x] == FOOD_CHAR || map[y][x] == POTION_CHAR){
    /*makes the impact on character life*/
    switch (consumables->identify)
      {
      case '=':
        character->life = character->life + consumables[0].impact_life;
        break;
      case 'p':
        character->life = character->life + consumables[1].impact_life;
        break;
      case 'C':
        character->life = character->life + consumables[2].impact_life;
        break;
      case 'E':
        character->life = character->life + consumables[3].impact_life;
        break;
      case 'B':
        character->life = character->life + consumables[4].impact_life;
        break;
      default:
        break;
    }
  }

  /*Delete the food and substitute it by a FLOOR_CHAR*/
  if(*previous_char == FOOD_CHAR || *previous_char == POTION_CHAR){
    *previous_char = FLOOR_CHAR;
  }
}