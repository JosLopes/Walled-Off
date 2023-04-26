#include "datatypes.h"
#include "defines.h"
#include <stdio.h>

void init_tag (Tag *tag, intel, char life, int range, int max_xp, int min_xp; int scream_range, int poison, int group_desire)
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

void init_enemy (Enemy *enemy, Tag tag, char name[], symbol, damage)
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

void place_enemies(char map[][map_width], Non_overlaping_rooms no_overlpg[], int nor_size, Enemy *enemy) 
{
  srand(time(NULL)); /* initializes the seed for random numbers */

}


