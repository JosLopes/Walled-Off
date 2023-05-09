#include "defines.h"
#include "datatypes.h"
#include "MOBsAI.h"
#include <stdlib.h>

void init_is_awake (int number_of_enemies, Awake *is_awake)
{
  is_awake -> enemies_awaken = malloc (sizeof (Enemy) * number_of_enemies);
  is_awake -> total_size = number_of_enemies; 
  is_awake -> current_size = 0; /* Starts with 0 enemies in the awaken array */ 
}

int subtract_to_max (int x, int y)
{
  int res;
  if (x > y) res = x - y;
  else res = y - x;
  return res;
}

/* Calculates the distance from the player to the enemy */
int distance_from_character (Character *character, Enemy enemy)
{
  int res;
  res = subtract_to_max (character -> y, enemy.y) + subtract_to_max (character -> x, enemy.x);
  return res;
}

/* 
  When the enemy is in a certain range from the player, it awakens (is added to the
  awaken array, inicating that it will pursue the player to kill him) */
void init_awaken_enemies (Character *character, Enemy *enemies, Awake *is_awake)
{
  int index, distance;

  for (index = 0; index < is_awake -> total_size; index++)
  {
    /* Distance between the player and the enemy */
    distance = distance_from_character (character, enemies[index]);
    if (distance < AWAKE_RANGE && enemies[index].awake == 1)
    {
      is_awake -> enemies_awaken[is_awake -> current_size] = enemies[index];
      is_awake -> current_size ++;
      enemies[index].awake = 0;
    }
  }
}

void build_path (Awake *is_awake, Character *objective, char **map, Node *place_holder)
{
  for (int index = 0; index < is_awake -> current_size; index ++)
  {
    /* Starting the single first node */
    Node *origin_node = malloc (sizeof (Node));
    init_origin_node (objective, origin_node, &(is_awake -> enemies_awaken[index]));

    Path_queue *path = malloc (sizeof (Path_queue));  /* Path builder */
    init_queue (path);
    insert_queue (path, *origin_node);  /* Inserts first node in the queue */

    /* 
      If an enemy has group desire (it will only attack once there are a certain
      number of enemies awaken), it searches for that required desire, be it by
      warning out the enemies and starting to fight, if they are smart, or by
      running away to the nearest enemy spawn point, expecting to bring someone to
      fight with them, if they are genius. This, ofcourse, only happens when the
      group desire isn't already achieved by the current awaken enemies 
    if (is_awake -> current_size < is_awake -> enemies_awaken[index].tag -> group_desire)
    {
      Node node = closest_enemy ();
    }
    else{
      Node node = find_path (objective, map, place_holder, path);
    }
    */

    Node node = find_path (objective, map, place_holder, path);

    while (node.prev != NULL)
    {
      map[node.row][node.col] = '=';
      node = *(node.prev);
    }

    free (origin_node);
    origin_node = NULL;

    free (path->nodes);
    path->nodes = NULL;

    free (path);
    path = NULL;
  }
}