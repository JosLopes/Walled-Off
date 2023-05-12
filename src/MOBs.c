#include "datatypes.h"
#include "defines.h"
#include "MOBs.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

/* 
  locate_positions locates eveery possible position for an enemy to spawn, 
  in a set number of rooms called non_overlaping_rooms (array '*no_overlpg').
  This set of rooms as described before has the property of, when built, not
  overlaping with any other room, aloying for a good candidate for placing
  enemies */
int locate_positions (int map_heigth, int map_width, char **map, int number_of_enemies, Enemy *enemies, int nor_size, Non_overlaping_rooms no_overlpg[])
{
  int enemies_ind = 0, rooms_ind = 1, max_per_room = 0;
  int offset, impossible_location = 0;  /* Max offset from the center of a room */

  if (enemies == NULL) {
    printf ("Not able to alloc memory.");
    return 1;
  }

  for (; rooms_ind < nor_size; rooms_ind ++)
  {
    offset = 4;
    impossible_location = 0;
    max_per_room = 0;
    while (enemies_ind < number_of_enemies && impossible_location == 0 && max_per_room < 3)
    {
      int possible_x = no_overlpg[rooms_ind].midX + (rand () % offset - rand () % offset);
      int possible_y = no_overlpg[rooms_ind].midY + (rand () % offset - rand () % offset);

      if (possible_x < map_width && possible_x > 0 && possible_y < map_heigth && possible_y > 0 && map[possible_y][possible_x] == FLOOR_CHAR)
      {
        enemies[enemies_ind].x = possible_x;
        enemies[enemies_ind].y = possible_y;
        max_per_room ++;
        enemies_ind ++;
        offset += 2;
      }
      else
      {
        impossible_location ++;
      }
    }
  }
  return enemies_ind;
}

/* 
  The function 'Variable_stats *d_enemies_variable_stats' initalizes every dumb enemy
  that is created manually by the programmer, its a special feature that allows
  to add new enemies to the game without much trouble, add the enemy to the 'd_variables'
  array and add +1 do D_ENMIES */

Variable_stats *d_enemies_variable_stats ()
{
  Variable_stats *d_variables = malloc (sizeof (Variable_stats) * D_ENEMIES);

  d_variables[0] = (Variable_stats){'A',"Albertino", 0, 0, 0};
  d_variables[1] = (Variable_stats){'J',"Josefino", 0, 0, 0};

  return d_variables; 
}

/* 
  The function 'Variable_stats *s_enemies_variable_stats' initalizes every smart enemy
  that is created manually by the programmer, its a special feature that allows
  to add new enemies to the game without much trouble: add the enemy to the 's_variables'
  array and add +1 do S_ENMIES */

Variable_stats *s_enemies_variable_stats ()
{
  Variable_stats *s_variables = malloc (sizeof (Variable_stats) * S_ENEMIES);

  s_variables[0] = (Variable_stats){'C',"Carambolas", 0, 0, 0};
  s_variables[1] = (Variable_stats){'M',"JMotherFucker", 0, 0, 0};

  return s_variables; 
}

/* 
  The function 'Variable_stats *g_enemies_variable_stats' initalizes every genius enemy
  that is created manually by the programmer, its a special feature that allows
  to add new enemies to the game without much trouble: add the enemy to the 'g_variables'
  array and add +1 do G_ENMIES */

Variable_stats *g_enemies_variable_stats ()
{
  Variable_stats *g_variables = malloc (sizeof (Variable_stats) * G_ENEMIES);

  g_variables[0] = (Variable_stats){'I',"Imtired", 0, 0, 0};
  g_variables[1] = (Variable_stats){'G',"Godblastmetodeath", 0, 0, 0 };

  return g_variables; 
}

/* 
  Initializes the tag to be used in the different enemies (depends only in the inteligence level)
  with pre-made stats that are common to that set type of enemy and can be changed in 'defines.h' */

void init_tag (Tag *tag, char intel, int max_xp, int min_xp, int poison, int group_desire)
{
  tag -> inteligence = intel; /* Initial for "tag" */
  tag -> xp_from_death = rand() % (max_xp - min_xp) + min_xp; /* Xp gained from killing this enemy type */
  tag -> poison_level = poison;  /* Gives information about how long does it last, the higher the */
                                 /* Number, the more steps the player needs to take to decipate   */
                                 /* The poison and stop taking damage                             */
  tag -> group_desire = group_desire;
}

/*
  Initializes the stats that are unique to each diferent enemy */

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
  enemy -> awake = 1; /* Enemy starts sleeping*/
}

/* 
  Places every enemy somewhere on the map, if the terrain is apropried, i.e. a FLOOR_CHAR */

void place_enemies (int number_of_enemies, Enemy *enemies, char **map)
{
  int current_enemy;

  for (current_enemy = 0; current_enemy < number_of_enemies; current_enemy ++)
  {
    map[enemies[current_enemy].y][enemies[current_enemy].x] = enemies[current_enemy].display;
  }
}

/*
  This is the "main function" of this module, it creates the tags that are attached to each
  enemy and combines both the tags and the unique characteristcs of an indivual enemy and places
  everyone on the map in the spots calculated before in the 'locate_positions' function */

Tag *init_enemies (int number_of_enemies, Enemy *enemies, Variable_stats *d_variables, Variable_stats *s_variables, Variable_stats *g_variables, char **map)
{
  Tag *tag = malloc (sizeof (Tag) * number_of_enemies);
  int current_enemy = 0;
  int dumb_enemies = number_of_enemies * 0.6;
  int dumb_plus_smart = dumb_enemies + (number_of_enemies * 0.3); 

  for (; current_enemy < dumb_enemies; current_enemy ++)
  {
    init_tag ((tag + current_enemy), D_CHAR, D_MAX_XP, D_MIN_XP, D_POISON, D_GROUP_DESIRE);
    init_enemy_stats ((enemies + current_enemy), (tag + current_enemy), (d_variables + (rand() % D_ENEMIES)));
  }
  free (d_variables);
  d_variables = NULL;

  for (; current_enemy < dumb_plus_smart; current_enemy ++)
  {
    init_tag ((tag + current_enemy), S_CHAR, S_MAX_XP, S_MIN_XP, S_POISON, S_GROUP_DESIRE);
    init_enemy_stats ((enemies + current_enemy), (tag + current_enemy), (s_variables + (rand() % S_ENEMIES)));
  }
  free (s_variables);
  s_variables = NULL;

  for (; current_enemy < number_of_enemies ; current_enemy ++)
  {
    init_tag ((tag + current_enemy), G_CHAR, G_MAX_XP, G_MIN_XP, G_POISON, G_GROUP_DESIRE);
    init_enemy_stats ((enemies + current_enemy), (tag + current_enemy), (g_variables + (rand() % G_ENEMIES)));
  }
  free (g_variables);
  g_variables = NULL;

  place_enemies (number_of_enemies, enemies, map);
  return tag;
}