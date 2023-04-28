#include "datatypes.h"
#include "defines.h"
#include "MOBs.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int locate_positions (int map_heigth, int map_width, char map[][map_width], int enemies_size, Enemy *enemies, int nor_size, Non_overlaping_rooms no_overlpg[])
{
  srand(time(NULL)); /* initializes the seed for random numbers */
  int enemies_ind = 0, rooms_ind = 0;
  int offset, impossible_location = 0;  /* Max offset from the center of a room */

  if (enemies == NULL) {
    printf ("Not able to alloc memory.");
    return 1;
  }

  for (; rooms_ind < nor_size; rooms_ind ++)
  {
    offset = 1;
    while (enemies_ind < enemies_size && impossible_location == 0)
    {
      int possible_x = no_overlpg[rooms_ind].midX + (rand () % offset - rand () % offset);
      int possible_y = no_overlpg[rooms_ind].midY + (rand () % offset - rand () % offset);

      if (possible_x < map_width && possible_y < map_heigth && map[possible_y][possible_x] == FLOOR_CHAR)
      {
        enemies[enemies_ind].x = possible_x;
        enemies[enemies_ind].y = possible_y;
        enemies_ind ++;
        offset ++;
      }
      else
      {
        impossible_location ++;
      }
    }
  }
  return enemies_ind;
}

Variable_stats *d_enemies_variable_stats ()
{
  Variable_stats *d_variables = malloc (sizeof (Variable_stats) * D_ENEMIES);

  d_variables[0] = (Variable_stats){'A',"Albertino", 0, 0, 0};
  d_variables[1] = (Variable_stats){'J',"Josefino", 0, 0, 0};

  return d_variables; 
}

Variable_stats *s_enemies_variable_stats ()
{
  Variable_stats *s_variables = malloc (sizeof (Variable_stats) * S_ENEMIES);

  s_variables[0] = (Variable_stats){'C',"Carambolas", 0, 0, 0};
  s_variables[1] = (Variable_stats){'M',"JMotherFucker", 0, 0, 0};

  return s_variables; 
}

Variable_stats *g_enemies_variable_stats ()
{
  Variable_stats *g_variables = malloc (sizeof (Variable_stats) * G_ENEMIES);

  g_variables[0] = (Variable_stats){'I',"Imtired", 0, 0, 0};
  g_variables[1] = (Variable_stats){'G',"Godblastmetodeath", 0, 0, 0 };

  return g_variables; 
}

void init_tag (Tag *tag, char intel, int max_xp, int min_xp, int scream_range, int poison, int group_desire)
{
  tag -> inteligence = intel; /* Initial for "tag" */
  tag -> xp_from_death = rand() % (max_xp - min_xp) + min_xp; /* Xp gained from killing this enemy type */
  tag -> screaming_range = scream_range;
  tag -> poison_level = poison;  /* Gives information about how long does it last, the higher the */
                                 /* Number, the more steps the player needs to take to decipate   */
                                 /* The poison and stop taking damage                             */
  tag -> group_desire = group_desire;
}

void init_enemy_stats (Enemy *enemy, Tag *tag, Variable_stats *variables)
{
  int name_ind;
  enemy -> tag = tag;
  enemy -> display = variables -> display; /* Character to represent the enemy in the map */

  /* Copies the string "name" to the respective struct */
  for (name_ind = 0; name_ind < 9 && (enemy -> name[name_ind]) != '\0'; name_ind ++)
  {
    enemy -> name[name_ind] = variables -> name[name_ind];
  }
  enemy -> name[name_ind] = '\0';

  enemy -> life = variables -> life;
  enemy -> range = variables -> range;
  enemy -> damage = variables -> damage;
}

void init_enemies (int enemies_size, Enemy *enemies, Tag *tag, Variable_stats *d_variables, Variable_stats *s_variables, Variable_stats *g_variables)
{
  int current_enemy = 0;
  int dumb_enemies = enemies_size * 0.6;
  int dumb_plus_smart = dumb_enemies + (enemies_size * 0.3); 

  for (; current_enemy < dumb_enemies; current_enemy ++)
  {
    init_tag ((tag + current_enemy), D_CHAR, D_MAX_XP, D_MIN_XP, D_SCREAM_RANGE, D_POISON, D_GROUP_DESIRE);
    init_enemy_stats ((enemies + current_enemy), (tag + current_enemy), (d_variables + (rand() % D_ENEMIES)));
  }

  for (; current_enemy < dumb_plus_smart; current_enemy ++)
  {
    init_tag ((tag + current_enemy), S_CHAR, S_MAX_XP, S_MIN_XP, S_SCREAM_RANGE, S_POISON, S_GROUP_DESIRE);
    init_enemy_stats ((enemies + current_enemy), (tag + current_enemy), (d_variables + (rand() % D_ENEMIES)));
  }

  for (; current_enemy < enemies_size ; current_enemy ++)
  {
    init_tag ((tag + current_enemy), G_CHAR, G_MAX_XP, G_MIN_XP, G_SCREAM_RANGE, G_POISON, G_GROUP_DESIRE);
    init_enemy_stats ((enemies + current_enemy), (tag + current_enemy), (d_variables + (rand() % D_ENEMIES)));
  }
}