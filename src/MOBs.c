#include "datatypes.h"
#include "defines.h"
#include <stdio.h>
#include <stdio.h>

int locate_positions (int map_heigth, int map_width, char map[][map_width], int enemys_size, Enemy enemys[], int nor_size, Non_overlaping_rooms no_overlpg[])
{
  srand(time(NULL)); /* initializes the seed for random numbers */
  int enemys_ind = 0, rooms_ind;
  int offset, impossible_location = 0;  /* Max offset from the center of a room */

  if (enemys == NULL) {
    printf ("Not able to alloc memory.");
    return 1;
  }

  for (; rooms_ind < nor_size; rooms_ind ++)
  {
    offset = 1;
    while (enemys_ind < enemys_size && impossible_location = 0)
    {
      int possible_x = no_overlpg[rooms_ind].midX + (rand () % offset - rand () % offset);
      int possible_y = no_overlpg[rooms_ind].midY + (rand () % offset - rand () % offset);

      if (possible_x < map_width && possible_y < map_heigth && map[possible_y][possible_x] == FLOOR_CHAR)
      {
        enemys[enemys_ind] -> x = possible_x;
        enemys[enemys_ind] -> y = possible_y;
        enemys_ind ++;
        offset ++;
      }
      else
      {
        impossible_location ++;
      }
    }
  }
  return enemys_ind;
}

void init_tag (Tag *tag, char intel, char life, int range, int max_xp, int min_xp, int scream_range, int poison, int group_desire)
{
  tag -> inteligence = intel /* Initial for "tag" */
  tag -> life = life; /* Enemies life */
  tag -> range = range; /* Range for distance attacks */
  tag -> xp_from_death = (rand() % max_xp-min_xp) + min_xp; /* Xp gained from killing this enemy type */
  tag -> screaming_range = scream_range;
  tag -> poison_level = poison;  /* Gives information about how long does it last, the higher the */
                                 /* Number, the more steps the player needs to take to decipate   */
                                 /* The poison and stop taking damage                             */
}

void init_enemy (Enemy *enemy, Tag tag, char name[], char symbol, damage)
{
  enemy -> tag = tag;

  /* Copies the string "name" to the respective struct */
  for (int name_ind = 0; name_ind < 9 && enemy -> name[name_ind] != "\0"; name_ind ++)
  {
    enemy -> name[name_ind] = name[name_ind];
  }
  enemy -> name[name_ind] = "\0";

  enemy -> display_symbol = symbol; /* Character to represent the enemy in the map */
  enemy -> damage = damage;
}