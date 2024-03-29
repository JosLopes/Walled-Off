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
* a104086 - Daniel Silva
* a104188 - Ana Cerqueira
* Function to spawn foods and potions in the map (WITHOUT ROOMS)
* Generates three random numbers, one for an index between 0 and
* the number of consumables and another two to determine the 
* position of the consumable on the map. It also keeps the consumables 
* generated in the array available.
*/
void place_foods_and_potions(char** map, int number_of_consumables, Consumables *consumables, Consumables *available)
{
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
      available[i] = consumables[consumable_index];
      available[i].x = x;
      available[i].y = y;
    }
    else
    {
      map[y][x] = POTION_CHAR;
      available[i] = consumables[consumable_index];
      available[i].x = x;
      available[i].y = y;
    }
  }
}

/*
* a104086 - Daniel Silva
* a104188 - Ana Cerqueira
* Function responsible for eating.
* This functons delete the chars on the map the represent
* the food and potion and adds the impact that is defined
* for each consumable.
*/
void food_and_potions (Character *character, Consumables *available, char *previous_char, int number_of_consumables)
{

  if (*previous_char == FOOD_CHAR || *previous_char == POTION_CHAR)
  {
  for (int i = 0; i < number_of_consumables; i++)
  {
    /*makes the impact on character life*/
    if (available[i].y == character -> y && available[i].x == character -> x)
    {
        if (character->life + available[i].impact_life <= character -> initial_life)
        {
          character->life += available[i].impact_life;
          character->xp += available[i].impact_xp;
        } else if (character->life + available[i].impact_life >= character -> initial_life) {
          character->life = character -> initial_life;
          character->xp += available[i].impact_xp;
        }

        i = number_of_consumables;
    }
  }
  }

  /*Delete the food and substitute it by a FLOOR_CHAR*/
  if (*previous_char == FOOD_CHAR || *previous_char == POTION_CHAR)
  {
    *previous_char = FLOOR_CHAR;
  }
}

/*
* a104086 - Daniel Silva
* Function responsible for taking life when the player is on the water.
* This function verifies if the player has been on the water for 4 plays and, if so, takes away 5 points of life
*/
void water_damage(Character *character, char *previous_char, int *count_water)
{
  if (*previous_char == WATER_CHAR) {
    (*count_water) ++;

    if ((*count_water) == 4 && character->life >= 5) {
      character->life -= 5;
      (*count_water) = 0;
    }
  }
}