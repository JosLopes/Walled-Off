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


/* Function to locate the positions available to put the consumables in the Non overlaping rooms */
void locate_positions_consumables(int map_heigth, int map_width, char **map, Consumables *consumables, int number_of_enemies, int nor_size, Non_overlaping_rooms no_overlpg[]) {
  int consumables_ind = 0;
  int rooms_ind = 1;
  int max_per_room = 0;
  int offset;
  int impossible_location = 0;  /* Max offset from the center of a room */

  /* Use this number to determine the number of foods/potions in the map */
  int number_of_consumables = number_of_enemies / 3;

  for (; rooms_ind < nor_size; rooms_ind ++)
  {
    offset = 4;
    impossible_location = 0;
    max_per_room = 0;
    while (consumables_ind < number_of_consumables && impossible_location == 0 && max_per_room < 2)
    {
      int possible_x = no_overlpg[rooms_ind].midX + (rand () % offset - rand () % offset);
      int possible_y = no_overlpg[rooms_ind].midY + (rand () % offset - rand () % offset);

      if (possible_x < map_width && possible_x > 0 && possible_y < map_heigth && possible_y > 0 && map[possible_y][possible_x] == FLOOR_CHAR)
      {
        consumables[consumables_ind].x = possible_x;
        consumables[consumables_ind].y = possible_y;
        max_per_room ++;
        consumables_ind ++;
        offset += 2;

        if (consumables_ind % 2 == 0) {
          /* Add FOOD_CHAR */
          map[possible_y][possible_x] = FOOD_CHAR;
        } else {
          /* Add POTION_CHAR */
          map[possible_y][possible_x] = POTION_CHAR;
        }
      }
      else
      {
        impossible_location ++;
      }
    }
  }
}

// /* Function to locate the positions available to put the consumables out of the Non overlaping rooms */
// void locate_positions_consumables_outside(int map_heigth, int map_width, char **map, Consumables *consumables, int number_of_enemies, int nor_size, Non_overlaping_rooms no_overlpg[]) {
//   int consumables_ind = 0;
//   int rooms_ind = 1;
//   int max_per_room = 0;
//   int offset;
//   int impossible_location = 0;  /* Max offset from the center of a room */

//   /* Use this number to determine the number of foods/potions in the map */
//   int number_of_consumables = number_of_enemies / 3;

//   for (; rooms_ind < nor_size; rooms_ind ++)
//   {
//     offset = 4;
//     impossible_location = 0;
//     max_per_room = 0;
//     while (consumables_ind < number_of_consumables && impossible_location == 0 && max_per_room < 2)
//     {
//       int possible_x = no_overlpg[rooms_ind].midX + (rand () % offset - rand () % offset);
//       int possible_y = no_overlpg[rooms_ind].midY + (rand () % offset - rand () % offset);

//       if (possible_x < map_width && possible_x > 0 && possible_y < map_heigth && possible_y > 0 && map[possible_y][possible_x] == FLOOR_CHAR)
//       {
//         consumables[consumables_ind].x = possible_x;
//         consumables[consumables_ind].y = possible_y;
//         max_per_room ++;
//         consumables_ind ++;
//         offset += 2;

//         if (consumables_ind % 2 == 0) {
//           /* Add FOOD_CHAR */
//           map[possible_y][possible_x] = FOOD_CHAR;
//         } else {
//           /* Add POTION_CHAR */
//           map[possible_y][possible_x] = POTION_CHAR;
//         }
//       }
//       else
//       {
//         impossible_location ++;
//       }
//     }
//   }
// }

/*
* Function to spawn foods and potions in the map (WITHOUT ROOMS)
*/
void place_foods_and_potions(int map_heigth, int map_width, char** map, Consumables *consumables, int number_of_enemies, int nor_size, Non_overlaping_rooms no_overlpg[])
{
  /* Use this number to determine the number of foods/potions in the map */
  int number_of_consumables = number_of_enemies / 3;

  /* Put half of the foods in the map (Non overlaping rooms)*/
  locate_positions_consumables(map_heigth, map_width, map, consumables, number_of_enemies, nor_size, no_overlpg);

  /* Put the other half of the foods in the map (Out of the Non overlaping rooms) */
  for (int i = 0; i < number_of_consumables / 2; i++)
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

  int x_f = consumables->x;
  int y_f = consumables->y;

  /*in case the position of character is the same as a consumable*/
  if(map[y][x] == FOOD_CHAR || map[y][x] == POTION_CHAR){
    /*makes the impact on character life*/
    switch (consumables->identify)
      {
      case 'A':
        character->life = character->life + consumables[0].impact_life;
        break;
      case 'V':
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