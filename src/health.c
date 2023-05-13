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
      x = rand() % 25;
      y = rand() % 25;
    } while (map[y][x] != FLOOR_CHAR);
  
    /* Determine the food/potion we are going to put on the map. Generate a random number between 0 and 4 */
    int consumable_index = rand() % NUM_CONSUMABLES;

    /* Check if the consumable is a food or a potion */
    switch (consumable_index)
    {
    case 0:
      map[y][x] = APPLE_CHAR;
      break;
    case 1:
      map[y][x] = AVOCADO_CHAR;
      break;
    case 2:
      map[y][x] = CHICKEN_CHAR;
      break;
    case 3:
      map[y][x] = ELIXIR_CHAR;
      break;
    case 4:
      map[y][x] = BREW_CHAR;
      break;
    default:
      break;
    }
  }
}

/*
* Function responsible for eating
*/
void food_and_potions(char **map, Character *character, Consumables consumables[])
{
  /*if food replace for floor char*/
  if(map[character->y][character->x] == APPLE_CHAR || map[character->y][character->x] == AVOCADO_CHAR || map[character->y][character->x] == CHICKEN_CHAR || map[character->y][character->x] == ELIXIR_CHAR || map[character->y][character->x] == BREW_CHAR){
    switch (map[character->y][character->x])
    {
    case APPLE_CHAR:
      /*Increase/Remove health*/
      character->life = character->life + consumables[0].impact_life;
      /*Delete the food and substitute it by a point*/
      map[character->y][character->x] = FLOOR_CHAR;
      break;
    case AVOCADO_CHAR:
      character->life = character->life + consumables[1].impact_life;
      /*Delete the food and substitute it by a point*/
      map[character->y][character->x] = FLOOR_CHAR;
      break;
    case CHICKEN_CHAR:
      /*Increase/Remove health*/
      character->life = character->life + consumables[2].impact_life;
      /*Delete the food and substitute it by a point*/
      map[character->y][character->x] = FLOOR_CHAR;
      break;
    case ELIXIR_CHAR:
      /*Increase/Remove health*/
      character->life = character->life + consumables[3].impact_life;
      /*Delete the food and substitute it by a point*/
      map[character->y][character->x] = FLOOR_CHAR;
      break;
    case BREW_CHAR:
      /*Increase/Remove health*/
      character->life = character->life + consumables[4].impact_life;
      /*Delete the food and substitute it by a point*/
      map[character->y][character->x] = FLOOR_CHAR;
      break;
    default:
      break;
    }
  }
}